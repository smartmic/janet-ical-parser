(declare-project
 :name "janet-ical-parser"
 :description "Convert iCalendar (.ics) to Janet"
 :author "Martin Michel <martin@famic.de>")

(def ldflags (case (os/which)
              :windows @["ical.lib"]
              :linux @["-lical"]
              #default
              @["-lical"]))

(def cflags (case (os/which)
              :windows @["/Ijpm_tree\\include"
                         "/Isrc"]
              :linux @["-I./src"]
              #default
              nil))

(declare-native
 :name "jip"
 :headers ["src/jip.h"]
 :source ["src/jip.c"
          "src/utils.c"]
 :cflags cflags
 :ldflags ldflags)
