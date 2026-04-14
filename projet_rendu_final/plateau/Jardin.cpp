#include "Jardin.h"
#include <vector>
#include <iostream>


bool Jardin::est_valide(const std::vector<Construction*>& voisins)const {
    return true;
}
std::string Jardin::get_type_construction() const {
    return "Jardin";
}

bool Jardin::est_valide_variante(const std::vector<Construction*>& voisins, const int etage) const{
    return false;
}

void Jardin::afficher_construction() const {
    std::cout << "J ";
    return;
}
