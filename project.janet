# SPDX-FileCopyrightText: 2026 Martin Michel <martin@famic.de>
#
# SPDX-License-Identifier: MPL-2.0

(declare-project
 :name "janet-ical-parser"
 :description "Convert iCalendar (.ics) to Janet"
 :author "Martin Michel <martin@famic.de>")

(declare-source
 :source ["jip"])

(do
  (case (os/which)
    :windows
     (do
       (task "install-ext-deps" []
             (do
               (copy "libical\\share" "jpm_tree")
               (copy "libical\\libical.dll" "jpm_tree\\bin"))
             nil)
       (add-dep "install" "install-ext-deps"))
    :linux
     (try
       (assert (= 0 (os/execute ["pkg-config" "libical" "--max-version=3.0.21"] :p)))
       ([err fib]
        (print "Error: missing libical development version, please install lastest 3.0 version (<= 3.0.21)")
        (os/exit 1)))))


(def ldflags (case (os/which)
              :windows @["ical.lib"]
              :linux @[
                       "-lical"
                       ]
              #default
              @["-lical"]))

(def cflags (case (os/which)
              :windows @[
                         "/Ilibical\\include"
                         "/Isrc"
                        ]
              :linux @[
                       "-Wno-discarded-qualifiers"
                       "-I./src"
                      ]
              #default
              nil))

(declare-native
 :name "jip/native"
 :headers ["src/jip.h"]
 :source ["src/jip.c"
          "src/utils.c"]
 :cflags cflags
 :ldflags ldflags)
