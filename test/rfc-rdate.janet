(import jip)
(use judge)
(use ../helpers)

(test (jip/table-from-ics (read-from-file "test/ics-files/rfc-rdate.ics"))
  @{:events @[@{:dtend 859912800
                :dtstamp 873118800
                :dtstart 859912200
                :duration 600
                :rdates @[{:end 868884000 :start 868883400}
                          {:end 871562400 :start 871561800}
                          {:end 868884000 :start 868883400}
                          {:end 828504000 :start 828496800}
                          {:end 828590400 :start 828579600}
                          {:end 828496800 :start 828489600}
                          {:end 828583200 :start 828572400}
                          {:end 852136800 :start 852136200}
                          {:end 853778400 :start 853777800}
                          {:end 856197600 :start 856197000}
                          {:end 861640800 :start 861640200}
                          {:end 864664800 :start 864664200}
                          {:end 868034400 :start 868033800}
                          {:end 873132000 :start 873131400}
                          {:end 876847200 :start 876846600}
                          {:end 880735200 :start 880734600}
                          {:end 880821600 :start 880821000}
                          {:end 883068000 :start 883067400}
                          {:end 1094429400 :start 1094428800}]
                :uid "19970901T130000Z-123402@example.com"}]
    :prodid "-//ABC Corporation//NONSGML My Product//EN"
    :version "2.0"})
