//--- LES FICHIERS EN-TETES ----
#include "ros/duration.h"
#include "ros/publisher.h"
#include <csignal>
#include <geometry_msgs/Twist.h>
#include <math.h>
#include <nav_msgs/Odometry.h>
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/String.h>
#include <string>
#include <tf/tf.h>
#include <tf2/LinearMath/Quaternion.h>

// LES NAMESPACES
using namespace ros;
using namespace std_msgs;
using namespace geometry_msgs;

// LES CONSTANTES
#define MAX_UINT 65535
#define MODE_CIRCULAIRE 1
#define MODE_RECTANGULAIRE 2
#define LINEAR_SPEED 0.6
#define ANGULAR_SPEED 0.6
#define FREQ_CONTROL 10 // hz

// LES VARIABLES GLOBALES
int mode;
double pos_x = 0.0, pos_y = 0.0, ang = 0.0;
Publisher pub_cmd_av;
geometry_msgs::Twist cmd_av;
double start_x = 0.0, start_y = 0.0; // Position initiale
bool is_initial = false; // Vérifie si la position initiale a été enregistrée
bool moving_forward = true; // Direction du robot
bool has_moved = false;
bool stopped = false;
bool direction_changed = false;
bool obstacle_detected = false;
double distance_obstacle = 100.0;

// LES PROTOTYPES DE FONCTIONS
void sigintHandler(int sig);
float quaternionToAngleEuler(Quaternion q_angle);

// LES FONCTIONS ET LES CALLBACKS
void avancer(float lin_vel) {
  cmd_av.linear.x = lin_vel;
  cmd_av.linear.y = 0.0;
  
  cmd_av.linear.z = 0.0;
}
void stop() {
  cmd_av.linear.x = 0.0;
  cmd_av.linear.y = 0.0;
  cmd_av.linear.z = 0.0;
  cmd_av.angular.x = 0.0;
  cmd_av.angular.y = 0.0;
  cmd_av.angular.z = 0.0;
  pub_cmd_av.publish(cmd_av);
}

void pivoter(float ang_vel) { 
  cmd_av.angular.z = ang_vel;
  cmd_av.linear.x = 0.0;
  }

void reculer(float lin_vel) { cmd_av.linear.x = -lin_vel; }

void drive(float lin_vel, float ang_vel) {
  avancer(lin_vel);
  pivoter(ang_vel);
}

void mouvement_rectangulaire() {
  avancer(1.0);
  pub_cmd_av.publish(cmd_av);
  ros::Duration(2).sleep();
  stop();
  pub_cmd_av.publish(cmd_av);
  ros::Duration(2).sleep();
  pivoter(0.5);
  pub_cmd_av.publish(cmd_av);
  ros::Duration(1).sleep();
  stop();
  pub_cmd_av.publish(cmd_av);
  ros::Duration(2).sleep();
  avancer(1.0);
  pub_cmd_av.publish(cmd_av);
  ros::Duration(1).sleep();
  stop();
  pub_cmd_av.publish(cmd_av);
  ros::Duration(2).sleep();
  pivoter(0.5);
  pub_cmd_av.publish(cmd_av);
  ros::Duration(1).sleep();
  stop();
  pub_cmd_av.publish(cmd_av);
  ros::Duration(2).sleep();
}


void mouvement_circulaire() {
  float linear_speed = 1.0;  // Vitesse linéaire
  float angular_speed = 1.0; // Vitesse de rotation

  if (!moving_forward) {
    linear_speed = -linear_speed; // Reculer si nécessaire
    angular_speed = -angular_speed;
  }

  // Publier la commande de mouvement
  drive(linear_speed, angular_speed);
  pub_cmd_av.publish(cmd_av);
}

void mouvement_avancer_reculer() {}

void odomCallback(const nav_msgs::Odometry::ConstPtr &msg) {
  pos_x = msg->pose.pose.position.x;
  pos_y = msg->pose.pose.position.y;

  Quaternion q_angle;
  q_angle.x = msg->pose.pose.orientation.x;
  q_angle.y = msg->pose.pose.orientation.y;
  q_angle.z = msg->pose.pose.orientation.z;
  q_angle.w = msg->pose.pose.orientation.w;
  ang = quaternionToAngleEuler(q_angle);

  // Enregistrer la position initiale une seule fois
  if (!is_initial) {
    start_x = pos_x;
    start_y = pos_y;
    is_initial = true;
  }

  // Vérifier si le robot s’est éloigné suffisamment pour activer la détection
  double dist_to_start =
      sqrt(pow(pos_x - start_x, 2) + pow(pos_y - start_y, 2));

  if (dist_to_start > 0.5) {
    has_moved = true; // Dès qu'il s'éloigne, on active la détection
  }

  // Si le robot est revenu proche de sa position de départ et qu'il a déjà
  // bougé
  if (has_moved && dist_to_start < 0.3) {
    if (!stopped) { // Vérifier si l’arrêt n’a pas encore été déclenché
      stop();
      ROS_INFO("Robot revenu à la position initiale, arrêt.");
      ros::Duration(2).sleep(); // Pause avant de repartir

      // **Inverser la direction** et s’assurer que l’inversion est bien prise
      // en compte
      moving_forward = !moving_forward;
      stopped = true;
      direction_changed = true;
    }
  } else {
    stopped = false;
  }

  ROS_INFO(
      "Position : [X: %f, Y: %f] | Distance à l'origine : %f | Direction : %s",
      pos_x, pos_y, dist_to_start, moving_forward ? "AVANT" : "ARRIÈRE");
}

void distanceCallBack(const std_msgs::Float64::ConstPtr &msg){
  distance_obstacle = msg->data;
  
  if (distance_obstacle <= 20.0){
    obstacle_detected = true;
    stop();
  } else {
    obstacle_detected = false;
    avancer(1.0);
    pub_cmd_av.publish(cmd_av);
  }
  
  ROS_INFO("Distance obstacle reçue : %f cm",distance_obstacle);
  ROS_INFO("Distance obstacle bool : %d ",obstacle_detected);
}
void mouvement_rectangulaire_with_detector(){
   if(obstacle_detected){
      //pivoter(0.5);
      //pub_cmd_av.publish(cmd_av);
      stop();
      pub_cmd_av.publish(cmd_av);
    } else {      
      mouvement_rectangulaire();
      pub_cmd_av.publish(cmd_av);
    }
}

void naviguer_labyrinthe(){
  if(obstacle_detected){
    pivoter(0.5);
    pub_cmd_av.publish(cmd_av);
    stop();
  } else {      
    mouvement_rectangulaire();
    pub_cmd_av.publish(cmd_av);
  }
}


// MAIN DE L'APPLICATION
int main(int argc, char **argv) {
  signal(SIGINT, sigintHandler);
  init(argc, argv, "controleur", ros::init_options::NoSigintHandler);  

  NodeHandle nh;

  // Publisher pour envoyer des commandes de vitesse
  pub_cmd_av = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
  
  //Subscriber
  Subscriber sub_distance = nh.subscribe("distance_obstacle",10,distanceCallBack);

  // Subscriber pour récupérer les données d'odométrie
  Subscriber sub_odom = nh.subscribe("/odom", 10, odomCallback);

  Duration(1).sleep();
  Rate loop_rate(FREQ_CONTROL);

  while (ok()) {    
    //mouvement_rectangulaire();
    //mouvement_rectangulaire_with_detector();
    naviguer_labyrinthe(); 
    spinOnce(); // Met à jour les callbacks (odométrie)
    loop_rate.sleep();
  }

  return 0;
}

void sigintHandler(int sig) {
  // Log quit
  ROS_INFO("Exiting program gracefully ....");
  stop();
  // Kill all open subscriptions, publications, service calls, and service
  // servers
  shutdown();
}

// FONCTION DE CONVERSION QUATERNION => ANGLE D'EULER YAW (ROTATION Z)
float quaternionToAngleEuler(Quaternion angle_quaternion) {

  float angle_Euler = tf::getYaw(angle_quaternion);

  return angle_Euler;
}