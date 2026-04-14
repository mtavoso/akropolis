#include "Tuile.h"
#include <iostream>
#include <string>
#include <algorithm>

Construction* Tuile::creerConstruction(TypeConstruction t)
{
    switch (t) {
    case TypeConstruction::K: return new Carriere(id_tuile);
    case TypeConstruction::J: return new Jardin(id_tuile);
    case TypeConstruction::T: return new Temple(id_tuile);
    case TypeConstruction::M: return new Marche(id_tuile);
    case TypeConstruction::C: return new Caserne(id_tuile);
    case TypeConstruction::H: return new Habitation(id_tuile);
    case TypeConstruction::PB: return new Place(id_tuile, "bleu", 1);
    case TypeConstruction::PV: return new Place(id_tuile, "vert", 3);
    case TypeConstruction::PR: return new Place(id_tuile, "rouge", 2);
    case TypeConstruction::PJ: return new Place(id_tuile, "jaune", 2);
    case TypeConstruction::PP: return new Place(id_tuile, "violet", 2);

    }
    return nullptr; 
}


void Tuile::afficher_tuile()
{
    std::cout << std::string(3, ' ') << std::string(8, '_') << std::endl;
    std::cout << "  /" << std::string(8, ' ') << "\\" << std::endl;
    std::cout << " /" << std::string(10, ' ') << "\\" << std::endl;
    std::cout << "/" << std::string(5, ' ');
    agencement[0]->afficher_construction();
    std::cout << std::string(5, ' ') << "\\" << std::string(8, '_') << std::endl;
    std::cout << "\\" << std::string(12, ' ') << "/" << std::string(8, ' ') << "\\" << std::endl;
    std::cout << " \\" << std::string(10, ' ') << "/" << std::string(10, ' ') << "\\" << std::endl;
    std::cout << "  \\" << std::string(8, '_') << "/" << std::string(5, ' ');
    agencement[2]->afficher_construction();
    std::cout << std::string(5, ' ') << "\\" << std::endl;
    std::cout << "  /" << std::string(8, ' ') << "\\" << std::string(12, ' ') << "/" << std::endl;
    std::cout << " /" << std::string(10, ' ') << "\\" << std::string(10, ' ') << "/" << std::endl;
    std::cout << "/" << std::string(5, ' ');
    agencement[1]->afficher_construction();
    std::cout << std::string(5, ' ') << "\\" << std::string(8, '_') << "/" << std::endl;
    std::cout << "\\" << std::string(12, ' ') << "/" << std::endl;
    std::cout << " \\" << std::string(10, ' ') << "/" << std::endl;
    std::cout << "  \\" << std::string(8, '_') << "/" << std::endl;
}

void Tuile::rotation_tuile(int nb_rota)
{
    for (int i = 0; i < nb_rota; ++i)
    {
        std::rotate(agencement.begin(), agencement.begin() + 1, agencement.end());
    }
}

bool Tuile::operator==(const Tuile& autre) const {
    return id_tuile == autre.id_tuile;
}
