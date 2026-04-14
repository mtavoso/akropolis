#include "Place.h"
#include <iostream>
void Place::afficher_construction() const {
    if (couleur == "bleu") {
        std::cout << "eH";
    }
    else if (couleur == "vert") {
        std::cout << "eJ";
    }
    else if (couleur == "jaune") {
        std::cout << "eM";
    }
    else if (couleur == "rouge") {
        std::cout << "eC";
    }
    else if (couleur == "violet") {
        std::cout << "eT";
    }
}