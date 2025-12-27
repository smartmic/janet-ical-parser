(import jip)
(use judge)
(use ../helpers)

(test (jip/table-from-ics (read-from-file "test/ics-files/rfc-multi.ics"))
  @{:events @[@{:attachments @["CID:jsmith.part3.960817T083000.xyzMail@example.com"
                               "ftp://example.com/pub/ reports/r-960812.ps"]
                :attendees @["mailto:joecool@example.com"
                             "mailto:ildoit@example.com"]
                :categories @["BUSINESS" "HUMAN RESOURCES"]
                :class "PUBLIC"
                :comments @["The meeting really needs to include both ourselves and the customer. We can't hold this meeting without them. As a matter of fact, the venue for the meeting ought to be at their site. - - John"
                            "fyi"]
                :contacts @["Jim Dolittle, ABC Industries, +1-919-555-1234"
                            "Jim Dolittle, ABC Industries, +1-919-555-1234"
                            "Jim Dolittle, ABC Industries, +1-919-555-1234"]
                :created 828106200
                :description "Meeting to provide technical review for \"Phoenix\" design.\nHappy Face Conference Room. Phoenix design team MUST attend this meeting.\nRSVP to team leader."
                :dtend 859942800
                :dtstamp 873118800
                :dtstart 859912200
                :duration 30600
                :exdates @[828406800 828493200 828579600]
                :geo {:lat 37.386013 :lon -122.082932}
                :last-mod 840288600
                :location "Conference Room - F123, Bldg. 002"
                :organizer "mailto:jsmith@example.com"
                :priority 1
                :recurid 828316800
                :related @["jsmith.part7.19960817T083000.xyzMail@example.com"
                           "19960401-080045-4000F192713-0052@example.com"]
                :resources @["EASEL"
                             "PROJECTOR"
                             "VCR"
                             "Nettoyeur haute pression"]
                :rstatus @["2.0;Success."
                           "3.1;Invalid property value.;DTSTART:96-Apr-01"
                           "2.8;Success, repeating event ignored. Scheduled as a  single component.;RRULE:FREQ=WEEKLY\\;INTERVAL=2"
                           "4.1;Store Access Denied."
                           "3.7;Invalid Calendar User.;ATTENDEE: mailto:jsmith@example.com"]
                :seq 0
                :status "TENTATIVE"
                :summary "Laurel is in sensitivity awareness class."
                :transp "TRANSPARENT"
                :uid "19970901T130000Z-123402@example.com"
                :url "http://example.com/pub/calendars/jsmith/mytime.ics"}
              @{:dtend 1183939200
                :dtstamp 1177331672
                :dtstart 1182988800
                :duration 950400
                :summary "Festival International de Jazz de Montreal"
                :transp "TRANSPARENT"
                :uid "20070423T123432Z-541111@example.com"}]
    :prodid "-//ABC Corporation//NONSGML My Product//EN"
    :version "2.0"})
