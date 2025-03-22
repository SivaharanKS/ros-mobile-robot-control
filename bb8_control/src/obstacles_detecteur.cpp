// LES FICHIERS EN-TETE
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <string>

// LES NAMESPACES
using namespace ros;
using namespace std_msgs;

// LES FONCTIONS

// MAIN
int main(int argc, char **argv) {
  init(argc, argv, "obstacles_detecteur");
  NodeHandle nh;

  // subscribers ICI

  //

  spin();

  return 0;
}
