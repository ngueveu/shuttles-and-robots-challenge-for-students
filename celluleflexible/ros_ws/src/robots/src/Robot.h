#ifndef ROBOT
#define ROBOT

#include <rclcpp/rclcpp.hpp>
#include <rclcpp/callback_group.hpp>

//Messages
#include <robots/msg/move_robot.hpp>
#include <robots/msg/faire_tache_msg.hpp>
#include <robots/msg/msg_num_robot.hpp>
#include <robots/msg/evacuer_msg.hpp>

#include <commande_locale/msg/robot_joints.hpp>
#include <commande_locale/msg/tache_finie_msg.hpp>
#include <commande_locale/msg/msg_erreur.hpp>
#include <commande_locale/msg/deplacer_piece_msg.hpp>

#include "shuttles/srv/shuttle_id.hpp"

#include <std_msgs/msg/int32.hpp>
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/byte.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>
#include <std_msgs/msg/int32_multi_array.hpp>

#include "sensor_msgs/msg/joint_state.hpp"


#include <vector>
#include <sstream>
#include <iostream>
#include <string>
#include <atomic>
#include <mutex>


#include "Poste.h"

// Nombre de cube max sur les postes et navettes
//#define NB_CUBE 6
#include "../../commande_locale/src/vrepController.h"


class Robot : public rclcpp::Node, public std::enable_shared_from_this<Robot>
{
private:

  /** Topic V-Rep **/
  // Le type Byte est utilisé lorsque le contenu du message n'est pas utile


  // GetObjectHandle
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pubSim_getObjectHandle;
  std_msgs::msg::String msgSim_getObjectHandle;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subSim_getObjectHandle;
  std::atomic<bool> repSim_getObjectHandle;
  std::atomic<int> valueSim_getObjectHandle;


  // SetJointState
  rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pubSim_setJointState;
  std_msgs::msg::Float32MultiArray msgSim_setJointState;
  rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr subSim_setJointState;
  std::atomic<bool> repSim_setJointState;


  // GetJointState
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pubSim_getJointState;
  std_msgs::msg::Int32 msgSim_getJointState;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr subSim_getJointState;
  std::atomic<bool> repSim_getJointState;
  std::atomic<float> valueSim_getJointState;


  // GetTime
  rclcpp::Publisher<std_msgs::msg::Byte>::SharedPtr pubSim_getTime;
  std_msgs::msg::Byte msgSim_getTime;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr subSim_getTime;
  std::atomic<bool> repSim_getTime;
  std::atomic<float> valueSim_getTime;


  // GetTimeUpdate
  rclcpp::Publisher<std_msgs::msg::Byte>::SharedPtr pubSim_getTimeUpdate;
  std_msgs::msg::Byte msgSim_getTimeUpdate;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr subSim_getTimeUpdate;
  std::atomic<bool> repSim_getTimeUpdate;
  std::atomic<float> valueSim_getTimeUpdate;


  // ChangeColor
  rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr pubSim_changeColor;
  std_msgs::msg::Int32MultiArray msgSim_changeColor;
  rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr subSim_changeColor;
  std::atomic<bool> repSim_changeColor;

  
  // ChangeShuttleColor
  rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr pubSim_changeShuttleColor;
  std_msgs::msg::Int32MultiArray msgSim_changeShuttleColor;
  rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr subSim_changeShuttleColor;
  std::atomic<bool> repSim_changeShuttleColor;


  // GetColor
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pubSim_getColor;
  std_msgs::msg::String msgSim_getColor;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subSim_getColor;
  std::atomic<bool> repSim_getColor{false};
  std::atomic<int> valueSim_getColor{0};

  
  // GetColorUpdate
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pubSim_getColorUpdate;
  std_msgs::msg::String msgSim_getColorUpdate;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subSim_getColorUpdate;
  std::atomic<bool> repSim_getColorUpdate;
  std::atomic<int> valueSim_getColorUpdate;


  /** Subscribers aux topics de la commande **/
  rclcpp::Subscription<robots::msg::MsgNumRobot>::SharedPtr planifSendPosition;
  rclcpp::Subscription<commande_locale::msg::RobotJoints>::SharedPtr planifSendJoints;
  rclcpp::Subscription<robots::msg::MsgNumRobot>::SharedPtr planifFermerPince;
  rclcpp::Subscription<robots::msg::MsgNumRobot>::SharedPtr planifOuvrirPince;
  rclcpp::Subscription<robots::msg::MsgNumRobot>::SharedPtr planifDescendreBras;
  rclcpp::Subscription<robots::msg::MsgNumRobot>::SharedPtr planifMonterBras;
  rclcpp::Subscription<robots::msg::MoveRobot>::SharedPtr planifControlerRobot;
  rclcpp::Subscription<robots::msg::FaireTacheMsg>::SharedPtr sub_faireTache;
  //rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr sub_evacuer;
  rclcpp::Subscription<robots::msg::EvacuerMsg>::SharedPtr sub_evacuer;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subStopTache;
  rclcpp::Subscription<commande_locale::msg::DeplacerPieceMsg>::SharedPtr subDeplacerPiece;
  rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr sub_pause;
  rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr sub_play;
  
  

  /** Publishers pour retours **/
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_pince;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_robotPosition;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_robotBras;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_robotPince;
  rclcpp::Publisher<robots::msg::MsgNumRobot>::SharedPtr pub_retourCommande;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pub_robot_transport;
  rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr pub_produitEvac;
  rclcpp::Publisher<commande_locale::msg::MsgErreur>::SharedPtr pub_erreur_log;
  rclcpp::Publisher<commande_locale::msg::TacheFinieMsg>::SharedPtr pub_tache_finie;

  rclcpp::Client<shuttles::srv::ShuttleId>::SharedPtr client;

  shuttles::srv::ShuttleId srv;
  commande_locale::msg::TacheFinieMsg msg_tache_finie;
  std::mutex color_mutex_;

  /** Variables **/
  float pi;
  unsigned char mymodes[7];
  float Rpos[7];
  int Rints[7];
  int couleur_transportee[NB_CUBE];
  int num_robot;
  bool is_paused_ = false;
  double time_pause_started_ = 0.0;

  rclcpp::Rate* loop_rate;
  rclcpp::Rate* loop_ok;

  /** Messages **/
  std_msgs::msg::Int32 robotPosition;
  std_msgs::msg::Int32 robotBras;
  std_msgs::msg::Int32 robotPince;
  robots::msg::MsgNumRobot retour;
  commande_locale::msg::MsgErreur msg_erreur;

  std_msgs::msg::Int32MultiArray msg_log_couleur;

  int produit_sur_poste;
  Poste poste_pos_1;
  Poste poste_pos_4;


public:

  explicit Robot(int num_du_robot);
  ~Robot();
  
  rclcpp::CallbackGroup::SharedPtr callback_group_;

  //Initialisation
  void init();

  int computeTableId(int position);
  void transport(bool valeur);
  void update();



  /** Fonctions internes permettant le contrôle du robot **/
	//Pour atteindre une position prédéfinie
  void EnvoyerRobot(int position);

  //Pour atteindre une position définie manuellement
  void EnvoyerJoints(int j1,int j2,int j3,int j4,int j5,int j6,int j7);

  //Pour monter ou descendre le bras
  void DescendreBras();
  void MonterBras();

  //Pour ouvrir ou fermer la pince
  void FermerPince();
  void OuvrirPince();

  

  /** Fonctions permettant de controler le robot avec des ordres du noeud commande **/
	//Pour atteindre une position prédéfinie
  void SendPositionCallback(const robots::msg::MsgNumRobot::SharedPtr msg);

  //Pour atteindre une position définie manuellement
  void SendJointsCallback(const commande_locale::msg::RobotJoints::SharedPtr msg);

  //Pour monter ou descendre le bras
  void DescendreBrasCallback(const robots::msg::MsgNumRobot::SharedPtr msg);
  void MonterBrasCallback(const robots::msg::MsgNumRobot::SharedPtr msg);

  //Pour ouvrir ou fermer la pince
  void FermerPinceCallback(const robots::msg::MsgNumRobot::SharedPtr msg);
  void OuvrirPinceCallback(const robots::msg::MsgNumRobot::SharedPtr msg);

  //Pour contrôler l'ensemble des mouvements du robot
  void ControlerRobotCallback(const robots::msg::MoveRobot::SharedPtr msg);
  void Colorer(int position, int type);
  void faireTacheCallback(const robots::msg::FaireTacheMsg::SharedPtr msg);
  void ajouter_produitCallback(commande_locale::msg::MsgAddProduct msg);

  int colorerPosteDebutTask(int positionPoste);
	int colorerPosteFinTask(int positionPoste, int duree);
  //void Evacuer(const std_msgs::msg::Byte::SharedPtr msg);
  void Evacuer(const robots::msg::EvacuerMsg::SharedPtr msg);

  void stopTacheCallback(const std_msgs::msg::Int32::SharedPtr msg);
  void DeplacerPieceCallback(const commande_locale::msg::DeplacerPieceMsg::SharedPtr msg);

  // Callbacks pour V-Rep
  void simGetObjectHandleCallback(const std_msgs::msg::Int32::SharedPtr msg);
  void simSetJointStateCallback(const std_msgs::msg::Byte::SharedPtr msg);
  void simGetJointStateCallback(const sensor_msgs::msg::JointState::SharedPtr msg);
  void simGetTimeCallback(const std_msgs::msg::Float32::SharedPtr msg);
  void simGetTimeUpdateCallback(const std_msgs::msg::Float32::SharedPtr msg);
  void simChangeColorCallback(const std_msgs::msg::Byte::SharedPtr msg);
  void simChangeShuttleColorCallback(const std_msgs::msg::Byte::SharedPtr msg);
  void simGetColorCallback(const std_msgs::msg::Int32::SharedPtr msg);
  void simGetColorUpdateCallback(const std_msgs::msg::Int32::SharedPtr msg);
  void pauseCallback(const std_msgs::msg::Byte::SharedPtr msg);
  void playCallback(const std_msgs::msg::Byte::SharedPtr msg);
};

#endif
