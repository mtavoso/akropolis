#include "Hexagone.h"
#include <iostream>
void Hexagone::afficher_hexagone() const
{
    if (est_vide) {
        std::cout << "   ";
    }
    else if (construction) {
        std::cout << couleur_par_tuile(construction->get_id_mere());
        std::cout << etage;
        construction->afficher_construction();
        std::cout << "\033[0m";
    }
    else {
        std::cout << "ERR";
    }
}