/*
 * SPDX-FileCopyrightText: 2026 Martin Michel <martin@famic.de>
 *
 * SPDX-License-Identifier: MPL-2.0
 */

#include <jip.h>

int jip_datetime_with_tzid(struct icaltimetype dttype, icalproperty *prop) {
  
  icalparameter *param = icalproperty_get_first_parameter(prop, ICAL_TZID_PARAMETER);
  if (param != 0) {
    const char* tzid = icalparameter_get_tzid(param);
    icaltime_set_timezone(&dttype, icaltimezone_get_builtin_timezone(tzid));
    return icaltime_as_timet_with_zone(dttype,
                                       icaltimezone_get_builtin_timezone(tzid));
  } else {
    return icaltime_as_timet(dttype);
  }
}
