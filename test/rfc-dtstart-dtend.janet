(import jip)
(use judge)
(use ../helpers)

(test (jip/table-from-ics (read-from-file "test/ics-files/rfc-dtstart-dtend.ics"))
  @{"events" @[@{"dtend" 873313200
                 "dtstamp" 873118800
                 "dtstart" 873304200
                 "duration" 9000
                 "uid" "rfc-dtstart-dtend-regular@example.com"}
               @{"dtend" 873313200
                 "dtstamp" 873118800
                 "dtstart" 873304200
                 "duration" 9000
                 "uid" "rfc-dtstart-dtend-different-order@example.com"}
               @{"dtend" 873310500
                 "dtstamp" 873118800
                 "dtstart" 873304200
                 "duration" 6300
                 "uid" "rfc-dtstart-duration@example.com"}
               @{"dtend" 873313200
                 "dtstamp" 873118800
                 "dtstart" 873304200
                 "duration" 9000
                 "uid" "rfc-dtstart-dtend-floating-time@example.com"}
               @{"description" "Note: libical parses DATE and DATE-TIME identically\n> Dates have an identical structure to a time, but the time portion (hours,\nminutes and seconds) is always 00:00:00"
                 "dtend" 873244800
                 "dtstamp" 873072000
                 "dtstart" 873244800
                 "duration" 0
                 "uid" "rfc-dtstart-dtend-dates@example.com"}]
    "prodid" "-//ABC Corporation//NONSGML My Product//EN"
    "version" "2.0"})

