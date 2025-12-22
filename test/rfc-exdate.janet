(import jip)
(use judge)
(use ../helpers)

(test (jip/table-from-ics (read-from-file "test/ics-files/rfc-exdate.ics"))
  @{"events" @[@{"dtend" 859942800
                 "dtstamp" 873118800
                 "dtstart" 859912200
                 "duration" 30600
                 "exdates" @[828406800
                             828493200
                             828579600
                             828424800
                             828511200
                             828597600]
                 "uid" "19970901T130000Z-123402@example.com"}]
    "prodid" "-//ABC Corporation//NONSGML My Product//EN"
    "version" "2.0"})
