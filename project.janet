(declare-project
 :name "ical")

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
 :source ["ical.c"]
 :cflags cflags
 :ldflags ldflags)
