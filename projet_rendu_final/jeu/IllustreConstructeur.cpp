#include "IllustreConstructeur.h"
#include "IA.h"

IllustreConstructeur::IllustreConstructeur(int longueur, int largeur, const std::string& nom, const std::string& niveau, int pierres)
    : Joueur(longueur, largeur, nom, pierres) {
    if (niveau == "facile") {
        this->niveau = new IAFacile();
    }
    else if (niveau == "moyen") {
        this->niveau = new IAMoyen();
    }
    else if (niveau == "difficile") {
        this->niveau = new IADifficile();
    }
    else {
        throw JoueurException("Niveau d'IA inconnu.");
    }
}
