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
            default:
              
            }
            for (param = icalproperty_get_first_parameter(prop, ICAL_ANY_PARAMETER);
                 param != 0;
                 param = icalproperty_get_next_parameter(prop, ICAL_ANY_PARAMETER))
              {
                // printf("%s ", icalparameter_as_ical_string(param));
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


