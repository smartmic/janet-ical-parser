(defn read-from-file [file-path]
  (let [f (file/open file-path :r)
        content  (file/read f :all)]
    (file/close f)
    content))
