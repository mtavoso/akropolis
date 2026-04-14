#include "Habitation.h"
#include <vector>
#include<iostream>

// Pour éviter les cycles :

void Habitation::set_suivante(Habitation* h){
    if (suivante == h) return;

    if (suivante != nullptr)
        suivante->precedente = nullptr;

    suivante = h;

    if (h != nullptr)
        h->precedente = this;
}

void Habitation::set_precedente(Habitation* h){
    if (precedente == h) return;

    if (precedente != nullptr)
        precedente->suivante = nullptr;

    precedente = h;

    if (h != nullptr)
        h->suivante = this;
}

void afficher_habitation(Habitation* h) {
    std::cout << "Habitation ID Chaine: " << h->get_id_chaine() << ", Points: " << h->get_points() << "->" << std::endl;
}

int Habitation::compteur_id_chaine = 0;

bool Habitation::est_valide_variante(const std::vector<Construction*>& voisins, const int etage) const {
    return (points >= 10);
}
std::string Habitation::get_type_construction() const {
    return "Habitation";
}

void Habitation::afficher_construction() const {
    std::cout << "H ";
    return;
}

bool Habitation::est_valide(const std::vector<Construction*>& voisins) const {
        // Pour que Habitation ne devienne pas une classe abstraite
        return true;
    }


void Habitation::update_id_chaine_gauche(int id){
    set_id_chaine(id);
    if(precedente != nullptr){
        precedente->update_id_chaine_gauche(id);
    }
}

void Habitation::update_id_chaine_droite(int id){
    set_id_chaine(id);
    if(suivante != nullptr){
        suivante->update_id_chaine_droite(id);
    }
}

void Habitation::update_id_chaine(int id) {
        set_id_chaine(id);

        if(precedente != nullptr){
            precedente->update_id_chaine_gauche(id);
        }

        if(suivante != nullptr){
            suivante->update_id_chaine_droite(id);
        }
    }

void Habitation::modifier_points_gauche(int valeur){
    set_points(valeur);
    if (precedente != nullptr) {
        precedente->modifier_points_gauche(valeur);
    }
}

void Habitation::modifier_points_droite(int valeur){
    set_points(valeur);
    if (suivante != nullptr) {
        suivante->modifier_points_droite(valeur);
    }
}

void Habitation::modifier_points(int valeur){
    int nouvelle_valeur = points + valeur;
    set_points(nouvelle_valeur);

    if (suivante != nullptr) {
        suivante->modifier_points_droite(nouvelle_valeur);
    }
    if (precedente != nullptr) {
        precedente->modifier_points_gauche(nouvelle_valeur);
    }
}

Habitation* Habitation::trouver_bout_chaine() {
        if (suivante != nullptr) {
            return suivante->trouver_bout_chaine();
        }else{
            return this;
        }
}

Habitation* Habitation::trouver_debut_chaine() {
        if (precedente != nullptr) {
            return precedente->trouver_debut_chaine();
        }else{
            return this;
        }
}

void Habitation::rejoindre_des_chaines(const std::vector<Construction*>& voisins, int etage) {
        this->set_points(etage+1);
        for (Construction* voisin : voisins) {
            if (voisin) {
                if (voisin->get_type_construction() == "Habitation") {
                    Habitation* h_voisin = dynamic_cast<Habitation*>(voisin);
                    if (h_voisin) {
                        if (h_voisin->get_id_chaine() == id_chaine) {
                            continue; // Déjà dans la même chaîne
                        }
                        else {
                            modifier_points(h_voisin->get_points());
                            h_voisin->modifier_points(etage+1);

                            //Fusion en évitant les cycles 

                            Habitation* bout1 = trouver_bout_chaine();
                            Habitation* debut2 = h_voisin->trouver_debut_chaine();

                            // Rupture des cycles en bout de chaîne 
                            if (bout1->suivante) {
                                bout1->suivante->precedente = nullptr;
                                bout1->suivante = nullptr;
                            }
                            if (debut2->precedente) {
                                debut2->precedente->suivante = nullptr;
                                debut2->precedente = nullptr;
                            }

                            // fusion
                            bout1->set_suivante(debut2);

                            //Fin fusion

                            update_id_chaine(h_voisin->get_id_chaine());
                        }
                    }
                }
            }
        }
    }


void Habitation::supprimer(int etage){
    // Nouvelle valeur de points pour la chaîne restante
    int nouvelle_valeur = points - (etage + 1);
    set_points(nouvelle_valeur);

    // Propagation à gauche
    if (precedente != nullptr) {
        precedente->modifier_points_gauche(nouvelle_valeur);
    }

    // Propagation à droite
    if (suivante != nullptr) {
        suivante->modifier_points_droite(nouvelle_valeur);
    }

    // Suppression de cet élément de la chaîne
    if (precedente != nullptr) {
        precedente->suivante = suivante;
    }
    if (suivante != nullptr) {
        suivante->precedente = precedente;
    }

    precedente = nullptr;
    suivante = nullptr;
}
