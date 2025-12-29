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
  "Consolidates base and recurring event dates into one set, named `schedule`."
  [jip-table]
  (def schedule @{})
  
  (loop [event :in (jip-table :events)]
    (put schedule (event :dtstart) (event :dtend))
    (loop [rdate :in (event :rdates)]
      (put schedule (rdate :start) (rdate :end)))
    (loop [exdate :in (event :exdates)]
      (put schedule exdate nil))
    # now clear redundant fields
    (map |(put event $ nil) [:dtstart :dtend :duration :rdates :exdates])
    # and replace with the consolidated schedule
    (put event :schedule schedule))

  jip-table)
