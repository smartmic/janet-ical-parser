# SPDX-FileCopyrightText: 2026 Martin Michel <martin@famic.de>
#
# SPDX-License-Identifier: MPL-2.0

(declare-project
  :name "janet-ical-parser"
  :description "Convert iCalendar (.ics) to Janet"
  :author "Martin Michel <martin@famic.de>"
  :dependencies ["judge"])

(def libical-remote "https://github.com/smartmic/libical.git")
(def libical-branch "wip_smartmic_zoneinfo")

(defn- nmake-build-install-libical
  []
  (let [cwd (os/cwd)]
    (os/execute ["cmake"
                 "-G" "NMake Makefiles"
                 "-S" "libical\\"
                 "-B" "libical\\build\\"
                 "-DCMAKE_INSTALL_PREFIX=.\\jpm_tree\\"
                 "-DICAL_GLIB=FALSE"
                 "-DUSE_BUILTIN_TZDATA=TRUE"
                 "-DICAL_BUILD_DOCS=FALSE"
                 "-DLIBICAL_BUILD_TESTING=FALSE"
                 "-DWITH_CXX_BINDINGS=FALSE"] :px)
    (os/cd "libical\\build")
    (os/execute ["nmake"] :px)
    (os/execute ["nmake" "install"] :px)
    (os/cd cwd)))

(defn- execute
  [& args]
  (try
    (let [p (os/spawn args :p {:out :pipe :err :pipe})
          r (os/proc-wait p)]
      {:status r
       :out (:read (p :out) :all)
       :err (:read (p :err) :all)})
    ([e] {:status false
          :error e})))

(case (os/which)
  :windows
  (do
    (rule "install-ext-deps" []
          (do
            (if (= "False\r\n" (string ((execute "powershell" "-Command" "Test-Path" "-Path" ".\\libical\\") :out)))
              (git "clone" "--depth" "1" "--branch" libical-branch "--single-branch" libical-remote))
            (nmake-build-install-libical)))
    (if (= "False\r\n" (string ((execute "powershell" "-Command" "Test-Path" "-Path" ".\\jpm_tree\\bin\\libical.dll") :out)))
      (do-rule "install-ext-deps")))
  :linux
  (try
    (assert (= 0 (os/execute ["pkg-config" "libical" "--max-version=3.0.21"] :p)))
    ([err fib]
      (print "Error: missing libical development version, please install lastest 3.0 version (<= 3.0.21)")
      (os/exit 1))))

(def ldflags (case (os/which)
               :windows @["ical.lib"]
               :linux @["-lical"]
               #default
               @["-lical"]))

(def cflags (case (os/which)
              :windows @["/Ijpm_tree\\include"
                         "/Isrc"]
              :linux @["-Wno-discarded-qualifiers"
                       "-I./src"]
              #default
              nil))

(declare-native
  :name "jip/native"
  :headers ["src/jip.h"]
  :source ["src/jip.c"
           "src/utils.c"]
  :cflags cflags
  :ldflags ldflags)


(declare-source
  :source ["jip"])
