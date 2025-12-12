#include <janet.h>
#include <libical/ical.h>

static Janet cfun_table_from_ics(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);

  JanetTable *result = janet_table(3);

  // we have the whole string in memory
  // improvement: parse components from memory buffer line by line 
  icalcomponent *component = icalparser_parse_string(janet_getbuffer(argv, 0)->data);

  if(!icalerrno || component == NULL) {
    
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

          JanetTable *event = janet_table(icalcomponent_count_properties(c, ICAL_ANY_PROPERTY));

          // collect multi-valued properties into arrays
          // see https://libical.github.io/libical/book/UsingLibical.html#43-multi-valued-properties
          JanetArray *categories = janet_array(icalcomponent_count_properties(c, ICAL_CATEGORIES_PROPERTY));
          JanetArray *attach = janet_array(icalcomponent_count_properties(c, ICAL_ATTACH_PROPERTY));
          JanetArray *attendee = janet_array(icalcomponent_count_properties(c, ICAL_ATTENDEE_PROPERTY));
          JanetArray *comment = janet_array(icalcomponent_count_properties(c, ICAL_COMMENT_PROPERTY));
          JanetArray *contact = janet_array(icalcomponent_count_properties(c, ICAL_CONTACT_PROPERTY));
          JanetArray *exdate = janet_array(icalcomponent_count_properties(c, ICAL_EXDATE_PROPERTY));
          JanetArray *rstatus = janet_array(icalcomponent_count_properties(c, ICAL_REQUESTSTATUS_PROPERTY));
          JanetArray *related = janet_array(icalcomponent_count_properties(c, ICAL_RELATEDTO_PROPERTY));
          JanetArray *resources = janet_array(icalcomponent_count_properties(c, ICAL_RESOURCES_PROPERTY));
          JanetArray *rdate = janet_array(icalcomponent_count_properties(c, ICAL_RDATE_PROPERTY));

          icalproperty *prop;
          icalparameter *param;

          // Early extraction of UID for later reference
          prop = icalcomponent_get_first_property(c, ICAL_UID_PROPERTY);
          janet_table_put(event,
                          janet_cstringv("uid"),
                          janet_cstringv(icalproperty_get_uid(prop)));

          
          // Early extraction of DTSTART - it is a dependency for RDATE
          prop = icalcomponent_get_first_property(c, ICAL_DTSTART_PROPERTY);
          icaltimetype dtstart;
          if (prop != 0) {
            dtstart = icalproperty_get_dtstart(prop);
            param = icalproperty_get_first_parameter(prop, ICAL_TZID_PARAMETER);
            if (param != 0) {
              const char* tzid = icalparameter_get_tzid(param);
              icaltime_set_timezone(&dtstart, icaltimezone_get_builtin_timezone(tzid));
              janet_table_put(event,
                              janet_cstringv("dtstart"),
                              janet_wrap_integer(icaltime_as_timet_with_zone(dtstart,
                                                                             icaltimezone_get_builtin_timezone(tzid))));
            } else {
              janet_table_put(event,
                              janet_cstringv("dtstart"),
                              janet_wrap_integer(icaltime_as_timet(dtstart)));
            }
          } else {
            janet_panicf("Event %s: DTSTART missing.\n",
                         janet_unwrap_string(janet_table_get(event, janet_cstringv("uid"))));
          }

          // Early extraction of DTEND and DURATION - either one of them is a dependency for RDATE
          prop = icalcomponent_get_first_property(c, ICAL_DTEND_PROPERTY);
          icaltimetype dtend;
          if (prop != 0) {
            dtend = icalproperty_get_dtend(prop);
            param = icalproperty_get_first_parameter(prop, ICAL_TZID_PARAMETER);
            if (param != 0) {
              const char* tzid = icalparameter_get_tzid(param);
              icaltime_set_timezone(&dtend, icaltimezone_get_builtin_timezone(tzid));
              janet_table_put(event,
                              janet_cstringv("dtend"),
                              janet_wrap_integer(icaltime_as_timet_with_zone(dtend,
                                                                             icaltimezone_get_builtin_timezone(tzid))));
            } else {
              janet_table_put(event,
                              janet_cstringv("dtend"),
                              janet_wrap_integer(icaltime_as_timet(dtend)));
            }
            janet_table_put(event,
                            janet_cstringv("duration"),
                            janet_wrap_integer(icaldurationtype_as_int(icaltime_subtract(dtend, dtstart))));
          } else {
            prop = icalcomponent_get_first_property(c, ICAL_DURATION_PROPERTY);
            if (prop == 0) janet_panicf("Event %s: Neither DTEND nor DURATION found.\n",
                                        janet_unwrap_string(janet_table_get(event, janet_cstringv("uid"))));
            struct icaldurationtype duration = icalproperty_get_duration(prop);
            janet_table_put(event,
                            janet_cstringv("duration"),
                            janet_wrap_integer(icaldurationtype_as_int(duration)));
            janet_table_put(event,
                            janet_cstringv("dtend"),
                            janet_wrap_integer(icaltime_as_timet(icaltime_add(dtstart, duration))));
          }

          // Bulk extraction of all other parameters
          for (prop = icalcomponent_get_first_property(c, ICAL_ANY_PROPERTY);
               prop != 0;
               prop = icalcomponent_get_next_property(c, ICAL_ANY_PROPERTY))
            {
              switch (icalproperty_isa(prop)) {
              case ICAL_DTSTAMP_PROPERTY:
                janet_table_put(event, janet_cstringv("dtstamp"), janet_wrap_integer(icaltime_as_timet(icalproperty_get_dtstamp(prop))));
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
                janet_array_push(categories, janet_cstringv(icalproperty_get_categories(prop)));
                janet_table_put(event, janet_cstringv("categories"),  janet_wrap_array(categories));
                break;
              case ICAL_COMMENT_PROPERTY:
                janet_array_push(comment, janet_cstringv(icalproperty_get_comment(prop)));
                janet_table_put(event, janet_cstringv("comment"),  janet_wrap_array(comment));
                break;
              case ICAL_CONTACT_PROPERTY:
                janet_array_push(contact, janet_cstringv(icalproperty_get_contact(prop)));
                janet_table_put(event, janet_cstringv("contact"),  janet_wrap_array(contact));
                break;
              case ICAL_EXDATE_PROPERTY:
                janet_array_push(exdate, janet_wrap_integer(icaltime_as_timet(icalproperty_get_exdate(prop))));
                janet_table_put(event, janet_cstringv("exdate"),  janet_wrap_array(exdate));
                break;
              case ICAL_REQUESTSTATUS_PROPERTY:
                janet_array_push(rstatus, janet_cstringv(icalreqstattype_as_string(icalproperty_get_requeststatus(prop))));
                janet_table_put(event, janet_cstringv("rstatus"),  janet_wrap_array(rstatus));
                break;
              case ICAL_RELATEDTO_PROPERTY:
                janet_array_push(related, janet_cstringv(icalproperty_get_relatedto(prop)));
                janet_table_put(event, janet_cstringv("related"),  janet_wrap_array(related));
                break;
              case ICAL_RESOURCES_PROPERTY:
                janet_array_push(resources, janet_cstringv(icalproperty_get_resources(prop)));
                janet_table_put(event, janet_cstringv("resources"),  janet_wrap_array(resources));
                break;
              case ICAL_RDATE_PROPERTY: {
                param = icalproperty_get_first_parameter(prop, ICAL_VALUE_PARAMETER);
                struct icaldatetimeperiodtype dtp = icalproperty_get_rdate(prop);
                JanetTable *revent = janet_table(2);
                
                icaltimetype rstart = dtp.time;
                int start0 = janet_unwrap_integer(janet_table_get(event, janet_cstringv("dtstart")));
                int duration0 = 0;
                
                // need DTEND or DURATION for calculation of end of event
                // recurrence - but it should be there because we sourced
                // those properties first
                if (!janet_checktype(janet_table_find(event, janet_cstringv("dtend"))->key, JANET_NIL)) {
                  int end0 = janet_unwrap_integer(janet_table_get(event, janet_cstringv("dtend")));
                  duration0 = end0 - start0;
                } else if (!janet_checktype(janet_table_find(event, janet_cstringv("duration"))->key, JANET_NIL)) {
                  duration0 = janet_unwrap_integer(janet_table_get(event, janet_cstringv("duration")));
                } else {
                  janet_panicf("No DTEND or DURATION property found.\n"); // Just pro forma, this case is caught by the validator.
                }

                // Distinguish 3 cases: PERIOD, DATE-TIME, DATE
                if (0 == strcmp(icalparameter_enum_to_string(icalparameter_get_value(param)), "PERIOD")) {
                  janet_table_put(revent, janet_cstringv("start"), janet_wrap_integer(icaltime_as_timet(dtp.period.start)));
                  if (0 == icaltime_as_timet(dtp.period.end)) {
                    janet_table_put(revent, janet_cstringv("end"), janet_wrap_integer(icaltime_as_timet(icaltime_add(dtp.period.start,
                                                                                                                     dtp.period.duration))));                    
                  } else {
                    janet_table_put(revent, janet_cstringv("end"), janet_wrap_integer(icaltime_as_timet(dtp.period.end)));
                  }
                  janet_array_push(rdate, janet_wrap_struct(janet_table_to_struct(revent)));

                } else if (0 == strcmp(icalparameter_enum_to_string(icalparameter_get_value(param)), "DATE-TIME")) {
                  janet_table_put(revent, janet_cstringv("end"), janet_wrap_integer(icaltime_as_timet(rstart) + duration0));
                  janet_table_put(revent, janet_cstringv("start"), janet_wrap_integer(icaltime_as_timet(rstart)));
                  janet_array_push(rdate, janet_wrap_struct(janet_table_to_struct(revent)));

                } else if (0 == strcmp(icalparameter_enum_to_string(icalparameter_get_value(param)), "DATE")) {
                  icaltimetype lrdate = {
                    .year = rstart.year,
                    .month = rstart.month,
                    .day = rstart.day,
                    .hour = dtstart.hour,
                    .minute = dtstart.minute,
                    .second = dtstart.second,
                    .zone = dtstart.zone,
                    .is_date = 0
                  };
                  janet_table_put(revent, janet_cstringv("end"), janet_wrap_integer(icaltime_as_timet(lrdate) + duration0));
                  janet_table_put(revent, janet_cstringv("start"), janet_wrap_integer(icaltime_as_timet(lrdate)));
                  janet_array_push(rdate, janet_wrap_struct(janet_table_to_struct(revent)));
                  
                } else {
                  janet_panicf("Invalid RDATE parameter.\n"); // Just pro forma, this case is caught by the validator.
                }

                janet_table_put(event, janet_cstringv("rdate"),  janet_wrap_array(rdate));
              }
                break;
              default:
              
              }
            }
          janet_array_push(events, janet_wrap_table(event));
        }
      janet_table_put(result, janet_cstringv("events"), janet_wrap_array(events));
    }
   
    return janet_wrap_table(result);

  } else {
    janet_panicf("Error while parsing ics data\n");
  }

  icalcomponent_free(component);
  
}

static JanetReg cfuns[] = {
  {"table-from-ics", cfun_table_from_ics, "(table-from-ics <buffer>)\n\nreturns a table from ICS data"},
  {NULL, NULL, NULL}
};

JANET_MODULE_ENTRY(JanetTable *env) {
  janet_cfuns(env, "ical", cfuns);
}


