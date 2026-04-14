#pragma once
#include "../abstract/Quartier.h"
#include <vector>

class Caserne : public Quartier {
public:
    using Quartier::Quartier;
    std::string get_type_construction() const override;
    void afficher_construction() const override;
    bool est_valide(const std::vector<Construction*>& voisins) const override;
    bool est_valide_variante(const std::vector<Construction*>& voisins, const int etage) const override;
    ~Caserne() = default;
};
