(defun factorial(x)
  (if (> x 1)
	(* x (factorial (- x 1)))
	1))

(defun tail-factorial(x)
  (defun fact2(x acc)
	(if (= x 0)
	  acc
	  (fact2 (- x 1) (* acc x))))
  (fact2 x 1))

(defun iter-factorial(x)
  (let ((acc 1))
	(dotimes (i x)
	  (setf acc (* acc (+ i 1))))
	  acc))
