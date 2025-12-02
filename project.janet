(declare-project
 :name "ical")

(declare-native
 :name "ical"
 :source ["ical.c"]
 :ldflags ["-lical"])
