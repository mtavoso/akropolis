#include "IA.h"


void IAFacile::jouer_tour(Joueur& j, Chantier& c, const std::vector<bool>& variantes)
{
    j.choisir_tuile(0, c);
    for (const auto& tuple1 : j.get_cite()->get_carte()) {
        const Coordonnee& c1 = tuple1.first;
        const std::vector<Coordonnee>& voisines = c1.get_voisines();
        for (const Coordonnee& c2 : voisines) {
            for (const Coordonnee& c3 : voisines) {
                try {
                    j.placer_tuiles_temporairement(c1, c2, c3, j.get_cite()->get_tuile_temporaire());
                    j.confirmer_emplacement(c);

                    return;
                }
                catch (const JoueurException&) {
                    // Ne rien faire, on essaye la prochaine combinaison
                }
            }
        }
    }
}


//prend la tuile la moins chère et la plae au meilleur endroit possible 
void IAMoyen::jouer_tour(Joueur& j, Chantier& c, const std::vector<bool>& variantes)
{
    j.choisir_tuile(0, c);
    Tuile* tuile = j.get_cite()->get_tuile_temporaire();

    Score score_initial = j.get_score();
    int score_max = score_initial.get_valeur_score();

    std::vector<Coordonnee> meilleures_coords;
    bool coup_trouve = false;

    for (const auto& it : j.get_cite()->get_carte()) {
        const Coordonnee& c1 = it.first;
        const auto& voisines = c1.get_voisines();

        for (const Coordonnee& c2 : voisines) {
            for (const Coordonnee& c3 : voisines) {

                // sauvrgarde
                std::vector<Coordonnee> backup_positions =
                    j.get_cite()->get_position_temporaire();
                Tuile* backup_tuile =
                    j.get_cite()->get_tuile_temporaire();

                try {
                    j.placer_tuiles_temporairement(c1, c2, c3, tuile);

                    Score score_temp = score_initial;
                    score_temp.maj_score(j.get_cite(), variantes);

                    int score_test = score_temp.get_valeur_score();

                    if (score_test > score_max) {
                        score_max = score_test;
                        meilleures_coords = { c1, c2, c3 };
                        coup_trouve = true;
                    }
                }
                catch (const JoueurException&) {}

                // restauration
                j.get_cite()->set_tuile_temporaire(backup_tuile);
                j.get_cite()->set_position_temporaire(backup_positions);
            }
        }
    }

    if (coup_trouve) {
        // recree la meilleure position
        j.choisir_tuile(0, c);
        j.placer_tuiles_temporairement(
            meilleures_coords[0],
            meilleures_coords[1],
            meilleures_coords[2],
            j.get_cite()->get_tuile_temporaire()
        );
    }

    j.confirmer_emplacement(c);
}
void IADifficile::jouer_tour(Joueur& j, Chantier& c, const std::vector<bool>& variantes)
{
    Score score_initial = j.get_score();

    double meilleur_score_combine = -1e9;
    std::vector<Coordonnee> meilleures_coords;
    int meilleur_index_tuile = -1;

    const double k_etoiles = 2.0; //important en debut de partie
    const double k_points_quartier = 3.0;
    const double k_score = 5.0;
    const double k_pierres = 0.5;

    // Parcourt toutes les tuiles disponibles
    for (int i = 0; i < c.get_taille(); i++) {
        if (c.calculer_cout_piocher_tuile(i) > j.get_nb_pierres())
            break;

        try { j.choisir_tuile(i, c); }
        catch (...) { continue; }

        Tuile* tuile = j.get_cite()->get_tuile_temporaire();

        // Parcourt toutes les positions possibles dans la cité
        for (const auto& it : j.get_cite()->get_carte()) {
            const Coordonnee& c1 = it.first;
            const auto& voisines = c1.get_voisines();

            for (const Coordonnee& c2 : voisines) {
                for (const Coordonnee& c3 : voisines) {

                    // Sauvegarde temporaire
                    std::vector<Coordonnee> backup_positions =
                        j.get_cite()->get_position_temporaire();
                    Tuile* backup_tuile =
                        j.get_cite()->get_tuile_temporaire();

                    try {
                        j.placer_tuiles_temporairement(c1, c2, c3, tuile);

                        // Simulation du score
                        Score score_temp = score_initial;
                        score_temp.maj_score(j.get_cite(), variantes);

                        const std::array<int, NB_VARIANTES>& points_quartiers = score_temp.get_points_quartiers();
                        const std::array<int, NB_VARIANTES>& etoiles = score_temp.get_etoiles_places();

                        int gain_pierres = 0;
                        int gain_etoiles = 0;
                        int gain_points_quartier = 0;

                        int indice = 0;
                        for (const Coordonnee& pos : j.get_cite()->get_position_temporaire()) {
                            Hexagone& hex = j.get_cite()->recherche_hexagone(pos);
                            Construction* cons = hex.get_constru();

                            // Carrières recouvertes
                            if (cons && cons->get_type_construction() == "carriere")
                                gain_pierres++;

                            // on augmente les places qui sont à 0 (en priorisant celle qui ont deja des quartiers du meme type)
                            Construction* constr_tuile = tuile->get_constru(indice);
                            if (constr_tuile && constr_tuile->get_type_construction() == "Place") {
                                if (Place* place = dynamic_cast<Place*>(constr_tuile)) {
                                    std::string couleur = place->getCouleur();
                                    int idx = -1;
                                    if (couleur == "bleu")   idx = 0;
                                    else if (couleur == "jaune")  idx = 1;
                                    else if (couleur == "rouge")  idx = 2;
                                    else if (couleur == "violet") idx = 3;
                                    else if (couleur == "vert")   idx = 4;

                                    if (idx >= 0 && etoiles[idx] == 0) {
                                        gain_etoiles++;
                                        gain_points_quartier += points_quartiers[idx];
                                    }
                                }
                            }
                            indice++;
                        }

                        int delta_score = score_temp.get_valeur_score() - score_initial.get_valeur_score();
                        if (delta_score < -2) continue;

                        // Calcul du score combiné
                        double score_combine =
                            k_etoiles * gain_etoiles +
                            k_points_quartier * gain_points_quartier +
                            k_score * delta_score +
                            k_pierres * gain_pierres;

                        if (score_combine > meilleur_score_combine) {
                            meilleur_score_combine = score_combine;
                            meilleures_coords = { c1, c2, c3 };
                            meilleur_index_tuile = i;
                        }
                    }
                    catch (...) {}

                    // Restaurer l'état temporaire
                    j.get_cite()->set_tuile_temporaire(backup_tuile);
                    j.get_cite()->set_position_temporaire(backup_positions);
                }
            }
        }
    }

    if (meilleur_index_tuile >= 0) {
        // meilleur coup trouvé
        j.choisir_tuile(meilleur_index_tuile, c);
        j.placer_tuiles_temporairement(
            meilleures_coords[0],
            meilleures_coords[1],
            meilleures_coords[2],
            j.get_cite()->get_tuile_temporaire()
        );
        j.confirmer_emplacement(c);
        return;
    }

    throw JoueurException("IA bloquée : aucun coup possible");
}