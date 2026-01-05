# SPDX-FileCopyrightText: 2026 Martin Michel <martin@famic.de>
#
# SPDX-License-Identifier: MPL-2.0

(import jip)
(use judge)

(test (jip/table-from-ics-file "test/ics-files/rfc-rrule.ics")
  @{:events @[@{:dtend 873208800
                :dtstamp 873118800
                :dtstart 873205200
                :duration 3600
                :rdates @[{:end 873208800 :start 873205200}
                          {:end 873295200 :start 873291600}
                          {:end 873381600 :start 873378000}
                          {:end 873468000 :start 873464400}
                          {:end 873554400 :start 873550800}
                          {:end 873640800 :start 873637200}
                          {:end 873727200 :start 873723600}
                          {:end 873813600 :start 873810000}
                          {:end 873900000 :start 873896400}
                          {:end 873986400 :start 873982800}]
                :uid "19970901T130000Z-daily-for-10-occurrences@example.com"}
              @{:dtend 873208800
                :dtstamp 873118800
                :dtstart 873205200
                :duration 3600
                :rdates @[{:end 873208800 :start 873205200}
                          {:end 873295200 :start 873291600}
                          {:end 873381600 :start 873378000}
                          {:end 873468000 :start 873464400}
                          {:end 873554400 :start 873550800}
                          {:end 873640800 :start 873637200}
                          {:end 873727200 :start 873723600}
                          {:end 873813600 :start 873810000}
                          {:end 873900000 :start 873896400}
                          {:end 873986400 :start 873982800}
                          {:end 874072800 :start 874069200}
                          {:end 874159200 :start 874155600}
                          {:end 874245600 :start 874242000}
                          {:end 874332000 :start 874328400}
                          {:end 874418400 :start 874414800}
                          {:end 874504800 :start 874501200}
                          {:end 874591200 :start 874587600}
                          {:end 874677600 :start 874674000}
                          {:end 874764000 :start 874760400}
                          {:end 874850400 :start 874846800}
                          {:end 874936800 :start 874933200}
                          {:end 875023200 :start 875019600}
                          {:end 875109600 :start 875106000}
                          {:end 875196000 :start 875192400}
                          {:end 875282400 :start 875278800}
                          {:end 875368800 :start 875365200}
                          {:end 875455200 :start 875451600}
                          {:end 875541600 :start 875538000}
                          {:end 875628000 :start 875624400}
                          {:end 875714400 :start 875710800}
                          {:end 875800800 :start 875797200}
                          {:end 875887200 :start 875883600}
                          {:end 875973600 :start 875970000}
                          {:end 876060000 :start 876056400}
                          {:end 876146400 :start 876142800}
                          {:end 876232800 :start 876229200}
                          {:end 876319200 :start 876315600}
                          {:end 876405600 :start 876402000}
                          {:end 876492000 :start 876488400}
                          {:end 876578400 :start 876574800}
                          {:end 876664800 :start 876661200}
                          {:end 876751200 :start 876747600}
                          {:end 876837600 :start 876834000}
                          {:end 876924000 :start 876920400}
                          {:end 877010400 :start 877006800}
                          {:end 877096800 :start 877093200}
                          {:end 877183200 :start 877179600}
                          {:end 877269600 :start 877266000}
                          {:end 877356000 :start 877352400}
                          {:end 877442400 :start 877438800}
                          {:end 877528800 :start 877525200}
                          {:end 877615200 :start 877611600}
                          {:end 877701600 :start 877698000}
                          {:end 877788000 :start 877784400}
                          {:end 877878000 :start 877874400}
                          {:end 877964400 :start 877960800}
                          {:end 878050800 :start 878047200}
                          {:end 878137200 :start 878133600}
                          {:end 878223600 :start 878220000}
                          {:end 878310000 :start 878306400}
                          {:end 878396400 :start 878392800}
                          {:end 878482800 :start 878479200}
                          {:end 878569200 :start 878565600}
                          {:end 878655600 :start 878652000}
                          {:end 878742000 :start 878738400}
                          {:end 878828400 :start 878824800}
                          {:end 878914800 :start 878911200}
                          {:end 879001200 :start 878997600}
                          {:end 879087600 :start 879084000}
                          {:end 879174000 :start 879170400}
                          {:end 879260400 :start 879256800}
                          {:end 879346800 :start 879343200}
                          {:end 879433200 :start 879429600}
                          {:end 879519600 :start 879516000}
                          {:end 879606000 :start 879602400}
                          {:end 879692400 :start 879688800}
                          {:end 879778800 :start 879775200}
                          {:end 879865200 :start 879861600}
                          {:end 879951600 :start 879948000}
                          {:end 880038000 :start 880034400}
                          {:end 880124400 :start 880120800}
                          {:end 880210800 :start 880207200}
                          {:end 880297200 :start 880293600}
                          {:end 880383600 :start 880380000}
                          {:end 880470000 :start 880466400}
                          {:end 880556400 :start 880552800}
                          {:end 880642800 :start 880639200}
                          {:end 880729200 :start 880725600}
                          {:end 880815600 :start 880812000}
                          {:end 880902000 :start 880898400}
                          {:end 880988400 :start 880984800}
                          {:end 881074800 :start 881071200}
                          {:end 881161200 :start 881157600}
                          {:end 881247600 :start 881244000}
                          {:end 881334000 :start 881330400}
                          {:end 881420400 :start 881416800}
                          {:end 881506800 :start 881503200}
                          {:end 881593200 :start 881589600}
                          {:end 881679600 :start 881676000}
                          {:end 881766000 :start 881762400}
                          {:end 881852400 :start 881848800}
                          {:end 881938800 :start 881935200}
                          {:end 882025200 :start 882021600}
                          {:end 882111600 :start 882108000}
                          {:end 882198000 :start 882194400}
                          {:end 882284400 :start 882280800}
                          {:end 882370800 :start 882367200}
                          {:end 882457200 :start 882453600}
                          {:end 882543600 :start 882540000}
                          {:end 882630000 :start 882626400}
                          {:end 882716400 :start 882712800}
                          {:end 882802800 :start 882799200}
                          {:end 882889200 :start 882885600}]
                :uid "19970901T130000Z-daily-until-december-24-1997@example.com"}
              @{:dtend 873208800
                :dtstamp 873118800
                :dtstart 873205200
                :duration 3600
                :rdates @[{:end 873208800 :start 873205200}
                          {:end 874072800 :start 874069200}
                          {:end 874936800 :start 874933200}
                          {:end 875800800 :start 875797200}
                          {:end 876664800 :start 876661200}]
                :uid "19970901T130000Z-every-10-days-5-occurrences@example.com"}
              @{:dtend 883666800
                :dtstamp 873118800
                :dtstart 883663200
                :duration 3600
                :rdates @[{:end 883666800 :start 883663200}
                          {:end 883753200 :start 883749600}
                          {:end 883839600 :start 883836000}
                          {:end 883926000 :start 883922400}
                          {:end 884012400 :start 884008800}
                          {:end 884098800 :start 884095200}
                          {:end 884185200 :start 884181600}
                          {:end 884271600 :start 884268000}
                          {:end 884358000 :start 884354400}
                          {:end 884444400 :start 884440800}
                          {:end 884530800 :start 884527200}
                          {:end 884617200 :start 884613600}
                          {:end 884703600 :start 884700000}
                          {:end 884790000 :start 884786400}
                          {:end 884876400 :start 884872800}
                          {:end 884962800 :start 884959200}
                          {:end 885049200 :start 885045600}
                          {:end 885135600 :start 885132000}
                          {:end 885222000 :start 885218400}
                          {:end 885308400 :start 885304800}
                          {:end 885394800 :start 885391200}
                          {:end 885481200 :start 885477600}
                          {:end 885567600 :start 885564000}
                          {:end 885654000 :start 885650400}
                          {:end 885740400 :start 885736800}
                          {:end 885826800 :start 885823200}
                          {:end 885913200 :start 885909600}
                          {:end 885999600 :start 885996000}
                          {:end 886086000 :start 886082400}
                          {:end 886172400 :start 886168800}
                          {:end 886258800 :start 886255200}]
                :uid "19970901T130000Z-every-day-in-january-for-3-years@example.com"}
              @{:dtend 873208800
                :dtstamp 873118800
                :dtstart 873205200
                :duration 3600
                :rdates @[{:end 873208800 :start 873205200}
                          {:end 873813600 :start 873810000}
                          {:end 874418400 :start 874414800}
                          {:end 875023200 :start 875019600}
                          {:end 875628000 :start 875624400}
                          {:end 876232800 :start 876229200}
                          {:end 876837600 :start 876834000}
                          {:end 877442400 :start 877438800}
                          {:end 878050800 :start 878047200}
                          {:end 878655600 :start 878652000}]
                :uid "19970901T130000Z-weekly-for-10-occurrences@example.com"}
              @{:dtend 873208800
                :dtstamp 873118800
                :dtstart 873205200
                :duration 3600
                :rdates @[{:end 873208800 :start 873205200}
                          {:end 873813600 :start 873810000}
                          {:end 874418400 :start 874414800}
                          {:end 875023200 :start 875019600}
                          {:end 875628000 :start 875624400}
                          {:end 876232800 :start 876229200}
                          {:end 876837600 :start 876834000}
                          {:end 877442400 :start 877438800}
                          {:end 878050800 :start 878047200}
                          {:end 878655600 :start 878652000}
                          {:end 879260400 :start 879256800}
                          {:end 879865200 :start 879861600}
                          {:end 880470000 :start 880466400}
                          {:end 881074800 :start 881071200}
                          {:end 881679600 :start 881676000}
                          {:end 882284400 :start 882280800}
                          {:end 882889200 :start 882885600}]
                :uid "19970901T130000Z-weekly-until-december-24-1997@example.com"}]
    :prodid "-//ABC Corporation//NONSGML My Product//EN"
    :version "2.0"})
