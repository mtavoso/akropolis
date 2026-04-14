#pragma once
#include "Hexagone.h"
#include "Tuile.h"
#include "Coordonnee.h"
#include <map>
#include <memory>

class Cite {
private:
    std::map<Coordonnee, std::unique_ptr<Hexagone>> carte;
    Tuile* tuile_temporaire;
    Tuile initiale;
    std::vector<Coordonnee> position_temporaire; 
 
public:
    Cite(int longueur, int largeur);
    void afficher_cite() const;
    Hexagone& recherche_hexagone(const Coordonnee& c) const;
    void maj_cite();
    void set_tuile_temporaire(Tuile* t); //Tuile non const car la rotation modifie l'objet
    Tuile* get_tuile_temporaire() const { return tuile_temporaire; }

    const std::vector<Coordonnee>& get_position_temporaire() const { return position_temporaire; }
    void set_position_temporaire(std::vector<Coordonnee>& t);
    std::vector<Construction*> get_voisins(const Coordonnee& c) const;
    const std::map<Coordonnee, std::unique_ptr<Hexagone>>& get_carte() const { return carte; }
    bool contient(const Coordonnee& c) const {return carte.find(c) != carte.end();}
	~Cite() = default;
    void sauvegarder(std::ostream& os) const;
    void charger(std::istream& is);

};
