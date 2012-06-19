(defun abs-value(x)
  (if (< x 0)
	(- 0 x)
	x))

(defun square-root(b)
  (defun sq2(b x tolerance)
	(let ((y x))
	  (setf y (* 0.5 (+ x (/ b x))))
	  (if (< (abs-value (- y x)) tolerance)
		y
		(sq2 b y tolerance))))
  (sq2 b b 0.000001))

(defun square-root-iterative(b)
  (let ((x b) (y b) (tolerance 0.000001))
	(loop
	  (setf y (* 0.5 (+ x (/ b x))))
	  (if (< (abs-value (- y x)) tolerance)
		(return y)
		(setf x y)))))

