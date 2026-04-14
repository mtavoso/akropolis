#include "Temple.h"
#include <vector>
#include <iostream>



bool Temple::est_valide(const std::vector<Construction*>& voisins)const {
    for (Construction* voisin : voisins) {
        if (voisin == nullptr){
            return false;
        }
    }
    return true;
}
bool Temple::est_valide_variante(const std::vector<Construction*>& voisins, const int etage) const{
    return (etage >= 1);
}
std::string Temple::get_type_construction() const {
    return "Temple";
}

void Temple::afficher_construction() const {
    std::cout << "T ";
    return;
}
