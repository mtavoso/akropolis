#pragma once
#include "Joueur.h"

class IaDiff {
public:
    virtual ~IaDiff() = default;
    virtual void jouer_tour(Joueur& j, Chantier& c, const std::vector<bool>& variantes) = 0;
};

class IAFacile : public IaDiff {
public:
    void jouer_tour(Joueur& j, Chantier& c, const std::vector<bool>& variantes) override;
};

class IAMoyen : public IaDiff {
public:
    void jouer_tour(Joueur& j, Chantier& c, const std::vector<bool>& variantes) override;
};

class IADifficile : public IaDiff {
public:
    void jouer_tour(Joueur& j, Chantier& c, const std::vector<bool>& variantes) override;
};
