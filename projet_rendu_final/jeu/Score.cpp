#include "Score.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

// Renvoie -1 si la coordonée n'est pas dans le vecteur, sinon renvoi son index
int appartient(const std::vector<Coordonnee>& vec, Coordonnee element) {
    auto it = std::find(vec.begin(), vec.end(), element);
    if (it != vec.end()) {
        return std::distance(vec.begin(), it);
    }
    return -1;
}

void Score::maj_score(const Cite* cite, const std::vector<bool>& variantes) {

    this->reset_score();

    for (auto& tuple : cite->get_carte()) {

        const Coordonnee& coord = tuple.first;
        Hexagone& hex = cite->recherche_hexagone(coord);
        Construction* constr = hex.get_constru();
        int etage = hex.get_etage();

        // On regarde dans la tuile temporaire au cas ou y serait.
        int index_dans_tuile_temporaire = appartient(cite->get_position_temporaire(), coord);
        if (index_dans_tuile_temporaire != -1) {
            if (constr != nullptr) {
                etage++;
            }
            // Défense : vérifier que la tuile temporaire existe avant de la déréférencer
            Tuile* ttemp = cite->get_tuile_temporaire();
            if (ttemp) {
                constr = ttemp->get_constru(index_dans_tuile_temporaire);
            }

        }


        if (constr) {
            Variante v;
            string_to_variante(constr->get_type_construction(), v);
            Quartier* quart = dynamic_cast<Quartier*>(constr);
            if (quart) {

                // Cas spécial pour Habitation
                if (v == Variante::Habitation) {
                    Habitation* h = dynamic_cast<Habitation*>(quart);

                    if (h->get_points() >= points_quartiers[idx(v)]) {
                        points_quartiers[idx(v)] = h->get_points();

                        if (h->est_valide_variante(cite->get_voisins(coord), etage)
                            && variantes[idx(v)]) {
                            points_quartiers[idx(v)] = 2 * h->get_points();
                        }
                    }
                }
                // Autres cas :
                else {
                    if (quart->est_valide(cite->get_voisins(coord))) {
                        incrementer_points_quartiers(v, etage);

                        if (quart->est_valide_variante(cite->get_voisins(coord), etage)
                            && variantes[idx(v)]) {
                            incrementer_points_quartiers(v, etage);
                        }
                    }
                }
            }

            else {
                Place* place = dynamic_cast<Place*>(constr);
                if (place) {
                    string_to_variante(place->getCouleur(), v);
                    incrementer_etoiles_places(v, place->getNbEtoiles());
                }
            }
        }
    }

    for (size_t i = 0; i < NB_VARIANTES; i++) {
        valeur_score += points_quartiers[i] * etoiles_places[i]; //Calcul du score final
    }
}


void Score::afficher_score() const {
    const int colw = 9;
    const std::string cols(VARIANTE_ABREVIATIONS);

    auto print_border = [&]() {
        std::cout << "+";
        for (int i = 0; i < 5; ++i) std::cout << std::string(colw, '-') << "+";
        std::cout << "\n";
    };

    print_border();

    // Header line
    std::cout << "|";
    for (char c : cols) {
        std::string s(1, c);
        int pad = (colw - (int)s.size()) / 2;
        std::cout << std::string(pad, ' ') << s << std::string(colw - pad - (int)s.size(), ' ') << "|";
    }
    std::cout << "\n";

    print_border();

    // Points (points_quartiers)
    std::cout << "|";
    for (int i = 0; i < 5; ++i) {
        std::string s = "P:" + std::to_string(points_quartiers[i]);
        if ((int)s.size() > colw) s = s.substr(0, colw);
        std::cout << std::setw(colw) << std::right << s << "|";
    }
    std::cout << "\n";

    // Etoiles (etoiles_places)
    std::cout << "|";
    for (int i = 0; i < 5; ++i) {
        std::string s = "E:" + std::to_string(etoiles_places[i]);
        if ((int)s.size() > colw) s = s.substr(0, colw);
        std::cout << std::setw(colw) << std::right << s << "|";
    }
    std::cout << "\n";

    print_border();

    std::cout << "Score total: " << valeur_score << "\n";
}
