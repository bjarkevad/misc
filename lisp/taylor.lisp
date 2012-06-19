(defun factorial(x)
  (let ((acc 1))
	(dotimes (i x)
	  (setf acc (* acc (+ i 1))))
	  acc))

(defun pow(x n)
  (let ((acc 1))
	(dotimes (i n)
	  (setf acc (* acc x)))
	acc))

(defun taylor-exp(x)
  (let ((a 1) (b x) (n 1) (tolerance 0.00001))
	(loop
	  (setf a (+ a (/ (pow x n) (factorial n))))
	  (setf n (+ n 1))
	  (if (< (abs (- b a)) tolerance)
		(return (* a 1.0))
		(setf b a)))))