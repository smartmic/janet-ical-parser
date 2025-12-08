(import ical)
(use judge)
(use ../helpers)

(test (ical/table-from-ics (read-from-file "test/ics-files/libical-example.ics"))
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

(test (ical/table-from-ics (read-from-file "test/ics-files/rfc5545-3.6.1.ics"))
  @{"events" @[@{"categories" @["BUSINESS" "HUMAN RESOURCES"]
                 "class" "PRIVATE"
                 "dtend" 873313200
                 "dtstamp" 873118800
                 "dtstart" 873304200
                 "summary" "Annual Employee Review"
                 "uid" "19970901T130000Z-123401@example.com"}
               @{"categories" @["BUSINESS" "HUMAN RESOURCES"]
                 "class" "PRIVATE"
                 "dtend" 873313200
                 "dtstamp" 873118800
                 "dtstart" 873304200
                 "rdate" @[{"end" 875727000 "start" 875718000}
                           {"end" 891576000 "start" 891568800}
                           {"end" 891662400 "start" 891651600}
                           {"end" 883681200 "start" 883672200}
                           {"end" 885322800 "start" 885313800}
                           {"end" 887742000 "start" 887733000}
                           {"end" 893185200 "start" 893176200}
                           {"end" 896209200 "start" 896200200}
                           {"end" 899578800 "start" 899569800}
                           {"end" 904676400 "start" 904667400}
                           {"end" 908391600 "start" 908382600}
                           {"end" 912279600 "start" 912270600}
                           {"end" 912366000 "start" 912357000}
                           {"end" 914612400 "start" 914603400}]
                 "summary" "Annual Employee Review"
                 "uid" "4222c4ef-c3a8-4251-8f0c-87641284d3a1@example.com"}]
    "prodid" "-//ABC Corporation//NONSGML My Product//EN"
    "version" "2.0"})
