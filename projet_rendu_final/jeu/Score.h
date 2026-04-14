#pragma once
#include "../plateau/Cite.h"
#include "../jeu/Variante.h"
#include <array>

class Score {
private:
    int valeur_score;

    // Indexés par Variante
    std::array<int, NB_VARIANTES> points_quartiers{};
    std::array<int, NB_VARIANTES> etoiles_places{};

    void set_valeur_score(int val) { valeur_score = val; }

public:
    Score() : valeur_score(0) {
        reset_score();
    }

    //  Points quartiers

    void reset_points_quartiers() {
        points_quartiers.fill(0);
    }

    void incrementer_points_quartiers(Variante v, int etage) {
        points_quartiers[idx(v)] += 1 + etage;
    }

    const std::array<int, NB_VARIANTES>& get_points_quartiers() const {
        return points_quartiers;
    }

    //  Étoiles places

    void reset_etoiles_places() {
        etoiles_places.fill(0);
    }

    void incrementer_etoiles_places(Variante v, int nb_etoiles) {
        etoiles_places[idx(v)] += nb_etoiles;
    }

    const std::array<int, NB_VARIANTES>& get_etoiles_places() const {
        return etoiles_places;
    }

    // Score global

    void reset_score() {
        valeur_score = 0;
        reset_points_quartiers();
        reset_etoiles_places();
    }

    void maj_score(const Cite* cite, const std::vector<bool>& variantes);

    int get_valeur_score() const { return valeur_score; }

    void ajouter_pierres(int nb) { valeur_score += nb; }

    void afficher_score() const;
};
