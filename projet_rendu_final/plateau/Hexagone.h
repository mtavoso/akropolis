#pragma once
#include "../abstract/Construction.h"
#include <vector>
#include <string>
#include <iostream>

class Hexagone {
private:
    Construction* construction;
    int etage;
    bool est_vide;

public:
    Hexagone() : construction(nullptr), etage(0), est_vide(true) {}

    void afficher_hexagone() const;  
    void set_constru(Construction* c) { construction = c; }
    Construction* get_constru() const { return construction; }
    int get_etage() const { return etage; }
    void set_etage(int e) { etage = e; }
    bool get_est_vide() const { return est_vide; }
    void set_est_vide(bool vide) { est_vide = vide; }
    ~Hexagone() = default;
};

inline std::string couleur_par_tuile(int id_tuile) {
    static const std::vector<std::string> couleurs = {
        "\033[1;31m", "\033[1;32m", "\033[1;33m", "\033[1;34m",
        "\033[1;35m", "\033[1;36m", "\033[1;37m"
    };
    if (id_tuile < 0) return "\033[0m"; // hexagone vide ou non defini
    return couleurs[id_tuile % couleurs.size()];
}
