#include "Cite.h"
#include <set>
#include <iostream>
#include <iomanip>
#include <algorithm>
Hexagone& Cite::recherche_hexagone(const Coordonnee& c) const {
    auto it = carte.find(c);
    if (it == carte.end()) {
        throw std::out_of_range("Cite::recherche_hexagone : coordonnee introuvable");
    }
    return *(it->second);
}

Cite::Cite(int longueur, int largeur): tuile_temporaire(nullptr), initiale(-1, TypeConstruction::PB,TypeConstruction::K, TypeConstruction::K, TypeConstruction::K)
{
    // Creation du plateau vide
    for (int x = 0; x <= longueur; x++) {
        for (int y = 0; y <= largeur; y++) {
            carte[{x, y}] = std::make_unique<Hexagone>();
        }
    }

    // Coordonnées du centre
    int cx = longueur / 2;
    int cy = largeur / 2;
    cy++;
    //Coordonnées des 4 hexagones
    std::vector<Coordonnee> pos = {
        {cx, cy},        
        {cx - 1, cy},  
        {cx + 1, cy},   
        {cx, cy-1}    
    };

    carte[pos[0]]->set_constru(initiale.get_constru(0));
    carte[pos[0]]->set_est_vide(false);

    for (int i = 1; i < 4; ++i) {
        carte[pos[i]]->set_constru(initiale.get_constru(i));
        carte[pos[i]]->set_est_vide(false);
    }

};


void Cite::maj_cite(){

	size_t i = 0;

    for (const Coordonnee& c : position_temporaire) {
        Construction* constru = tuile_temporaire->get_constru(i);
        carte.at(c)->set_constru(constru); //on maj l'hexagone
        i++;
    }

	set_tuile_temporaire(nullptr);
	position_temporaire.clear();

}
void Cite::set_tuile_temporaire(Tuile* t) {
    tuile_temporaire = t;

}
void Cite::set_position_temporaire(std::vector<Coordonnee>& c){

	position_temporaire = c;
}

std::vector<Construction*> Cite::get_voisins(const Coordonnee& c) const{
	std::vector<Construction*> constru_voisines;
	const auto& voisins = c.get_voisines();

	for (const Coordonnee& v : voisins) {
        if (contient(v))
		    constru_voisines.push_back(carte.at(v).get()->get_constru());

	}
	return constru_voisines;
}

void Cite::afficher_cite() const {

    int min_x = 0;
    int max_x = 8;
    int min_y = 0;
    int max_y = 8;

    const bool has_temp = (tuile_temporaire != nullptr);

    int hauteur = (max_y - min_y + 1) * 2 + 2;

    std::cout << "Y\\X|";
    for (int x = min_x; x <= max_x; x++)
        std::cout << std::setw(3) << x << "  ";
    std::cout << "\n";

    std::cout << "---|";
    for (int x = min_x; x <= max_x; x++)
        std::cout << "-----";
    std::cout << "\n";

 
    for (int l = 0; l < hauteur; l++) {

        // Affichage axe Y
        if (l % 2 == 0) {
            int display_y = min_y + l / 2;
            if (display_y <= max_y) std::cout << std::setw(3) << display_y << "|";
            else                    std::cout << "   |";
        }
        else {
            std::cout << "   |";
        }

        // Parcours colonnes X
        for (int x = min_x; x <= max_x; x++) {

            // Décalage des colonnes impaires
            int local_line = l;
            if (x % 2 != 0) local_line--;

            int y_grid = min_y + (local_line - 1) / 2;

            // PARTIE HAUTE
            
            if (local_line == 0) {
                std::cout << " ___ ";
                continue;
            }

            // HORS ZONE
            
            if (local_line < 0 || y_grid < min_y || y_grid > max_y) {
                std::cout << "     ";
                continue;
            }

            // CORPS DE L'HEXAGONE
            if (local_line % 2 != 0) {

                Coordonnee c(x, y_grid);

                // Tuile temporaire 
                if (has_temp) {
                    auto it = std::find(position_temporaire.begin(),
                        position_temporaire.end(), c);
                    if (it != position_temporaire.end()) {

                        size_t idx = std::distance(position_temporaire.begin(), it);
                        std::cout << "/";
                        tuile_temporaire->get_constru(idx)->afficher_construction();

                        std::cout << " \\";
                        continue;
                    }
                }

                // Hexagone normal
                if (contient(c)) {
                    std::cout << "/";
                    carte.at(c)->afficher_hexagone();
                    std::cout << "\\";
                }
                else {
                    std::cout << "/   \\";
                }

                continue;
            }

            
            // bes de l'hexagone
            std::cout << "\\___/";
        }

        std::cout << "\n";
    }
}

static Construction* fabriquerConstruction(std::string type, std::string couleur, int id) {
    if (type == "carriere") return new Carriere(id);
    if (type == "Jardin") return new Jardin(id);
    if (type == "Temple") return new Temple(id);
    if (type == "Marche") return new Marche(id);
    if (type == "Caserne") return new Caserne(id);
    if (type == "Habitation") return new Habitation(id);
    if (type == "Place") {
        if (couleur == "bleu") return new Place(id, "bleu", 1);
        if (couleur == "vert") return new Place(id, "vert", 3);
        if (couleur == "rouge") return new Place(id, "rouge", 2);
        if (couleur == "jaune") return new Place(id, "jaune", 2);
        if (couleur == "violet") return new Place(id, "violet", 2);
    }
    return nullptr;
}

void Cite::sauvegarder(std::ostream& os) const {
    // Sauvegarde des tuiles et les positions temporaires
    os << position_temporaire.size() << "\n";


    // Sauvegarde de la carte
    os << carte.size() << "\n";

    for (const auto& pair : carte) {
        // Coordonnées
        os << pair.first.getX() << " " << pair.first.getY() << " ";

        Hexagone* hex = pair.second.get();
        os << hex->get_etage() << " ";

        if (hex->get_constru() != nullptr) {
            os << "1 "; 
            Construction* c = hex->get_constru();
            os << c->get_type_construction() << " ";
            os << c->get_id_mere() << " ";
            if (c->get_type_construction() == "Place") {
                Place* p = dynamic_cast<Place*>(c);
                os << (p ? p->getCouleur() : "inconnue") << " ";
            }
 
            if (c->get_type_construction() == "Habitation") {
                Habitation* h = dynamic_cast<Habitation*>(c);
                os << (h ? h->get_points() : 0) << " ";
                os << (h ? h->get_id_chaine() : -1) << " ";
            }
        }
        else {
            os << "0 "; 
        }
        os << "\n";
    }
}

void Cite::charger(std::istream& is) {
    carte.clear();
    position_temporaire.clear();
    tuile_temporaire = nullptr;

    int nbPosTemp;
    is >> nbPosTemp; // On ignore les pos temp pour la sauvegarde

    int nbHexagones;
    is >> nbHexagones;

    for (int i = 0; i < nbHexagones; ++i) {
        int x, y, etage, a_constru, id_mere, id_chaine;
        int hab_points = 0, hab_id_chaine = -1;
        std::string type, couleur;

        is >> x >> y >> etage >> a_constru;

        auto hex = std::make_unique<Hexagone>();
        hex->set_etage(etage);

        if (a_constru == 1) {
            is >> type >> id_mere;

            if (type == "Place") is >> couleur;
            if (type == "Habitation") is >> hab_points >> hab_id_chaine;

            Construction* c = fabriquerConstruction(type, couleur, id_mere);
            if (c) {
                if (type == "Habitation") {
                    Habitation* h = dynamic_cast<Habitation*>(c);
                    h->set_points(hab_points);       // Restaure le score
                    h->set_id_chaine(hab_id_chaine); // Restaure le lien logique
                }
                hex->set_constru(c);
            }
            hex->set_est_vide(false);
        }
        else {
            hex->set_est_vide(true);
        }

        carte[{x, y}] = std::move(hex);
    }
}