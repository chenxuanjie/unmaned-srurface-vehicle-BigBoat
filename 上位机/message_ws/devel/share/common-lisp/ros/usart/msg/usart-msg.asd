
(cl:in-package :asdf)

(defsystem "usart-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "environment" :depends-on ("_package_environment"))
    (:file "_package_environment" :depends-on ("_package"))
  ))