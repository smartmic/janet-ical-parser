(import jip)
(use judge)


(test (jip/table-from-ics-file "test/ics-files/schedule-set.ics")
  @{:events @[@{:dtend 873313200
                :dtstamp 873118800
                :dtstart 873304200
                :duration 9000
                :uid "consolidated-schedule-set-01@example.com"}
              @{:dtend 873313200
                :dtstamp 873118800
                :dtstart 873304200
                :duration 9000
                :rdates @[{:end 873313200 :start 873304200}]
                :uid "consolidated-schedule-set-02@example.com"}
              @{:dtend 873313200
                :dtstamp 873118800
                :dtstart 873304200
                :duration 9000
                :rdates @[{:end 873306000 :start 873297000}]
                :uid "consolidated-schedule-set-03@example.com"}
              @{:dtend 873313200
                :dtstamp 873118800
                :dtstart 873304200
                :duration 9000
                :exdates @[873297000]
                :rdates @[{:end 873306000 :start 873297000}]
                :uid "consolidated-schedule-set-04@example.com"}
              @{:dtend 873313200
                :dtstamp 873118800
                :dtstart 873304200
                :duration 9000
                :exdates @[873304200]
                :rdates @[{:end 873306000 :start 873297000}]
                :uid "consolidated-schedule-set-05@example.com"}
              @{:dtend 873313200
                :dtstamp 873118800
                :dtstart 873304200
                :duration 9000
                :exdates @[873390600]
                :rdates @[{:end 873306000 :start 873297000}]
                :uid "consolidated-schedule-set-05@example.com"}
              @{:dtend 873313200
                :dtstamp 873118800
                :dtstart 873304200
                :duration 9000
                :exdates @[873304200]
                :uid "consolidated-schedule-set-06@example.com"}
              @{:dtend 873313200
                :dtstamp 873118800
                :dtstart 873304200
                :duration 9000
                :exdates @[876673800
                           877312800
                           881067600
                           881166600
                           882203400
                           882203400
                           882721800
                           882981000
                           883067400
                           883153800
                           883585800]
                :rdates @[{:end 874249200 :start 874240200}
                          {:end 875732400 :start 875723400}
                          {:end 876682800 :start 876673800}
                          {:end 876769200 :start 876760200}
                          {:end 877406400 :start 877312800}
                          {:end 877492800 :start 877482000}
                          {:end 879252071 :start 879243071}
                          {:end 881002800 :start 880993800}
                          {:end 881089200 :start 881080200}
                          {:end 881175600 :start 881166600}
                          {:end 881262000 :start 881253000}
                          {:end 881348400 :start 881339400}
                          {:end 881434800 :start 881425800}
                          {:end 881521200 :start 881512200}
                          {:end 881607600 :start 881598600}
                          {:end 881694000 :start 881685000}
                          {:end 881780400 :start 881771400}
                          {:end 881866800 :start 881857800}
                          {:end 881953200 :start 881944200}
                          {:end 882039600 :start 882030600}
                          {:end 882126000 :start 882117000}
                          {:end 882212400 :start 882203400}
                          {:end 882298800 :start 882289800}
                          {:end 882385200 :start 882376200}
                          {:end 882471600 :start 882462600}
                          {:end 882558000 :start 882549000}
                          {:end 882644400 :start 882635400}
                          {:end 882730800 :start 882721800}
                          {:end 882817200 :start 882808200}
                          {:end 882903600 :start 882894600}
                          {:end 882990000 :start 882981000}
                          {:end 883076400 :start 883067400}
                          {:end 883162800 :start 883153800}
                          {:end 883249200 :start 883240200}
                          {:end 883335600 :start 883326600}
                          {:end 883422000 :start 883413000}
                          {:end 883508400 :start 883499400}
                          {:end 883594800 :start 883585800}]
                :uid "consolidated-schedule-set-20@example.com"}]
    :prodid "-//ABC Corporation//NONSGML My Product//EN"
    :version "2.0"})

(test (jip/schedule-set (jip/table-from-ics-file "test/ics-files/schedule-set.ics"))
  @{:events @[@{:dtstamp 873118800
                :schedule @{873304200 873313200}
                :uid "consolidated-schedule-set-01@example.com"}
              @{:dtstamp 873118800
                :schedule @{873304200 873313200}
                :uid "consolidated-schedule-set-02@example.com"}
              @{:dtstamp 873118800
                :schedule @{873297000 873306000
                            873304200 873313200}
                :uid "consolidated-schedule-set-03@example.com"}
              @{:dtstamp 873118800
                :schedule @{873304200 873313200}
                :uid "consolidated-schedule-set-04@example.com"}
              @{:dtstamp 873118800
                :schedule @{873297000 873306000}
                :uid "consolidated-schedule-set-05@example.com"}
              @{:dtstamp 873118800
                :schedule @{873297000 873306000
                            873304200 873313200}
                :uid "consolidated-schedule-set-05@example.com"}
              @{:dtstamp 873118800
                :schedule @{}
                :uid "consolidated-schedule-set-06@example.com"}
              @{:dtstamp 873118800
                :schedule @{873304200 873313200
                            874240200 874249200
                            875723400 875732400
                            876760200 876769200
                            877482000 877492800
                            879243071 879252071
                            880993800 881002800
                            881080200 881089200
                            881253000 881262000
                            881339400 881348400
                            881425800 881434800
                            881512200 881521200
                            881598600 881607600
                            881685000 881694000
                            881771400 881780400
                            881857800 881866800
                            881944200 881953200
                            882030600 882039600
                            882117000 882126000
                            882289800 882298800
                            882376200 882385200
                            882462600 882471600
                            882549000 882558000
                            882635400 882644400
                            882808200 882817200
                            882894600 882903600
                            883240200 883249200
                            883326600 883335600
                            883413000 883422000
                            883499400 883508400}
                :uid "consolidated-schedule-set-20@example.com"}]
    :prodid "-//ABC Corporation//NONSGML My Product//EN"
    :version "2.0"})
