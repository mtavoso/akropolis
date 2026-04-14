#pragma once
#include "Joueur.h"
#include "IA.h"

class IllustreConstructeur : public Joueur {
    IaDiff* niveau;
public:
    IllustreConstructeur(int longueur, int largeur, const std::string& nom, const std::string& niveau, int pierres=0);
    void jouer_tour(Chantier& c, const std::vector<bool>& variantes) { niveau->jouer_tour(*this, c, variantes); }

    ~IllustreConstructeur() { delete niveau; }
};
