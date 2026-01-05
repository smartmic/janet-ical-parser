# SPDX-FileCopyrightText: 2026 Martin Michel <martin@famic.de>
#
# SPDX-License-Identifier: MPL-2.0

(defn read-from-file [file-path]
  (let [f (file/open file-path :r)
        content  (file/read f :all)]
    (file/close f)
    (string content)))
