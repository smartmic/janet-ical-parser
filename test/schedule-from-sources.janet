# SPDX-FileCopyrightText: 2026 Martin Michel <martin@famic.de>
#
# SPDX-License-Identifier: MPL-2.0

(import jip)
(use judge)


(test (jip/schedule-set-from-ics-file "test/ics-files/libical-example.ics")
  @{:events @[@{:attendees @["MAILTO:employee-A@host.com"]
                :categories @["MEETING"]
                :class "PUBLIC"
                :created 889448400
                :description "Project XYZ Review Meeting"
                :dtstamp 889485000
                :location "1CP Conference Room 4350"
                :organizer "MAILTO:mrbig@host.com"
                :schedule @{889709400 889713000}
                :summary "XYZ Project Review"
                :uid "guid-1.host1.com"}]
    :prodid "-//hacksw/handcal//NONSGML v1.0//EN"
    :version "2.0"})

(test (jip/schedule-set-from-ics-string
``
BEGIN:VCALENDAR
METHOD:REQUEST
VERSION:2.0
PRODID: -//hacksw/handcal//NONSGML v1.0//EN
BEGIN:VEVENT
DTSTAMP:19980309T231000Z
UID:guid-1.host1.com
ORGANIZER;ROLE=CHAIR:MAILTO:mrbig@host.com
ATTENDEE;RSVP=TRUE;ROLE=REQ-PARTICIPANT;CUTYPE=GROUP:
  MAILTO:employee-A@host.com
DESCRIPTION:Project XYZ Review Meeting
CATEGORIES:MEETING
CLASS:PUBLIC
CREATED:19980309T130000Z
SUMMARY:XYZ Project Review
DTSTART;TZID=America/New_York:19980312T083000
DTEND;TZID=America/New_York:19980312T093000
LOCATION:1CP Conference Room 4350
END:VEVENT
END:VCALENDAR
``)
  @{:events @[@{:attendees @["MAILTO:employee-A@host.com"]
                :categories @["MEETING"]
                :class "PUBLIC"
                :created 889448400
                :description "Project XYZ Review Meeting"
                :dtstamp 889485000
                :location "1CP Conference Room 4350"
                :organizer "MAILTO:mrbig@host.com"
                :schedule @{889709400 889713000}
                :summary "XYZ Project Review"
                :uid "guid-1.host1.com"}]
    :prodid "-//hacksw/handcal//NONSGML v1.0//EN"
    :version "2.0"})
