;; Auto-generated. Do not edit!


(when (boundp 'commande_locale::Msg_Erreur)
  (if (not (find-package "COMMANDE_LOCALE"))
    (make-package "COMMANDE_LOCALE"))
  (shadow 'Msg_Erreur (find-package "COMMANDE_LOCALE")))
(unless (find-package "COMMANDE_LOCALE::MSG_ERREUR")
  (make-package "COMMANDE_LOCALE::MSG_ERREUR"))

(in-package "ROS")
;;//! \htmlinclude Msg_Erreur.msg.html


(defclass commande_locale::Msg_Erreur
  :super ros::object
  :slots (_code _n_poste ))

(defmethod commande_locale::Msg_Erreur
  (:init
   (&key
    ((:code __code) 0)
    ((:n_poste __n_poste) 0)
    )
   (send-super :init)
   (setq _code (round __code))
   (setq _n_poste (round __n_poste))
   self)
  (:code
   (&optional __code)
   (if __code (setq _code __code)) _code)
  (:n_poste
   (&optional __n_poste)
   (if __n_poste (setq _n_poste __n_poste)) _n_poste)
  (:serialization-length
   ()
   (+
    ;; int32 _code
    4
    ;; int32 _n_poste
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _code
       (write-long _code s)
     ;; int32 _n_poste
       (write-long _n_poste s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _code
     (setq _code (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _n_poste
     (setq _n_poste (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get commande_locale::Msg_Erreur :md5sum-) "81853f895108ea47877f09e6d08519dc")
(setf (get commande_locale::Msg_Erreur :datatype-) "commande_locale/Msg_Erreur")
(setf (get commande_locale::Msg_Erreur :definition-)
      "int32 code
int32 n_poste

")



(provide :commande_locale/Msg_Erreur "81853f895108ea47877f09e6d08519dc")

