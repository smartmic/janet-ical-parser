(declare-project
 :name "janet-ical-parser"
 :description "Convert iCalendar (.ics) to Janet"
 :author "Martin Michel <martin@famic.de>")

(def libical-remote "https://github.com/smartmic/libical.git")
(def libical-branch "wip_smartmic_zoneinfo")

(defn libical-clone-replace []
  "Clone or replace libical repo"
  (do
    (os/execute ["rm" "-rf" "libical/"] :p)
    (git "clone" "--depth" "1" "--branch" libical-branch "--single-branch" libical-remote)))

(defn libical-build []
  "Build libical with external script"
  (os/execute ["sh" "install-libical.sh"] :p))

(def ldflags (case (os/which)
              :windows @["ical.lib"]
              :linux @[
                       "-L./jpm_tree/lib64"
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
                       "-I./jpm_tree/include"
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

(phony "install-deps" []
       (do
         (libical-clone-replace)
         (libical-build))
       nil)
