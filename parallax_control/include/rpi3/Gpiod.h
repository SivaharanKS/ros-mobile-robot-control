#ifndef GPIOD_H2
#define GPIOD_H2

#include <gpiod.h> // Inclure la bibliothèque libgpiod
#include <string>
#include <stdexcept>

namespace rpi3{

class Gpiod {
public:
    // Constructeur et destructeur
  /*  Gpiod(const std::string& chipName, unsigned int lineOffset);
    ~Gpiod();

    // Méthodes pour manipuler les GPIO
    void setDirectionOutput();
    void setDirectionInput();
    void setValue(int value);
    int getValue();*/

    struct gpiod_chip* chip; // Pointeur vers le chip GPIO
    struct gpiod_line* line; // Pointeur vers la ligne GPIO
    unsigned int lineOffset; // Offset de la ligne GPIO


// Constructeur
Gpiod(const std::string& chipName, unsigned int lineOffset)
    : lineOffset(lineOffset) {
    // Ouvrir le chip GPIO
    chip = gpiod_chip_open(chipName.c_str());
    if (!chip) {
        throw std::runtime_error("Impossible d'ouvrir le chip GPIO: " + chipName);
    }

    // Obtenir la ligne GPIO
    line = gpiod_chip_get_line(chip, lineOffset);
    if (!line) {
        gpiod_chip_close(chip);
        throw std::runtime_error("Impossible d'obtenir la ligne GPIO: " + std::to_string(lineOffset));
    }
}

// Destructeur
~Gpiod() {
    if (line) {
        gpiod_line_release(line);
    }
    if (chip) {
        gpiod_chip_close(chip);
    }
}

// Configurer la ligne en sortie
void setDirectionOutput() {
    if (gpiod_line_request_output(line, "hc-sr04", 0) < 0) {
        throw std::runtime_error("Impossible de configurer la ligne en sortie.");
    }
}

// Configurer la ligne en entrée
void setDirectionInput() {
    //setup_gpio(line, 1, chip);
    if (gpiod_line_request_input(line, "hc_sr04") < 0) {
        throw std::runtime_error("Impossible de configurer la ligne en entrée.");
    }
}

// Définir la valeur de la ligne (pour les sorties)
void setValue(int value) {
    gpiod_line_set_value(line, value);
    if (gpiod_line_set_value(line, value) < 0) {
        throw std::runtime_error("Impossible de définir la valeur de la ligne.");
    }
}

// Lire la valeur de la ligne (pour les entrées)
int getValue() {
    int value = gpiod_line_get_value(line);
    if (value < 0) {
        throw std::runtime_error("Impossible de lire la valeur de la ligne.");
    }
    return value;
}

};

}

#endif // GPIOD_H2