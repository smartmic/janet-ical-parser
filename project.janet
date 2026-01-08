# SPDX-FileCopyrightText: 2026 Martin Michel <martin@famic.de>
#
# SPDX-License-Identifier: MPL-2.0

(declare-project
 :name "janet-ical-parser"
 :description "Convert iCalendar (.ics) to Janet"
 :author "Martin Michel <martin@famic.de>")

(declare-source
 :source ["jip"])

(try
  (assert (= 0 (os/execute ["pkg-config" "libical" "--max-version=3.0.21"] :p)))
  ([err fib]
   (print "Error: missing libical development version, please install lastest 3.0 version (<= 3.0.21)")
   (os/exit 1)))

(def ldflags (case (os/which)
              :windows @["ical.lib"]
              :linux @[
                       "-lical"
                       ]
              #default
              @["-lical"]))

(def cflags (case (os/which)
              :windows @[
                         "/Ijpm_tree\\include"
                         "/Isrc"
                        ]
              :linux @[
                       "-Wno-discarded-qualifiers"
                       "-I./src"
                      ]
              #default
              nil))


(declare-source
 :source ["jip"])

(declare-native
 :name "jip/native"
 :headers ["src/jip.h"]
 :source ["src/jip.c"
          "src/utils.c"]
 :cflags cflags
 :ldflags ldflags)
