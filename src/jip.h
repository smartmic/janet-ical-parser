/*
 * SPDX-FileCopyrightText: 2026 Martin Michel <martin@famic.de>
 *
 * SPDX-License-Identifier: MPL-2.0
 */

#pragma once
#include <janet.h>
#include <libical/ical.h>

#define LOOK_AHEAD_DURATION "P365D"

int jip_datetime_with_tzid(struct icaltimetype, icalproperty *);
