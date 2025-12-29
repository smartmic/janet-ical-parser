(import jip/native)
(import ./helpers)

(defn table-from-ics-string
  "Convert an iCalendar text snippet to a Janet table"
  [ics-string & jip-conf]
  (native/table-from-ics ics-string ;jip-conf))

(defn table-from-ics-file
  "Convert an iCalender .ics file to a Janet table"
  [ics-file-path & jip-conf]
  (native/table-from-ics (helpers/read-from-file ics-file-path) ;jip-conf))

(defn schedule-set
  "TODO Consolidates base and recurring event dates into one set."
  [jip-data]
  jip-data)
