#pragma once
#include "Tuile.h"
#include <vector>

class Chantier {
private:
    int taille_max_chantier;               // taille max du chantier
    int taille_chantier;
    std::vector<Tuile*> tuile_usees;
    std::vector<TypeConstruction> element;
    std::vector<Tuile*> pioche;    // tuiles disponibles dans le chantier


public:
    explicit Chantier(int nb_joueurs); //constructeur
    int calculer_cout_piocher_tuile(int i) const;       // retourne le cot de la tuile i
    void afficher_chantier() const;       // affiche la rivire
    void maj_chantier(Tuile* tuile_a_supprimer);
    Tuile* get_tuile(int i) const { return pioche[i]; }
    int get_taille() const { return taille_chantier; }
    bool est_vide() const;
    const std::vector<Tuile*>& get_tuiles_disponibles() const { return pioche; }
    ~Chantier();
    Chantier(const Chantier&) = delete;
    Chantier& operator=(const Chantier&) = delete;
    void sauvegarder(std::ostream& os) const;
    void charger(std::istream& is);
};
