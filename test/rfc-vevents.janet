(import jip)
(use judge)

(test (jip/schedule-set (jip/table-from-ics-file "test/ics-files/rfc-vevents.ics"))
  @{:events @[@{:dtstamp 865963425
                :schedule @{868899600 868939200}
                :summary "Bastille Day Party"
                :uid "19970610T172345Z-AF23B2@example.com"}
              @{:categories @["BUSINESS" "HUMAN RESOURCES"]
                :class "PRIVATE"
                :dtstamp 873118800
                :schedule @{873304200 873313200}
                :summary "Annual Employee Review"
                :uid "19970901T130000Z-123401@example.com"}
              @{:categories @["BUSINESS" "HUMAN RESOURCES"]
                :class "PUBLIC"
                :dtstamp 873118800
                :schedule @{859912200 859942800}
                :summary "Laurel is in sensitivity awareness class."
                :transp "TRANSPARENT"
                :uid "19970901T130000Z-123402@example.com"}
              @{:categories @["ANNIVERSARY"
                              "PERSONAL"
                              "SPECIAL OCCASION"]
                :class "CONFIDENTIAL"
                :dtstamp 873118800
                :schedule @{878428800 878515200
                            909964800 910051200}
                :summary "Our Blissful Anniversary"
                :transp "TRANSPARENT"
                :uid "19970901T130000Z-123403@example.com"}
              @{:dtstamp 1177331672
                :schedule @{1182988800 1183939200}
                :summary "Festival International de Jazz de Montreal"
                :transp "TRANSPARENT"
                :uid "20070423T123432Z-541111@example.com"}]
    :prodid "-//hacksw/handcal//NONSGML v1.0//EN"
    :version "2.0"})
