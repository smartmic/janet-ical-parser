(declare-project
 :name "ical"
 :description "Convert iCalendar (.ics) to Janet"
 :author "Martin Michel <martin@famic.de>")

(def ldflags (case (os/which)
              :windows @["ical.lib"]
              :linux @["-lical"]
              #default
              @["-lical"]))

(def cflags (case (os/which)
              :windows @["/Ijpm_tree\\include"]
              :linux nil
              #default
              nil))

(declare-native
 :name "ical"
 :source ["src/ical.c"]
 :cflags cflags
 :ldflags ldflags)
