#pragma once
#include "../abstract/Quartier.h"
#include <vector>

class Habitation : public Quartier {
private:
    Habitation* precedente;
    Habitation* suivante;
    int id_chaine;
    int points;
    static int compteur_id_chaine;
public:
    using Quartier::Quartier;
    Habitation(int id) : Quartier(id), precedente(nullptr), suivante(nullptr), id_chaine(compteur_id_chaine++), points(0) {}

    // setters et getters :
    int get_id_chaine() const { return id_chaine; }
    void set_id_chaine(int id) { id_chaine = id; }
    Habitation* get_precedente() const { return precedente; }
    void set_precedente(Habitation* h);
    Habitation* get_suivante() const { return suivante; }
    void set_suivante(Habitation* h);
    int get_points() const { return points; }
    void set_points(int p) { points = p; }


    bool est_valide_variante(const std::vector<Construction*>& voisins, const int etage) const override;


    void update_id_chaine_gauche(int id);
    void update_id_chaine_droite(int id);
    void update_id_chaine(int id);

    void modifier_points_gauche(int valeur);
    void modifier_points_droite(int valeur);
    void modifier_points(int valeur);

    Habitation* trouver_bout_chaine();
    Habitation* trouver_debut_chaine();

    void rejoindre_des_chaines(const std::vector<Construction*>& voisins, int etage);

    std::string get_type_construction() const override;
    void afficher_construction() const override;
    bool est_valide(const std::vector<Construction*>& voisins) const override;

    void supprimer(int etage);
};
