(import jip)
(use judge)
(use ../helpers)

(test (jip/table-from-ics (read-from-file "test/ics-files/libical-example.ics"))
  @{:events @[@{:attendees @["MAILTO:employee-A@host.com"]
                :categories @["MEETING"]
                :class "PUBLIC"
                :created 889448400
                :description "Project XYZ Review Meeting"
                :dtend 889713000
                :dtstamp 889485000
                :dtstart 889709400
                :duration 3600
                :location "1CP Conference Room 4350"
                :organizer "MAILTO:mrbig@host.com"
                :summary "XYZ Project Review"
                :uid "guid-1.host1.com"}]
    :prodid "-//hacksw/handcal//NONSGML v1.0//EN"
    :version "2.0"})
