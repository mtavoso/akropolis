#include "Caserne.h"
#include <vector>
#include <iostream>


bool Caserne::est_valide(const std::vector<Construction*>& voisins) const {
    for (Construction* c : voisins) {
        if (c == nullptr) return true;
    }
    return false;
}

bool Caserne::est_valide_variante(const std::vector<Construction*>& voisins, const int etage) const {
    int libres = 0;
    for (Construction* c : voisins) {
        if (c == nullptr) libres++;
    }
    return (libres == 3)||(libres == 4);
}
std::string Caserne::get_type_construction() const {
    return "Caserne";
}

void Caserne::afficher_construction() const {
    std::cout << "C ";
    return ;
}
