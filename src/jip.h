#pragma once
#include <janet.h>
#include <libical/ical.h>

int jip_datetime_with_tzid(struct icaltimetype, icalproperty *);
