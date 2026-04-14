#include "Marche.h"
#include "Place.h"
#include <vector>
#include <iostream>


bool Marche::est_valide(const std::vector<Construction*>& voisins)const {
    for (Construction* voisin : voisins) {
        if (voisin){
            if (voisin->get_type_construction() == "Marche") {
            return false;
            }
        }
    }
    return true;
}
bool Marche::est_valide_variante(const std::vector<Construction*>& voisins, const int etage) const{
    for (Construction* voisin : voisins) {
        if (voisin != nullptr) {
            Place* p = dynamic_cast<Place*>(voisin);
            if (p != nullptr && p->getCouleur() == "Jaune") {
                return true;
            }
        }
    }
    return false;
}
std::string Marche::get_type_construction() const {
    return "Marche";
}

void Marche::afficher_construction() const {
    std::cout << "M ";
    return;
}
