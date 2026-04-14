#include "Chantier.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <array> 
#include "../reglesconstru.h"


Chantier::Chantier(int nb_joueurs): element(TUILES_PIOCHE) {
    int taille_de_pioche;
    switch (nb_joueurs) {
    case 2: taille_max_chantier = 4; taille_de_pioche = 37; break;
    case 3: taille_max_chantier = 5; taille_de_pioche = 49; break;
    case 4: taille_max_chantier = 6; taille_de_pioche = 61; break;
    default: throw std::runtime_error("Nombre de joueurs invalide");
    }

   
    std::random_device rd;
    std::mt19937 g(rd());

    //  On decoupe en blocs de 3
    std::vector<std::array<TypeConstruction, 3>> blocs;

       
    for (size_t i = 0; i + 2 < element.size(); i += 3) {
         std::array<TypeConstruction, 3> arr;
         arr[0] = element[i];
         arr[1] = element[i + 1];
         arr[2] = element[i + 2];
         blocs.push_back(arr);
        }

     //  On melange les blocs (pas les elements internes)
     std::shuffle(blocs.begin(), blocs.end(), g);

     //  On reconstruit le vecteur
     int index = 0;
     for (auto& bloc : blocs) {
         element[index++] = bloc[0];
         element[index++] = bloc[1];
         element[index++] = bloc[2];}


    // Creation de toutes les tuiles
    size_t j = 0;
    for (size_t i = 0; i < taille_de_pioche; i++) {
        if (j + 2 < element.size()) {
            pioche.push_back(new Tuile(i, element[j], element[j + 1], element[j + 2]));
            j += 3;
        }
    }

    // Le chantier commence avec le maximum possible
    taille_chantier = taille_max_chantier;

}

int Chantier::calculer_cout_piocher_tuile(int i) const {
    if (i < 0 || i >= taille_chantier) {
        throw std::out_of_range("Indice tuile invalide");
    }
    return i;
}

void Chantier::afficher_chantier() const {
    std::cout << "Rivière :" << std::endl;
    int limite = taille_chantier;
    for (int i = 0; i < limite; i++) {
        std::cout << "Tuile: " << i << " cout = " << i << std::endl;
        pioche[i]->afficher_tuile();
    }
}

void Chantier::maj_chantier(Tuile* tuile_a_supprimer) {
    auto it = find(pioche.begin(), pioche.end(), tuile_a_supprimer);
    if (it != pioche.end()) {
        pioche.erase(it);
        tuile_usees.push_back(tuile_a_supprimer);

        // Diminution normale du chantier
        if (taille_chantier > 2) {
            taille_chantier--;
        }
        else {
            // Si le chantier tombe à 1 tuile on le refill
            taille_chantier = taille_max_chantier;
        }
    }
}

bool Chantier::est_vide() const {
    return pioche.size() < 2;
}

Chantier::~Chantier() {
    for (auto t : pioche)
        delete t;
    for (auto t : tuile_usees)
        delete t;
}
//convertir string en TypeConstruction
static TypeConstruction strToEnum(std::string s, std::string col = "") {
    if (s == "carriere") return TypeConstruction::K;
    if (s == "Jardin") return TypeConstruction::J;
    if (s == "Temple") return TypeConstruction::T;
    if (s == "Marche") return TypeConstruction::M;
    if (s == "Caserne") return TypeConstruction::C;
    if (s == "Habitation") return TypeConstruction::H;
    if (s == "Place") {
        if (col == "bleu") return TypeConstruction::PB;
        if (col == "vert") return TypeConstruction::PV;
        if (col == "rouge") return TypeConstruction::PR;
        if (col == "jaune") return TypeConstruction::PJ;
        if (col == "violet") return TypeConstruction::PP;
    }
    return TypeConstruction::K;
}

void Chantier::sauvegarder(std::ostream& os) const {
    // On sauvegarde la taille actuelle
    os << taille_chantier << "\n";

    // On sauvegarde la pioche
    os << pioche.size() << "\n";
    for (Tuile* t : pioche) {
        os << t->get_id() << " "; 

        // On doit sauvegarder les 3 composants de la tuile
        for (int k = 0; k < 3; ++k) {
            Construction* c = t->get_constru(k);
            os << c->get_type_construction() << " ";
            if (c->get_type_construction() == "Place") {
                os << dynamic_cast<Place*>(c)->getCouleur() << " ";
            }
        }
        os << "\n";
    }
}

void Chantier::charger(std::istream& is) {
    // Nettoyage complet
    for (auto t : pioche) delete t;
    pioche.clear();

    is >> taille_chantier;

    int taille_pioche;
    is >> taille_pioche;

    for (int i = 0; i < taille_pioche; ++i) {
        int id;
        is >> id;

        std::vector<TypeConstruction> types;
        for (int k = 0; k < 3; ++k) {
            std::string type, col;
            is >> type;
            if (type == "Place") is >> col;
            types.push_back(strToEnum(type, col));
        }

        // Reconstruction de la Tuile
        Tuile* t = new Tuile(id, types[0], types[1], types[2]);
        pioche.push_back(t);
    }
}
