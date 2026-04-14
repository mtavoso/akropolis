#include "Partie.h"
#include <iostream>
#include <limits>
#include <sstream>
#include "Caretaker.h"

Memento Partie::creerMemento() {
    std::stringstream ss;

    //  Données globales
    ss << nb_joueurs << "\n";
    ss << indexJoueurCourant << "\n"; 

    //  Variantes
    for (bool b : variantes) ss << b << " ";
    ss << "\n";

    // Chantier
    chantier->sauvegarder(ss);

    // Joueurs
    for (Joueur* j : joueurs) {
        // On sauvegarde le type pour savoir si c'est IA ou Humain au chargement
        if (dynamic_cast<IllustreConstructeur*>(j)) ss << "IA ";
        else ss << "HUMAIN ";

        j->sauvegarder(ss);
    }

    return Memento(ss.str());
}

void Partie::retablirMemento(const Memento& m) {
    std::stringstream ss(m.getEtat());

    int nbJ, idxCourant;
    ss >> nbJ >> idxCourant;
    this->indexJoueurCourant = idxCourant; // Restaurer l'index

    // Restaurer variantes
    for (size_t i = 0; i < variantes.size(); ++i) {
        int val; ss >> val; variantes[i] = (bool)val;
    }

    // Restaurer Chantier
    chantier->charger(ss);

    // Restaurer Joueurs
    for (Joueur* j : joueurs) delete j;
    joueurs.clear();

    for (int i = 0; i < nbJ; ++i) {
        std::string type;
        ss >> type;

        Joueur* j = nullptr;
        if (type == "IA") {
            j = new IllustreConstructeur(8, 8, "IA", "moyen", 0);
        }
        else {
            j = new Joueur(8, 8, "Tmp", 0);
        }

        j->charger(ss); 
        joueurs.push_back(j);
    }
    for (auto j : joueurs) {
        j->get_score().maj_score(j->get_cite(), variantes);
    }
}

Partie* Partie::instance = nullptr;

Partie& Partie::get_instance(int j, bool c, const std::vector<bool>& var, const std::vector<std::string>& noms) {
    if (instance == nullptr) instance = new Partie(j, c, var, noms);
    return *instance;
}

Partie& Partie::get_instance(bool c, const std::vector<bool>& var, const std::string& nom, const std::string& lvl) {
    if (instance == nullptr) instance = new Partie(c, var, nom, lvl);
    return *instance;
}


Partie& Partie::get_instance() { return *instance; }


Partie::Partie(int j, bool c, const std::vector<bool>& var, const std::vector<std::string>& noms): nb_joueurs(j), courante(c), indexJoueurCourant(0) {
    variantes = var;
    chantier = new Chantier(nb_joueurs);
    int taille_noms = noms.size();
    for (int i = 0; i < nb_joueurs; ++i) {
        std::string nom = (i < taille_noms) ? noms[i] : "Joueur " + std::to_string(i + 1);
        Joueur* nouveau = new Joueur(8, 8, nom, i+1);
        joueurs.push_back(nouveau);
    }
   
}

Partie::Partie(bool c, const std::vector<bool>& var, const std::string& nom, const std::string & lvl): nb_joueurs(2), courante(c), indexJoueurCourant(0) {
    
    variantes = var;
    chantier = new Chantier(nb_joueurs);    
    Joueur* nouveau = new Joueur(8, 8, nom, 1);
    IllustreConstructeur* ia = new IllustreConstructeur(8, 8, "IA", lvl, 2);
    joueurs.push_back(nouveau);
    joueurs.push_back(ia);
}
Partie::~Partie() {
    for (auto j : joueurs) delete j;
    delete chantier;
    instance = nullptr;
}

bool Partie::setCourante() { courante = true; return courante; }

int Partie::getNbJoueurs() const { return nb_joueurs; }

std::vector<Joueur*> Partie::determiner_classement() {
    for (auto j : joueurs) {
        j->get_score().maj_score(j->get_cite(), *(this->get_variantes()));
        j->get_score().ajouter_pierres(j->get_nb_pierres());
    }
    std::vector<Joueur*> cl = joueurs;
    std::sort(cl.begin(), cl.end(), [](Joueur* a, Joueur* b) { return a->get_score().get_valeur_score() > b->get_score().get_valeur_score(); });
    return cl;
}


void Partie::jouer() {

        // Boucle principale
        while (courante && !chantier->est_vide()) {
            for (; indexJoueurCourant < joueurs.size(); ++indexJoueurCourant) {
                Joueur* j = joueurs[indexJoueurCourant]; 
                if (!courante) break;

                std::cout << "\n-----------------------------------\n";
                std::cout << "TOUR DE : " << j->getNom() << " (Pierres: " << j->get_nb_pierres() << ")\n";
                std::cout << "-----------------------------------\n";

                j->get_score().afficher_score();
                j->get_cite()->afficher_cite();

                try {
                    j->jouer_tour(*chantier, variantes);  
                    j->get_score().maj_score(j->get_cite(), variantes);
                }
                catch (const JoueurException& e) {
                    std::string msg = e.what();

                    // SAUVEGARDE
                    if (msg == "SAVE") {
                        //  Sauvegarde
                        Caretaker caretaker;
                        Memento m = this->creerMemento();
                        caretaker.sauvegarderMemento(m, "sauvegarde.txt");

                        // Demande de continuation
                        std::cout << "[Succes] Partie sauvegardee." << std::endl;
                        std::cout << "Voulez-vous continuer a jouer ? (o/n) : ";
                        char rep;
                        std::cin >> rep;
                        if (rep == 'n' || rep == 'N') {
                            courante = false;
                            break;
                        }
                        // Si o, on ne fait rien, la boucle continue et le joueur reprend son tour
                    }
                    //  QUITTER
                    else if (msg == "QUIT") {
                        courante = false;
                        break;
                    }
                    else {
                        std::cout << "/!\\ Erreur : " << msg << std::endl;
                    }
                }
                j->get_score().afficher_score();
                j->get_cite()->afficher_cite();
                std::cout << "-----------------------------------\n";
            }
            indexJoueurCourant = 0;
        }

        std::cout << "=== PARTIE TERMINEE ===\n";
        determiner_classement(); // calcul du classement final
    }


