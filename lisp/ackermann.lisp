(defun ackermann(m n)
  (if (= m 0)
	(+ n 1)
	(if (> m 0)
	  (if (= n 0)
		(ackermann (- m 1) 1)
		(ackermann (- m 1) (ackermann m (- n 1)))))))

