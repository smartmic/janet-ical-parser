(import ical)
(use judge)
(use ../helpers)

(test (ical/table_from_ics (read-from-file "test/ics-files/libical-example.ics"))
  @{"events" @[@{"attendee" "MAILTO:employee-A@host.com"
                 "categories" @["MEETING"]
                 "class" "PUBLIC"
                 "created" 889448400
                 "description" "Project XYZ Review Meeting"
                 "dtend" 889713000
                 "dtstamp" 889485000
                 "dtstart" 889709400
                 "location" "1CP Conference Room 4350"
                 "organizer" "MAILTO:mrbig@host.com"
                 "summary" "XYZ Project Review"
                 "uid" "guid-1.host1.com"}]
    "prodid" "-//hacksw/handcal//NONSGML v1.0//EN"
    "version" "1.0"})

(test (ical/table_from_ics (read-from-file "test/ics-files/rfc5545-3.6.1.ics"))
  @{"events" @[@{"categories" @["BUSINESS" "HUMAN RESOURCES"]
                 "class" "PRIVATE"
                 "dtend" 873313200
                 "dtstamp" 873118800
                 "dtstart" 873304200
                 "summary" "Annual Employee Review"
                 "uid" "19970901T130000Z-123401@example.com"}]
    "prodid" "-//ABC Corporation//NONSGML My Product//EN"
    "version" "2.0"})
