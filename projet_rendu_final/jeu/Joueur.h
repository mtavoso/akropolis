#pragma once
#include "../plateau/Cite.h"
#include "../plateau/Chantier.h"
#include "../exceptions/JoueurException.h"
#include "./Score.h"
#include <string>
#include <iostream>
#include<memory>
class Joueur {
private:
    std::unique_ptr<Cite> cite;
    Score score;
    int nb_pierres;
    std::string nom;

public:
    Joueur(int longueur, int largeur, const std::string& nom, int pierres = 0) :
        cite(std::make_unique<Cite>(longueur, largeur)), score(Score()), nb_pierres(pierres), nom(nom) {}
    void debiter_pierres(int nb) { nb_pierres = nb_pierres - nb; }
    Score& get_score() { return score; }
    virtual void placer_tuiles_temporairement(const Coordonnee& c1, const Coordonnee& c2,const Coordonnee& c3, Tuile* t);
    virtual void confirmer_emplacement(Chantier& chantier);
    virtual void choisir_tuile(int rang, Chantier& chantier);
    void changer_tuile();
    const std::string& getNom() { return nom; }
    virtual void jouer_tour(Chantier& chantier, const std::vector<bool>& variantes);
    Cite* get_cite() const { return cite.get(); }
	virtual ~Joueur() = default;
    Joueur(const Joueur&) = delete;
    void set_nb_pierres(int nb) { nb_pierres = nb; }
    int get_nb_pierres() { return nb_pierres; }
    virtual void sauvegarder(std::ostream& os) const;
    virtual void charger(std::istream& is);
};
