#include <iostream>
#include <unistd.h>
#include "rpi3/Gpiod.h"
#include "ros/ros.h"
#include "std_msgs/Float64.h"

#define TRIGGER_PIN 23
#define ECHO_PIN 24

int main(int argc, char **argv) {
    // Initialisation ROS
    ros::init(argc, argv, "obstacle_detecteur");
    ros::NodeHandle nh;

    // Création du publisher
    ros::Publisher pub_distance = nh.advertise<std_msgs::Float64>("distance_obstacle", 10);

    rpi3::Gpiod trigger("/dev/gpiochip0", TRIGGER_PIN);
    rpi3::Gpiod echo("/dev/gpiochip0", ECHO_PIN);

    trigger.setDirectionOutput();
    echo.setDirectionInput();

    ros::Rate loop_rate(10); // 10 Hz

    while (ros::ok()) {
        // Envoi du trigger
        trigger.setValue(0);
        usleep(2);
        trigger.setValue(1);
        usleep(10);
        trigger.setValue(0);

        // Attente du signal echo à 1 (début mesure)
        unsigned int wait = 0;
        while (echo.getValue() == 0 && wait < 30000) {
            usleep(1);
            wait++;
        }
        if (wait >= 30000) {
            std::cout << "Echo n'a pas démarré." << std::endl;
            continue;
        }

        // Mesure de la durée
        unsigned int duration = 0;
        while (echo.getValue() == 1 && duration < 30000) {
            usleep(1);
            duration++;
        }

        if (duration >= 30000) {
            std::cout << "Echo trop long." << std::endl;
            continue;
        }

        // Calcul de distance
        float distance = ((duration * 0.0343) / 2.0) * 100;
        std::cout << "Distance mesurée : " << distance << " cm" << std::endl;

        // Publier la distance sur ROS
        std_msgs::Float64 msg_distance;
        msg_distance.data = distance;
        pub_distance.publish(msg_distance);

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}