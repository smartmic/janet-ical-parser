#pragma once
#include <janet.h>
#include <libical/ical.h>

#define LOOK_AHEAD_DURATION "P365D"

int jip_datetime_with_tzid(struct icaltimetype, icalproperty *);
