; Auto-generated. Do not edit!


(cl:in-package usart-msg)


;//! \htmlinclude environment_message.msg.html

(cl:defclass <environment_message> (roslisp-msg-protocol:ros-message)
  ((voltage_left_battery
    :reader voltage_left_battery
    :initarg :voltage_left_battery
    :type cl:float
    :initform 0.0)
   (voltage_right_battery
    :reader voltage_right_battery
    :initarg :voltage_right_battery
    :type cl:float
    :initform 0.0)
   (temperature
    :reader temperature
    :initarg :temperature
    :type cl:float
    :initform 0.0))
)

(cl:defclass environment_message (<environment_message>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <environment_message>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'environment_message)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name usart-msg:<environment_message> is deprecated: use usart-msg:environment_message instead.")))

(cl:ensure-generic-function 'voltage_left_battery-val :lambda-list '(m))
(cl:defmethod voltage_left_battery-val ((m <environment_message>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader usart-msg:voltage_left_battery-val is deprecated.  Use usart-msg:voltage_left_battery instead.")
  (voltage_left_battery m))

(cl:ensure-generic-function 'voltage_right_battery-val :lambda-list '(m))
(cl:defmethod voltage_right_battery-val ((m <environment_message>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader usart-msg:voltage_right_battery-val is deprecated.  Use usart-msg:voltage_right_battery instead.")
  (voltage_right_battery m))

(cl:ensure-generic-function 'temperature-val :lambda-list '(m))
(cl:defmethod temperature-val ((m <environment_message>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader usart-msg:temperature-val is deprecated.  Use usart-msg:temperature instead.")
  (temperature m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <environment_message>) ostream)
  "Serializes a message object of type '<environment_message>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'voltage_left_battery))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'voltage_right_battery))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'temperature))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <environment_message>) istream)
  "Deserializes a message object of type '<environment_message>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'voltage_left_battery) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'voltage_right_battery) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'temperature) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<environment_message>)))
  "Returns string type for a message object of type '<environment_message>"
  "usart/environment_message")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'environment_message)))
  "Returns string type for a message object of type 'environment_message"
  "usart/environment_message")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<environment_message>)))
  "Returns md5sum for a message object of type '<environment_message>"
  "4be2f61eda002ec29f7656590853bb61")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'environment_message)))
  "Returns md5sum for a message object of type 'environment_message"
  "4be2f61eda002ec29f7656590853bb61")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<environment_message>)))
  "Returns full string definition for message of type '<environment_message>"
  (cl:format cl:nil "float64 voltage_left_battery~%float64 voltage_right_battery~%float64 temperature~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'environment_message)))
  "Returns full string definition for message of type 'environment_message"
  (cl:format cl:nil "float64 voltage_left_battery~%float64 voltage_right_battery~%float64 temperature~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <environment_message>))
  (cl:+ 0
     8
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <environment_message>))
  "Converts a ROS message object to a list"
  (cl:list 'environment_message
    (cl:cons ':voltage_left_battery (voltage_left_battery msg))
    (cl:cons ':voltage_right_battery (voltage_right_battery msg))
    (cl:cons ':temperature (temperature msg))
))
