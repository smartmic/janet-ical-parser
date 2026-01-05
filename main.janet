# SPDX-FileCopyrightText: 2026 Martin Michel <martin@famic.de>
#
# SPDX-License-Identifier: MPL-2.0

# main.janet

# Import package
(import jip)

# Define example event from https://datatracker.ietf.org/doc/html/rfc5545#page-55
(def ics-vevent ``
BEGIN:VCALENDAR
VERSION:2.0
PRODID:-//hacksw/handcal//NONSGML v1.0//EN
BEGIN:VEVENT
UID:19970901T130000Z-123403@example.com
DTSTAMP:19970901T130000Z
DTSTART;VALUE=DATE:19971102
SUMMARY:Our Blissful Anniversary
TRANSP:TRANSPARENT
CLASS:CONFIDENTIAL
CATEGORIES:ANNIVERSARY,PERSONAL,SPECIAL OCCASION
RRULE:FREQ=YEARLY
END:VEVENT
END:VCALENDAR
  ``)

# Get Janet table as consolidated schedule. Look ahead and find all the
# recurrences up to the year 2003. If you omit the optional jip-conf table, look
# ahead for unlimited recurrences is one year from start date.
(def jip-conf @{:until-date "20030101T000000Z"})
(def result (jip/schedule-set-from-ics-string ics-vevent jip-conf))

# Pretty-print to stdout for demo purpose
(print (string/format "%P" result))
