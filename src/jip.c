#include <jip.h>
#include <stdbool.h>

static Janet cfun_table_from_ics(int32_t argc, Janet *argv) {
  janet_arity(argc, 1, 2);

  JanetTable *result = janet_table(3);

  icalerror_clear_errno(); // needed because icalerrno is set globally and
                           // persists between function calls
  struct icaltimetype until_date = icaltime_null_time();
  if (argc == 2) {
    JanetTable *jip_conf = janet_gettable(argv, 1);
    
    if (janet_checktype(janet_table_find(jip_conf, janet_ckeywordv("until-date"))->value, JANET_STRING)) {
      const char *time_string = janet_unwrap_string(janet_table_get(jip_conf, janet_ckeywordv("until-date")));
      until_date = icaltime_from_string(time_string);
      if (icalerrno == ICAL_MALFORMEDDATA_ERROR) {
        janet_panicf("Invalid \"until-date\" in table \"jip-conf\"; libical says %s.\n", icalerror_strerror(icalerrno));
      }
    } else {
      janet_panicf("Invalid Janet type for key \"until-date\" in table \"jip-conf\".\n");
    }
  }
  
  // we have the whole string in memory
  icalcomponent *component = icalparser_parse_string(janet_getstring(argv, 0));

  if(!icalerrno || component != NULL) {
    
    if (component == 0 || icalcomponent_isa(component) != ICAL_VCALENDAR_COMPONENT) {
      janet_panicf("No valid iCalendar component provided\n");
    } else {
      // RFC5546 check
      icalrestriction_check(component);
      if (icalcomponent_count_errors(component) != 0) {

        FILE *fPtr;
        fPtr = fopen("x-lic-errors.ics", "w");

        if (fPtr == NULL) {
          janet_panicf("Error opening file to write RFC 5545 validation errors.\n");
        }

        const char *data = icalcomponent_as_ical_string(component);
        fputs(data, fPtr);
        fclose(fPtr);

        janet_panicf("Semantic or syntactic error in provided iCalender data\n");
      }

      icalproperty *prop;
      for (prop = icalcomponent_get_first_property(component, ICAL_ANY_PROPERTY);
           prop != 0;
           prop = icalcomponent_get_next_property(component, ICAL_ANY_PROPERTY))
        {
          switch (icalproperty_isa(prop)) {
          case ICAL_PRODID_PROPERTY:
            janet_table_put(result, janet_ckeywordv("prodid"), janet_cstringv(icalproperty_get_prodid(prop)));
            break;
          case ICAL_VERSION_PROPERTY:
            janet_table_put(result, janet_ckeywordv("version"), janet_cstringv(icalproperty_get_version(prop)));
            break;
          default:
            ;
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
          JanetArray *attachments = janet_array(icalcomponent_count_properties(c, ICAL_ATTACH_PROPERTY));
          JanetArray *attendees = janet_array(icalcomponent_count_properties(c, ICAL_ATTENDEE_PROPERTY));
          JanetArray *comments = janet_array(icalcomponent_count_properties(c, ICAL_COMMENT_PROPERTY));
          JanetArray *contacts = janet_array(icalcomponent_count_properties(c, ICAL_CONTACT_PROPERTY));
          JanetArray *exdates = janet_array(icalcomponent_count_properties(c, ICAL_EXDATE_PROPERTY));
          JanetArray *rstatus = janet_array(icalcomponent_count_properties(c, ICAL_REQUESTSTATUS_PROPERTY));
          JanetArray *related = janet_array(icalcomponent_count_properties(c, ICAL_RELATEDTO_PROPERTY));
          JanetArray *resources = janet_array(icalcomponent_count_properties(c, ICAL_RESOURCES_PROPERTY));
          JanetArray *rdates = janet_array(icalcomponent_count_properties(c, ICAL_RDATE_PROPERTY));

          icalproperty *prop;
          icalparameter *param;

          // Early extraction of UID for later reference
          prop = icalcomponent_get_first_property(c, ICAL_UID_PROPERTY);
          janet_table_put(event,
                          janet_ckeywordv("uid"),
                          janet_cstringv(icalproperty_get_uid(prop)));

          
          // Early extraction of DTSTART - it is a dependency for RDATE
          prop = icalcomponent_get_first_property(c, ICAL_DTSTART_PROPERTY);
          icaltimetype dtstart;
          if (prop != 0) {
            dtstart = icalproperty_get_dtstart(prop);
            param = icalproperty_get_first_parameter(prop, ICAL_TZID_PARAMETER); // We need TZID for DTSTART for RRULEs
            if (param != 0) {
              const char* tzid = icalparameter_get_tzid(param);
              icaltime_set_timezone(&dtstart, icaltimezone_get_builtin_timezone(tzid));
            }
            janet_table_put(event,
                            janet_ckeywordv("dtstart"),
                            janet_wrap_integer(jip_datetime_with_tzid(dtstart, prop)));
          } else {
            janet_panicf("Event %s: DTSTART missing.\n",
                         janet_unwrap_string(janet_table_get(event, janet_ckeywordv("uid"))));
          }

          // Early extraction of DTEND and DURATION - either one of them is a dependency for RDATE
          prop = icalcomponent_get_first_property(c, ICAL_DTEND_PROPERTY);
          icaltimetype dtend;
          if (prop != 0) {
            dtend = icalproperty_get_dtend(prop);
            janet_table_put(event,
                            janet_ckeywordv("dtend"),
                            janet_wrap_integer(jip_datetime_with_tzid(dtend, prop)));
            if (icaltime_compare(dtend, dtstart) >= 0) {
              janet_table_put(event,
                              janet_ckeywordv("duration"),
                              // workaround with janet values, libical icaltime_substract cannot deal with different time zones:
                              janet_wrap_integer(janet_unwrap_integer(janet_table_get(event, janet_ckeywordv("dtend")))
                                                 -
                                                 janet_unwrap_integer(janet_table_get(event, janet_ckeywordv("dtstart")))));
            } else {
              janet_panicf("DTEND is before DTSTART.\n");
            }
          } else {
            prop = icalcomponent_get_first_property(c, ICAL_DURATION_PROPERTY);
            if (prop == 0) janet_panicf("Event %s: Neither DTEND nor DURATION found.\n",
                                        janet_unwrap_string(janet_table_get(event, janet_ckeywordv("uid"))));
            struct icaldurationtype duration = icalproperty_get_duration(prop);
            janet_table_put(event,
                            janet_ckeywordv("duration"),
                            janet_wrap_integer(icaldurationtype_as_int(duration)));
            janet_table_put(event,
                            janet_ckeywordv("dtend"),
                            janet_wrap_integer(janet_unwrap_integer(janet_table_get(event, janet_ckeywordv("dtstart")))
                                               +
                                               icaldurationtype_as_int(duration)));
          }

          // Bulk extraction of all other parameters
          for (prop = icalcomponent_get_first_property(c, ICAL_ANY_PROPERTY);
               prop != 0;
               prop = icalcomponent_get_next_property(c, ICAL_ANY_PROPERTY))
            {
              switch (icalproperty_isa(prop)) {
              case ICAL_DTSTAMP_PROPERTY:
                janet_table_put(event, janet_ckeywordv("dtstamp"), janet_wrap_integer(icaltime_as_timet(icalproperty_get_dtstamp(prop))));
                break;
              case ICAL_CLASS_PROPERTY:
                janet_table_put(event, janet_ckeywordv("class"), janet_cstringv(icalproperty_enum_to_string(icalproperty_get_class(prop))));
                break;
              case ICAL_CREATED_PROPERTY:
                janet_table_put(event, janet_ckeywordv("created"), janet_wrap_integer(icaltime_as_timet(icalproperty_get_created(prop))));
                break;
              case ICAL_DESCRIPTION_PROPERTY:
                janet_table_put(event, janet_ckeywordv("description"), janet_cstringv(icalproperty_get_description(prop))); 
                break;
              case ICAL_GEO_PROPERTY:
                struct icalgeotype geo;
                geo = icalproperty_get_geo(prop);
                JanetTable *coordinates = janet_table(2);
                janet_table_put(coordinates, janet_ckeywordv("lon"), janet_wrap_number(geo.lon));
                janet_table_put(coordinates, janet_ckeywordv("lat"), janet_wrap_number(geo.lat));
                janet_table_put(event, janet_ckeywordv("geo"), janet_wrap_struct(janet_table_to_struct(coordinates)));
                break;
              case ICAL_LASTMODIFIED_PROPERTY:
                janet_table_put(event, janet_ckeywordv("last-mod"), janet_wrap_integer(icaltime_as_timet(icalproperty_get_lastmodified(prop))));
                break;
              case ICAL_LOCATION_PROPERTY:
                // TODO: altrepparam, languageparam
                janet_table_put(event, janet_ckeywordv("location"),  janet_cstringv(icalproperty_get_location(prop)));
                break;
              case ICAL_ORGANIZER_PROPERTY:
                // TODO: cnparam, dirparam, sentbyparam
                janet_table_put(event, janet_ckeywordv("organizer"), janet_cstringv(icalproperty_get_organizer(prop)));
                break;
              case ICAL_PRIORITY_PROPERTY:
                janet_table_put(event, janet_ckeywordv("priority"), janet_wrap_integer(icalproperty_get_priority(prop)));
                break;
              case ICAL_SEQUENCE_PROPERTY:
                janet_table_put(event, janet_ckeywordv("seq"), janet_wrap_integer(icalproperty_get_sequence(prop)));
                break;
              case ICAL_STATUS_PROPERTY:
                janet_table_put(event, janet_ckeywordv("status"),  janet_cstringv(icalproperty_enum_to_string(icalproperty_get_status(prop))));
                break;
              case ICAL_SUMMARY_PROPERTY:
                janet_table_put(event, janet_ckeywordv("summary"), janet_cstringv(icalproperty_get_summary(prop)));
                break;
              case ICAL_TRANSP_PROPERTY:
                janet_table_put(event, janet_ckeywordv("transp"), janet_cstringv(icalproperty_enum_to_string(icalproperty_get_transp(prop))));
                break;
              case ICAL_URL_PROPERTY:
                janet_table_put(event, janet_ckeywordv("url"), janet_cstringv(icalproperty_get_url(prop)));
                break;
              case ICAL_RECURRENCEID_PROPERTY:
                // TODO:
                janet_table_put(event, janet_ckeywordv("recurid"), janet_wrap_integer(icaltime_as_timet(icalproperty_get_recurrenceid(prop))));
                break;
              case ICAL_RRULE_PROPERTY: 
                struct icalrecurrencetype recur  = icalproperty_get_rrule(prop);
                int start0 = janet_unwrap_integer(janet_table_get(event, janet_ckeywordv("dtstart")));
                int duration0 = 0;
                if (!janet_checktype(janet_table_find(event, janet_ckeywordv("duration"))->key, JANET_NIL)) {
                  duration0 = janet_unwrap_integer(janet_table_get(event, janet_ckeywordv("duration")));
                } else {
                  janet_panicf("Failed to fetch event duration.\n");
                }

                JanetTable *revent = janet_table(2);

                icalrecur_iterator *ritr;
                struct icaltimetype next;
                ritr = icalrecur_iterator_new(recur, dtstart);
                
                // Workaround standard/daylight saving time transition within
                // RRULE: we introduce a fictional TZID parameter for RRULE
                // property (this is not compliant with RFC 5545) so that we
                // reuse jip_datetime_with_tzid for local time zone settings
                icalproperty_set_parameter_from_string(prop, "TZID", icaltimezone_get_display_name(dtstart.zone));

                if (icaltime_is_null_time(until_date)) {
                  struct icaldurationtype look_ahead_duration = icaldurationtype_from_string(LOOK_AHEAD_DURATION);
                  until_date = icaltime_add(dtstart, look_ahead_duration);
                }
                  
                next = icalrecur_iterator_next(ritr);
                while (!icaltime_is_null_time(next) && 1 != icaltime_compare(next, until_date)) {
                  janet_table_put(revent, janet_ckeywordv("end"), janet_wrap_integer(jip_datetime_with_tzid(next, prop) + duration0));
                  janet_table_put(revent, janet_ckeywordv("start"), janet_wrap_integer(jip_datetime_with_tzid(next, prop)));
                  janet_array_push(rdates, janet_wrap_struct(janet_table_to_struct(revent)));
                  next = icalrecur_iterator_next(ritr);
                }
                janet_table_put(event, janet_ckeywordv("rdates"),  janet_wrap_array(rdates));
                break;
              case ICAL_ATTACH_PROPERTY:
                // TODO: parameters
                if (icalattach_get_is_url(icalproperty_get_attach(prop))) {
                  janet_array_push(attachments, janet_cstringv(icalattach_get_url(icalproperty_get_attach(prop))));
                } else {
                  janet_array_push(attachments, janet_cstringv(icalattach_get_data(icalproperty_get_attach(prop))));
                }
                janet_table_put(event, janet_ckeywordv("attachments"), janet_wrap_array(attachments));
                break;
              case ICAL_ATTENDEE_PROPERTY:
                // TODO: parameters
                janet_array_push(attendees, janet_cstringv(icalproperty_get_attendee(prop)));
                janet_table_put(event, janet_ckeywordv("attendees"), janet_wrap_array(attendees));
                break;
              case ICAL_CATEGORIES_PROPERTY:
                janet_array_push(categories, janet_cstringv(icalproperty_get_categories(prop)));
                janet_table_put(event, janet_ckeywordv("categories"),  janet_wrap_array(categories));
                break;
              case ICAL_COMMENT_PROPERTY:
                janet_array_push(comments, janet_cstringv(icalproperty_get_comment(prop)));
                janet_table_put(event, janet_ckeywordv("comments"),  janet_wrap_array(comments));
                break;
              case ICAL_CONTACT_PROPERTY:
                // TODO: altrepparam, languageparam
                janet_array_push(contacts, janet_cstringv(icalproperty_get_contact(prop)));
                janet_table_put(event, janet_ckeywordv("contacts"),  janet_wrap_array(contacts));
                break;
              case ICAL_EXDATE_PROPERTY:
                icaltimetype exdate = icalproperty_get_exdate(prop);
                param = icalproperty_get_first_parameter(prop, ICAL_VALUE_PARAMETER);
                if (param != 0 && 0 == strcmp(icalparameter_enum_to_string(icalparameter_get_value(param)), "DATE")) {
                    icaltimetype mod_exdate = {
                      .year = exdate.year,
                      .month = exdate.month,
                      .day = exdate.day,
                      .hour = dtstart.hour,
                      .minute = dtstart.minute,
                      .second = dtstart.second,
                      .zone = dtstart.zone,
                      .is_date = 0
                    };
                    janet_array_push(exdates, janet_wrap_integer(jip_datetime_with_tzid(mod_exdate, prop)));
                } else {
                  janet_array_push(exdates, janet_wrap_integer(jip_datetime_with_tzid(exdate, prop)));
                }
                janet_table_put(event, janet_ckeywordv("exdates"),  janet_wrap_array(exdates));
                break;
              case ICAL_REQUESTSTATUS_PROPERTY:
                janet_array_push(rstatus, janet_cstringv(icalreqstattype_as_string(icalproperty_get_requeststatus(prop))));
                janet_table_put(event, janet_ckeywordv("rstatus"),  janet_wrap_array(rstatus));
                break;
              case ICAL_RELATEDTO_PROPERTY:
                janet_array_push(related, janet_cstringv(icalproperty_get_relatedto(prop)));
                janet_table_put(event, janet_ckeywordv("related"),  janet_wrap_array(related));
                break;
              case ICAL_RESOURCES_PROPERTY:
                // TODO: altrepparam, languageparam
                janet_array_push(resources, janet_cstringv(icalproperty_get_resources(prop)));
                janet_table_put(event, janet_ckeywordv("resources"),  janet_wrap_array(resources));
                break;
              case ICAL_RDATE_PROPERTY: {
                param = icalproperty_get_first_parameter(prop, ICAL_VALUE_PARAMETER);
                struct icaldatetimeperiodtype dtp = icalproperty_get_rdate(prop);
                JanetTable *revent = janet_table(2);
                
                icaltimetype rstart = dtp.time;
                int start0 = janet_unwrap_integer(janet_table_get(event, janet_ckeywordv("dtstart")));
                int duration0 = 0;
                
                // need DTEND or DURATION for calculation of end of event
                // recurrence - but it should be there because we sourced
                // those properties first
                if (!janet_checktype(janet_table_find(event, janet_ckeywordv("dtend"))->key, JANET_NIL)) {
                  int end0 = janet_unwrap_integer(janet_table_get(event, janet_ckeywordv("dtend")));
                  duration0 = end0 - start0;
                } else if (!janet_checktype(janet_table_find(event, janet_ckeywordv("duration"))->key, JANET_NIL)) {
                  duration0 = janet_unwrap_integer(janet_table_get(event, janet_ckeywordv("duration")));
                } else {
                  janet_panicf("No DTEND or DURATION property found.\n"); // Just pro forma, this case is caught by the validator.
                }

                // Distinguish 3 cases: PERIOD, DATE-TIME, DATE
                if (param != 0) {
                  if (0 == strcmp(icalparameter_enum_to_string(icalparameter_get_value(param)), "PERIOD")) {
                    janet_table_put(revent, janet_ckeywordv("start"), janet_wrap_integer(jip_datetime_with_tzid(dtp.period.start, prop)));
                    if (0 == icaltime_as_timet(dtp.period.end)) {
                      janet_table_put(revent, janet_ckeywordv("end"), janet_wrap_integer(jip_datetime_with_tzid(icaltime_add(dtp.period.start,
                                                                                                                            dtp.period.duration),
                                                                                                               prop)));                    
                    } else {
                      janet_table_put(revent, janet_ckeywordv("end"), janet_wrap_integer(jip_datetime_with_tzid(dtp.period.end, prop)));
                    }
                    janet_array_push(rdates, janet_wrap_struct(janet_table_to_struct(revent)));

                  } else if (0 == strcmp(icalparameter_enum_to_string(icalparameter_get_value(param)), "DATE-TIME")) {

                    janet_table_put(revent, janet_ckeywordv("end"), janet_wrap_integer(jip_datetime_with_tzid(rstart, prop) + duration0));
                    janet_table_put(revent, janet_ckeywordv("start"), janet_wrap_integer(jip_datetime_with_tzid(rstart, prop)));
                    janet_array_push(rdates, janet_wrap_struct(janet_table_to_struct(revent)));
                    
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
                    janet_table_put(revent, janet_ckeywordv("end"), janet_wrap_integer(jip_datetime_with_tzid(lrdate, prop) + duration0));
                    janet_table_put(revent, janet_ckeywordv("start"), janet_wrap_integer(jip_datetime_with_tzid(lrdate, prop)));
                    janet_array_push(rdates, janet_wrap_struct(janet_table_to_struct(revent)));
                    
                  }
                  
                } else {
                  janet_table_put(revent, janet_ckeywordv("end"), janet_wrap_integer(jip_datetime_with_tzid(rstart, prop) + duration0));
                  janet_table_put(revent, janet_ckeywordv("start"), janet_wrap_integer(jip_datetime_with_tzid(rstart, prop)));
                  janet_array_push(rdates, janet_wrap_struct(janet_table_to_struct(revent)));
                }

                janet_table_put(event, janet_ckeywordv("rdates"),  janet_wrap_array(rdates));
              }
                break;
              default:
                ;
              }
            }
          janet_array_push(events, janet_wrap_table(event));
        }
      janet_table_put(result, janet_ckeywordv("events"), janet_wrap_array(events));
    }
   
    return janet_wrap_table(result);

  } else {
    janet_panicf("Error while parsing ics data; libical says %s\n", icalerror_strerror(icalerrno));
  }

  icalcomponent_free(component);
  
}

static JanetReg cfuns[] = {
  {"table-from-ics", cfun_table_from_ics, "(table-from-ics <buffer>)\n\nreturns a table from ICS data"},
  {NULL, NULL, NULL}
};

JANET_MODULE_ENTRY(JanetTable *env) {
  janet_cfuns(env, "jip", cfuns);
}


