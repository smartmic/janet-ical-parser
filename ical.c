#include <janet.h>
#include <libical/ical.h>

static Janet cfun_table_from_ics(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);

  JanetTable *result = janet_table(3);
  icalcomponent *component;

  // we have the whole string in memory
  // improvement: parse components from memory buffer line by line 
  component = icalparser_parse_string(janet_unwrap_buffer(argv[0])->data);

  if (component == 0 || icalcomponent_isa(component) != ICAL_VCALENDAR_COMPONENT) {
    janet_panicf("No valid iCalendar component provided\n");
  } else {
    // RFC5546 check
    icalrestriction_check(component);
    if (icalcomponent_count_errors(component) != 0) {
      // TODO: export the ics with inserted error properties to indicate non-compliance
      janet_panicf("Semantic or syntactic error in provided iCalender data\n");
    }

    icalproperty *prop;
    for (prop = icalcomponent_get_first_property(component, ICAL_ANY_PROPERTY);
         prop != 0;
         prop = icalcomponent_get_next_property(component, ICAL_ANY_PROPERTY))
      {
        switch (icalproperty_isa(prop)) {
        case ICAL_PRODID_PROPERTY:
          janet_table_put(result, janet_cstringv("prodid"), janet_cstringv(icalproperty_get_prodid(prop)));
          break;
        case ICAL_VERSION_PROPERTY:
          janet_table_put(result, janet_cstringv("version"), janet_cstringv(icalproperty_get_version(prop)));
          break;
        default:
        }
      }

    JanetArray *events = janet_array(icalcomponent_count_components(component, ICAL_VEVENT_COMPONENT));
    
    icalcomponent *c;
    for (c = icalcomponent_get_first_component(component, ICAL_VEVENT_COMPONENT);
         c != 0;
         c = icalcomponent_get_next_component(component, ICAL_VEVENT_COMPONENT))
      {
        icalrestriction_check(c);

        JanetTable *event = janet_table(2);
        
        icalproperty *prop;
        icalparameter *param;
        for (prop = icalcomponent_get_first_property(c, ICAL_ANY_PROPERTY);
             prop != 0;
             prop = icalcomponent_get_next_property(c, ICAL_ANY_PROPERTY))
          {
            switch (icalproperty_isa(prop)) {
            case ICAL_UID_PROPERTY:
              janet_table_put(event, janet_cstringv("uid"), janet_cstringv(icalproperty_get_uid(prop)));
              break;
            case ICAL_DTSTAMP_PROPERTY:
              janet_table_put(event, janet_cstringv("dtstamp"), janet_wrap_integer(icaltime_as_timet(icalproperty_get_dtstamp(prop))));
              break;
            case ICAL_DTSTART_PROPERTY: {
              param = icalproperty_get_first_parameter(prop, ICAL_TZID_PARAMETER);
              const char* tzid = icalparameter_get_tzid(param);
              icaltimetype dtstart = icalproperty_get_dtstart(prop);
              icaltime_set_timezone(&dtstart, icaltimezone_get_builtin_timezone(tzid));
              janet_table_put(event,
                              janet_cstringv("dtstart"),
                              janet_wrap_integer(icaltime_as_timet_with_zone(dtstart,icaltimezone_get_builtin_timezone(tzid))));
            }
              break;
            case ICAL_CLASS_PROPERTY:
              janet_table_put(event, janet_cstringv("class"), janet_cstringv(icalproperty_enum_to_string(icalproperty_get_class(prop))));
              break;
            case ICAL_CREATED_PROPERTY:
              janet_table_put(event, janet_cstringv("created"), janet_wrap_integer(icaltime_as_timet(icalproperty_get_dtstamp(prop))));
              break;
            case ICAL_DESCRIPTION_PROPERTY:
              janet_table_put(event, janet_cstringv("description"), janet_cstringv(icalproperty_get_description(prop))); 
              break;
            case ICAL_GEO_PROPERTY:
              struct icalgeotype geo;
              geo = icalproperty_get_geo(prop);
              JanetTable *coordinates = janet_table(2);
              janet_table_put(coordinates, janet_cstringv("lon"), janet_wrap_number(geo.lon));
              janet_table_put(coordinates, janet_cstringv("lat"), janet_wrap_number(geo.lat));
              janet_table_put(event, janet_cstringv("geo"), janet_wrap_struct(janet_table_to_struct(coordinates)));
              break;
            case ICAL_LASTMODIFIED_PROPERTY:
              janet_table_put(event, janet_cstringv("last-mod"), janet_wrap_integer(icaltime_as_timet(icalproperty_get_lastmodified(prop))));
              break;
            case ICAL_LOCATION_PROPERTY:
              janet_table_put(event, janet_cstringv("location"),  janet_cstringv(icalproperty_get_location(prop)));
              break;
            case ICAL_ORGANIZER_PROPERTY:
              janet_table_put(event, janet_cstringv("organizer"), janet_cstringv(icalproperty_get_organizer(prop)));
              break;
            case ICAL_PRIORITY_PROPERTY:
              janet_table_put(event, janet_cstringv("priority"), janet_wrap_integer(icalproperty_get_priority(prop)));
              break;
            case ICAL_SEQUENCE_PROPERTY:
              janet_table_put(event, janet_cstringv("seq"), janet_wrap_integer(icalproperty_get_sequence(prop)));
              break;
            case ICAL_STATUS_PROPERTY:
              janet_table_put(event, janet_cstringv("status"),  janet_cstringv(icalproperty_enum_to_string(icalproperty_get_status(prop))));
              break;
            case ICAL_SUMMARY_PROPERTY:
              janet_table_put(event, janet_cstringv("summary"), janet_cstringv(icalproperty_get_summary(prop)));
              break;
            case ICAL_TRANSP_PROPERTY:
              janet_table_put(event, janet_cstringv("transp"), janet_cstringv(icalproperty_enum_to_string(icalproperty_get_transp(prop))));
              break;
            case ICAL_URL_PROPERTY:
              janet_table_put(event, janet_cstringv("url"), janet_cstringv(icalproperty_get_url(prop)));
              break;
            case ICAL_RECURRENCEID_PROPERTY:
              janet_table_put(event, janet_cstringv("recurid"), janet_wrap_integer(icaltime_as_timet(icalproperty_get_recurrenceid(prop))));
              break;
            case ICAL_RRULE_PROPERTY:
              janet_table_put(event, janet_cstringv("url"), janet_cstringv(icalproperty_as_ical_string(prop)));
              break;
            case ICAL_DTEND_PROPERTY: {
              param = icalproperty_get_first_parameter(prop, ICAL_TZID_PARAMETER);
              const char* tzid = icalparameter_get_tzid(param);
              icaltimetype dtend = icalproperty_get_dtend(prop);
              icaltime_set_timezone(&dtend, icaltimezone_get_builtin_timezone(tzid));
              janet_table_put(event,
                              janet_cstringv("dtend"),
                              janet_wrap_integer(icaltime_as_timet_with_zone(dtend,icaltimezone_get_builtin_timezone(tzid))));
            }
              break;
            case ICAL_DURATION_PROPERTY:
              struct icaldurationtype dur;
              dur = icalproperty_get_duration(prop);
              JanetTable *duration = janet_table(6);
              janet_table_put(duration, janet_cstringv("days"), janet_wrap_integer(dur.days));
              janet_table_put(duration, janet_cstringv("hours"), janet_wrap_integer(dur.hours));
              janet_table_put(duration, janet_cstringv("is_neg"), janet_wrap_integer(dur.is_neg));
              janet_table_put(duration, janet_cstringv("minutes"), janet_wrap_number(dur.minutes));
              janet_table_put(duration, janet_cstringv("seconds"), janet_wrap_number(dur.seconds));
              janet_table_put(duration, janet_cstringv("weeks"), janet_wrap_number(dur.weeks));
              janet_table_put(event, janet_cstringv("duration"), janet_wrap_struct(janet_table_to_struct(duration)));
              break;
            case ICAL_ATTACH_PROPERTY:
              if (icalattach_get_is_url(icalproperty_get_attach(prop))) {
                janet_table_put(event, janet_cstringv("attach"), janet_cstringv(icalattach_get_url(icalproperty_get_attach(prop))));
              } else {
                janet_table_put(event, janet_cstringv("attach"), janet_cstringv(icalattach_get_data(icalproperty_get_attach(prop))));
              }
              break;
            case ICAL_ATTENDEE_PROPERTY:
              janet_table_put(event, janet_cstringv("attendee"), janet_cstringv(icalproperty_get_attendee(prop)));
              break;
            case ICAL_CATEGORIES_PROPERTY:
              janet_table_put(event, janet_cstringv("categories"),  janet_cstringv(icalproperty_get_categories(prop)));
              break;
            case ICAL_COMMENT_PROPERTY:
              janet_table_put(event, janet_cstringv("comment"), janet_cstringv(icalproperty_get_comment(prop)));
              break;
            case ICAL_CONTACT_PROPERTY:
              janet_table_put(event, janet_cstringv("contact"),  janet_cstringv(icalproperty_get_contact(prop)));
              break;
            case ICAL_EXDATE_PROPERTY:
              janet_table_put(event, janet_cstringv("exdate"), janet_wrap_integer(icaltime_as_timet(icalproperty_get_exdate(prop))));
              break;
            case ICAL_REQUESTSTATUS_PROPERTY:
              janet_table_put(event, janet_cstringv("rstatus"), janet_wrap_integer(icaltime_as_timet(icalproperty_get_dtstamp(prop))));
              break;
            case ICAL_RELATEDTO_PROPERTY:
              janet_table_put(event, janet_cstringv("related"), janet_cstringv(icalproperty_get_relatedto(prop)));
              break;
            case ICAL_RESOURCES_PROPERTY:
              janet_table_put(event, janet_cstringv("resources"), janet_cstringv(icalproperty_get_resources(prop)));
              break;
            case ICAL_RDATE_PROPERTY:               
              janet_table_put(event, janet_cstringv("rdate"), janet_wrap_integer(icaltime_as_timet(icalproperty_get_dtstamp(prop))));
              break;
            default:
              
            }
          }
        janet_array_push(events, janet_wrap_table(event));
      }
    janet_table_put(result, janet_cstringv("events"), janet_wrap_array(events));
  }
   
  return janet_wrap_table(result);
}

static JanetReg cfuns[] = {
  {"table_from_ics", cfun_table_from_ics, "(table_from_ics <buffer>)\n\nreturns a table from ICS data"},
  {NULL, NULL, NULL}
};

JANET_MODULE_ENTRY(JanetTable *env) {
  janet_cfuns(env, "ical", cfuns);
}


