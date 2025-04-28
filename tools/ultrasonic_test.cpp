#include <iostream>
#include <unistd.h>
#include "rpi3/Gpiod.h"

#define TRIGGER_PIN 23
#define ECHO_PIN 24



int main() {
    rpi3::Gpiod trigger("/dev/gpiochip0", TRIGGER_PIN);
    rpi3::Gpiod echo("/dev/gpiochip0", ECHO_PIN);

    trigger.setDirectionOutput();
    echo.setDirectionInput();

    while (true) {
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
            std::cout << " Echo n'a pas demarre." << std::endl;
            continue;
        }

        // Mesure de la durée
        unsigned int duration = 0;
        while (echo.getValue() == 1 && duration < 30000) {
            usleep(1);
            duration++;
        }

        if (duration >= 30000) {
            std::cout << " Echo trop long." << std::endl;
            continue;
        }

        std::cout << " Durée mesure : " << duration << " us" << std::endl;


        // Calcul de distance
        float distance = ((duration * 0.0343) / 2.0)*100;
        std::cout << " Distance mesure : " << distance << " cm" << std::endl;

        usleep(500000); 
    }

    return 0;
}