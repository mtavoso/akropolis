#pragma once
#include <iostream>
#include "../abstract/Construction.h"

class Place : public Construction {
    const std::string couleur;
    int nb_etoiles;
public:
    Place(int id, const std::string& couleur, int etoiles)
        : Construction(id), couleur(couleur), nb_etoiles(etoiles) {
    }
    std::string getCouleur() const { return couleur; }
    int getNbEtoiles() const { return nb_etoiles; }
    std::string get_type_construction() const override { return "Place"; }
    void afficher_construction() const override;
	~Place() = default;
};
