(import jip)
(use judge)
(use ../helpers)

(test (jip/table-from-ics (read-from-file "test/ics-files/rfc-exdate.ics")))
