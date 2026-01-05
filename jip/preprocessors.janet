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
