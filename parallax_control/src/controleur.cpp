//--- LES FICHIERS EN-TETES ----
#include <csignal>
#include <math.h>
#include <ros/ros.h>
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


// LES PROTOTYPES DE FONCTIONS
void sigintHandler(int sig);
float quaternionToAngleEuler(Quaternion q_angle);


// LES FONCTIONS ET LES CALLBACKS
void avancer(float lin_vel) {

}

void stop() {

}

void pivoter(float ang_vel) {

}

void reculer(float lin_vel) { 

}

void drive(float lin_vel, float ang_vel) {

}

void mouvement_rectangulaire() {

}

void mouvement_circulaire() {
	
}

void mouvement_avancer_reculer() {
	
}

// MAIN DE L'APPLICATION
int main(int argc, char **argv) {
  signal(SIGINT, sigintHandler);
  init(argc, argv, "controleur", ros::init_options::NoSigintHandler);

  NodeHandle nh;



  
  Duration(1).sleep();
  Rate loop_rate(FREQ_CONTROL);

  while (ok()) {
	loop_rate.sleep();



    spinOnce();
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