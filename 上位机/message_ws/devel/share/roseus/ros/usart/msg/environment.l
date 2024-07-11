;; Auto-generated. Do not edit!


(when (boundp 'usart::environment)
  (if (not (find-package "USART"))
    (make-package "USART"))
  (shadow 'environment (find-package "USART")))
(unless (find-package "USART::ENVIRONMENT")
  (make-package "USART::ENVIRONMENT"))

(in-package "ROS")
;;//! \htmlinclude environment.msg.html


(defclass usart::environment
  :super ros::object
  :slots (_voltage_left_battery _voltage_right_battery _temperature ))

(defmethod usart::environment
  (:init
   (&key
    ((:voltage_left_battery __voltage_left_battery) 0.0)
    ((:voltage_right_battery __voltage_right_battery) 0.0)
    ((:temperature __temperature) 0.0)
    )
   (send-super :init)
   (setq _voltage_left_battery (float __voltage_left_battery))
   (setq _voltage_right_battery (float __voltage_right_battery))
   (setq _temperature (float __temperature))
   self)
  (:voltage_left_battery
   (&optional __voltage_left_battery)
   (if __voltage_left_battery (setq _voltage_left_battery __voltage_left_battery)) _voltage_left_battery)
  (:voltage_right_battery
   (&optional __voltage_right_battery)
   (if __voltage_right_battery (setq _voltage_right_battery __voltage_right_battery)) _voltage_right_battery)
  (:temperature
   (&optional __temperature)
   (if __temperature (setq _temperature __temperature)) _temperature)
  (:serialization-length
   ()
   (+
    ;; float64 _voltage_left_battery
    8
    ;; float64 _voltage_right_battery
    8
    ;; float64 _temperature
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _voltage_left_battery
       (sys::poke _voltage_left_battery (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _voltage_right_battery
       (sys::poke _voltage_right_battery (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _temperature
       (sys::poke _temperature (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _voltage_left_battery
     (setq _voltage_left_battery (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _voltage_right_battery
     (setq _voltage_right_battery (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _temperature
     (setq _temperature (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get usart::environment :md5sum-) "4be2f61eda002ec29f7656590853bb61")
(setf (get usart::environment :datatype-) "usart/environment")
(setf (get usart::environment :definition-)
      "float64 voltage_left_battery
float64 voltage_right_battery
float64 temperature
")



(provide :usart/environment "4be2f61eda002ec29f7656590853bb61")


