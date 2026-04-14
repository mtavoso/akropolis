#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "Joueur.h"
#include "../plateau/Chantier.h"
#include "../jeu/IllustreConstructeur.h"
#include "../jeu/Variante.h"
#include "Memento.h"
class Partie {
private:
    static Partie* instance;
    const int nb_joueurs;
    bool courante;
    std::vector<bool> variantes;
    std::vector<Joueur*> joueurs;
    Chantier* chantier;
    Partie(int j, bool c, const std::vector<bool>& var, const std::vector<std::string>& noms);
    Partie(bool c, const std::vector<bool>& var, const std::string& nom, const std::string& lvl);
    int indexJoueurCourant;

public:
    Partie(const Partie&) = delete;
    void operator=(const Partie&) = delete;
    ~Partie();
    static Partie& get_instance(int j, bool c, const std::vector<bool>& var, const std::vector<std::string>& noms);
    static Partie& get_instance(bool c, const std::vector<bool>& var, const std::string& nom, const std::string& lvl);
    static Partie& get_instance();
    bool setCourante();
    int getNbJoueurs() const;
    std::vector<Joueur*> determiner_classement();
    void jouer();
    const std::vector<bool>* get_variantes() const{ return &variantes;}
    std::vector<Joueur*>& get_joueurs() { return joueurs; }
    Chantier* get_chantier() const { return chantier; }
    Memento creerMemento();
    void retablirMemento(const Memento& m);
    int get_index_joueur_courant() const { return indexJoueurCourant; }
    void set_index_joueur_courant(int i) { indexJoueurCourant = i; }
};
