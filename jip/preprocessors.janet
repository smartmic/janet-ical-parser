# SPDX-FileCopyrightText: 2026 Martin Michel <martin@famic.de>
#
# SPDX-License-Identifier: MPL-2.0

(defn replace-missing-description
  "Replace emtpy DESCRIPTION or SUMMARY properties with dummy value"
  [text]
  (string/join
   (peg/match
    ~(accumulate
      (any
       (choice
        (replace (sequence (capture (choice "DESCRIPTION:" "SUMMARY:")) (any "\r") "\n")
                 ,|(string $ "<jip-replacer-token>\n"))
        (capture 1))))
    text)))

(defn remove-method
  "Remove METHOD property from input ics iCalendar"
  [text]
  (string/join
   (peg/match
    ~(accumulate
      (any
       (choice
        (replace (sequence "METHOD:" (any :a) (any "\r") "\n")
                 nil)
        (capture 1))))
    text)))
