(import ical)
(use judge)
(use ../helpers)

(test (ical/table-from-ics (read-from-file "test/ics-files/rfc-tzid.ics"))
  @{"events" @[@{"dtend" 873313200
                 "dtstamp" 873118800
                 "dtstart" 873304200
                 "duration" 9000
                 "uid" "rfc-tzid-utc@example.com"}
               @{"dtend" 873327600
                 "dtstamp" 873118800
                 "dtstart" 873318600
                 "duration" 9000
                 "uid" "rfc-tzid-america-new_york@example.com"}
               @{"dtend" 873284400
                 "dtstamp" 873118800
                 "dtstart" 873284400
                 "duration" 0
                 "uid" "rfc-tzid-mix@example.com"}
               @{"dtend" 1743382800
                 "dtstamp" 1743217200
                 "dtstart" 1743292800
                 "duration" 90000
                 "uid" "rfc-tzid-europe-berlin-dst-1@example.com"}
               @{"description" "There is no defined time between 02:00 AM and 02:59 AM, libical will assume one hour earlier"
                 "dtend" 1743382800
                 "dtstamp" 1743217200
                 "dtstart" 1743292800
                 "duration" 90000
                 "uid" "rfc-tzid-europe-berlin-dst-2A@example.com"}
               @{"dtend" 1743382800
                 "dtstamp" 1743217200
                 "dtstart" 1743296340
                 "duration" 86460
                 "uid" "rfc-tzid-europe-berlin-dst-2B@example.com"}
               @{"dtend" 1743382800
                 "dtstamp" 1743217200
                 "dtstart" 1743296400
                 "duration" 86400
                 "uid" "rfc-tzid-europe-berlin-dst-3@example.com"}
               @{"dtend" 1761526800
                 "dtstamp" 1761357600
                 "dtstart" 1761433200
                 "duration" 93600
                 "uid" "rfc-tzid-europe-berlin-standard-1@example.com"}
               @{"dtend" 1761526800
                 "dtstamp" 1761357600
                 "dtstart" 1761440400
                 "duration" 86400
                 "uid" "rfc-tzid-europe-berlin-standard-2@example.com"}
               @{"dtend" 1761526800
                 "dtstamp" 1761357600
                 "dtstart" 1761444000
                 "duration" 82800
                 "uid" "rfc-tzid-europe-berlin-standard-3@example.com"}]
    "prodid" "-//ABC Corporation//NONSGML My Product//EN"
    "version" "2.0"})
