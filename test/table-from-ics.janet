(use judge)
(import ical)

#(test (file/read stdin :all) @"BEGIN:VCALENDAR\nMETHOD:REQUEST\nPRODID: -//hacksw/handcal//NONSGML v1.0//EN\nBEGIN:VEVENT\nDTSTAMP:19980309T231000Z\nUID:guid-1.host1.com\nORGANIZER;ROLE=CHAIR:MAILTO:mrbig@host.com\nATTENDEE;RSVP=TRUE;ROLE=REQ-PARTICIPANT;CUTYPE=GROUP:\n  MAILTO:employee-A@host.com\nDESCRIPTION:Project XYZ Review Meeting\nCATEGORIES:MEETING\nCLASS:PUBLIC\nCREATED:19980309T130000Z\nSUMMARY:XYZ Project Review\nDTSTART;TZID=US-Eastern:19980312T083000\nDTEND;TZID=US-Eastern:19980312T093000\nLOCATION:1CP Conference Room 4350\nEND:VEVENT\nEND:VCALENDAR\n\n")

(test (ical/table_from_ics (file/read stdin :all))
  @{"events" @[@{"dtstamp" 889485000
                 "uid" "guid-1.host1.com"}]
    "prodid" "-//hacksw/handcal//NONSGML v1.0//EN"
    "version" "1.0"})
