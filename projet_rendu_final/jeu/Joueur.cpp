#include "Joueur.h"
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>

void Joueur::confirmer_emplacement(Chantier& chantier){
    for (size_t i = 0; i < cite->get_position_temporaire().size(); i++) {
        Coordonnee c = cite->get_position_temporaire()[i];
        Hexagone& hex = cite->recherche_hexagone(c);
        Construction* ancienne_constru = hex.get_constru();
        Construction* constru = cite->get_tuile_temporaire()->get_constru(i);
        hex.set_est_vide(false);

        if (ancienne_constru) {
            if (ancienne_constru->get_type_construction()=="carriere"){
                nb_pierres += 1;
			}else if (ancienne_constru->get_type_construction()=="Habitation"){
                Habitation* ancienne_hab = dynamic_cast<Habitation*>(ancienne_constru);
                ancienne_hab->supprimer(hex.get_etage());
            }
            hex.set_etage(hex.get_etage() + 1);
        }

        if (constru){
            if (constru->get_type_construction()=="Habitation"){
                Habitation* h = dynamic_cast<Habitation*>(constru);
                h->rejoindre_des_chaines(cite->get_voisins(c), hex.get_etage());

                hex.set_constru(h);
            }else{
                hex.set_constru(constru);
            }
        }else{
            hex.set_constru(nullptr); 
        }


	}
    chantier.maj_chantier(cite->get_tuile_temporaire());
	cite->maj_cite();

}
void Joueur::placer_tuiles_temporairement(const Coordonnee& c1, const Coordonnee& c2, const Coordonnee& c3, Tuile* t) {

    if (!cite->contient(c1) || !cite->contient(c2) || !cite->contient(c3))
        throw JoueurException("Coordonnée hors plateau.");
	if (!(trio_coordonnees_voisines(c1, c2, c3))){

        throw JoueurException("Ce ne sont pas 3 coordonnées voisines.");
    }
    else{
        //Vérifier si on ne pose pas dans le vide, si c'est dans la continuité du reste de la Cité :
        bool continuite = false;

        //  Vérification autour de c1 
        for (const Coordonnee& c : c1.get_voisines()) {
            if (!cite->contient(c)) continue; 
            if (!cite->recherche_hexagone(c).get_est_vide()) {
                continuite = true;
            }
        }

        //  Vérification autour de c2 
        for (const Coordonnee& c : c2.get_voisines()) {
            if (!cite->contient(c)) continue;   
            if (!cite->recherche_hexagone(c).get_est_vide()) {
                continuite = true;
            }
        }

        // Vérification autour de c3
        for (const Coordonnee& c : c3.get_voisines()) {
            if (!cite->contient(c)) continue; 
            if (!cite->recherche_hexagone(c).get_est_vide()) {
                continuite = true;
            }
        }

        if (!continuite){
            throw JoueurException("Impossible de poser ici, il faut que ce soit dans la continuité du reste de la Cité.");
        }
        else {
            Hexagone& h1 = cite->recherche_hexagone(c1);
            Hexagone& h2 = cite->recherche_hexagone(c2);
            Hexagone& h3 = cite->recherche_hexagone(c3);
            //Vérifier si tout les Hexagones sur lesquels on construit sont vides :
            if ((h1.get_est_vide()) && (h2.get_est_vide()) && (h3.get_est_vide())){
                std::vector<Coordonnee> temp = { c1, c2, c3 };
                cite->set_position_temporaire(temp);
            }
            else{
                //Vérification de l'uniformité de l'étage des Hexagones sur lesquelles on construit:
                bool etage_uniforme = (h1.get_etage() == h2.get_etage()) && (h2.get_etage() == h3.get_etage());

                // Vérifier qu'on construit à cheval entre deux tuiles :
                Construction* cons1 = h1.get_constru();
                Construction* cons2 = h2.get_constru();
                Construction* cons3 = h3.get_constru();

                bool a_cheval = false;

                if (cons1 && cons2 && cons3) {
                    a_cheval = (cons1->get_id_mere() != cons2->get_id_mere())
                        || (cons2->get_id_mere() != cons3->get_id_mere())
                        || (cons3->get_id_mere() != cons1->get_id_mere());
                }

                if (!(etage_uniforme)){
                    throw JoueurException("Impossible de construire sur plusieurs étages à la fois.");
                }else if (!(a_cheval)){
                    throw JoueurException("Impossible de superposer deux Tuiles.");
                }else{
                    std::vector<Coordonnee> temp = { c1, c2, c3 };
                    cite->set_position_temporaire(temp);
                }
            }
        }
    }
}

void Joueur::choisir_tuile(int rang, Chantier& chantier) {

	if (chantier.calculer_cout_piocher_tuile(rang) > nb_pierres)
		throw JoueurException("Cette tuile est trop chère.");
	cite->set_tuile_temporaire(chantier.get_tuile(rang));
}

void Joueur::changer_tuile() {
    // Annule la tuile sélectionnée
    cite->set_tuile_temporaire(nullptr);
    }


void Joueur::jouer_tour(Chantier& chantier, const std::vector<bool>& variantes) {
    std::string input;
    Cite* cite = get_cite();
    bool tour_termine = false;

    while (!tour_termine) {
        int index_tuile = -1;
        chantier.afficher_chantier();

        std::cout << "> Choisir tuile (0-" << chantier.get_taille() - 1 << ") ou 'save'/'quit' : ";
        std::cin >> input;

        if (input == "quit") throw JoueurException("QUIT");
        if (input == "save") throw JoueurException("SAVE");

        try {
            index_tuile = std::stoi(input);
            choisir_tuile(index_tuile, chantier);
        }
        catch (...) {
            std::cout << "Entrée invalide." << std::endl;
            continue;
        }

        cite->afficher_cite();
        bool placement_confirme = false;
        while (!placement_confirme) {
            std::vector<Coordonnee> coordonnes;
            bool saisie_valide_et_placee = false;

            while (!saisie_valide_et_placee) {
                coordonnes.clear();
                for (int i = 0; i < 3; i++) {
                    int x, y;
                    std::cout << "> Coordonnees (X Y) : ";
                    std::cin >> x >> y;
                    coordonnes.push_back(Coordonnee(x, y));
                }

                try {
                    placer_tuiles_temporairement(coordonnes[0], coordonnes[1], coordonnes[2],
                        cite->get_tuile_temporaire());
                    saisie_valide_et_placee = true;
                }
                catch (const JoueurException& e) {
                    std::cout << "/!\\ Erreur de placement : " << e.what() << std::endl;
                    std::cout << "Veuillez recommencer la saisie des coordonnées.\n";
                }
            }

            bool choix_action_termine = false;
            while (!choix_action_termine) {
                cite->afficher_cite();
                std::cout << "> Action ? (tourner / changer_coord / changer_tuile / confirmer) : ";
                std::cin >> input;

                if (input == "tourner" || input == "t") {
                    cite->get_tuile_temporaire()->rotation_tuile(1);
                    placer_tuiles_temporairement(coordonnes[0], coordonnes[1], coordonnes[2],
                        cite->get_tuile_temporaire());
                    std::cout << "Rotation effectuée." << std::endl;

                }
                else if (input == "changer_coord" || input == "c_c") {
                    choix_action_termine = true;

                }
                else if (input == "changer_tuile" || input == "c_t") {
                    changer_tuile();
                    chantier.afficher_chantier();
                    std::cout << "Indice tuile (0-" << chantier.get_taille() - 1 << ") : ";
                    std::string idxs;
                    std::cin >> idxs;
                    try {
                        int new_idx = std::stoi(idxs);
                        choisir_tuile(new_idx, chantier);
                    }
                    catch (...) {
                        std::cout << "Entrée invalide." << std::endl;
                    }

                }
                else if (input == "confirmer" || input == "c") {
                    confirmer_emplacement(chantier);
                    debiter_pierres(index_tuile);
                    get_score().maj_score(cite, variantes);
                    std::cout << "Coup valide !" << std::endl;
                    choix_action_termine = true;
                    placement_confirme = true;
                    tour_termine = true;
                }
                else {
                    std::cout << "Commande inconnue, réessayez." << std::endl;
                }
            }
        }
    }
}
void Joueur::sauvegarder(std::ostream& os) const {
    std::string nom_safe = nom;
    if (nom_safe.empty()) nom_safe = "Joueur_Inconnu";
    std::replace(nom_safe.begin(), nom_safe.end(), ' ', '_');

    os << nom_safe << "\n";
    os << nb_pierres << "\n";
    cite->sauvegarder(os);
}

void Joueur::charger(std::istream& is) {
    is >> nom;
    std::replace(nom.begin(), nom.end(), '_', ' ');

    is >> nb_pierres;
    if (is.fail()) {
        nb_pierres = 0;
    }

    cite->charger(is);
}
