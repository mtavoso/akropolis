#include "vueplateau.h"
#include <QResizeEvent>
#include <QDebug>
#include <algorithm>
#include "vueconstruction.h"

VuePlateau::VuePlateau(Cite* c, QWidget* parent): QWidget(parent), cite(c)
{
    largeur_hex = 80;
    hauteur_hex = 70;
    setMinimumSize(800, 600);
    if (cite != nullptr) {
        recreerInterface();
    }
}

void VuePlateau::setCite(Cite* c) {
    cite = c;
    recreerInterface();
}

void VuePlateau::recreerInterface() {
    for (auto const& [coord, vue] : vues_hexagones) {
        delete vue;
    }
    vues_hexagones.clear();
    selection.clear();
    emit selectionChanged(0);

    if (!cite) return;
    // On regarde si une tuile est en cours de placement
    std::vector<Coordonnee> pos_temp = cite->get_position_temporaire();
    Tuile* tuile_temp = cite->get_tuile_temporaire();
    bool mode_temporaire = !pos_temp.empty() && tuile_temp != nullptr;

    for (const auto& pair : cite->get_carte()) {
        const Coordonnee& coord = pair.first;
        Hexagone* hexa = pair.second.get();

        Vuehexagone* vue = new Vuehexagone(coord, hexa, this);
        vues_hexagones[coord] = vue;

        bool a_affiche_temporaire = false;

        // Si on est en mode placement, on regarde si cette case correspond à la tuile en main
        if (mode_temporaire) {
            for (size_t i = 0; i < pos_temp.size(); ++i) {
                if (pos_temp[i] == coord) {
                    // Si c'est une case temporaire, on affiche la construction de la tuile
                    VueConstruction* vc = new VueConstruction(tuile_temp->get_constru(i), vue);
                    vc->setFixedSize(largeur_hex, hauteur_hex);
                    vue->setConstruction(vc);

                    a_affiche_temporaire = true;
                    break;
                }
            }
        }
        if (!a_affiche_temporaire) {
            bool est_pose_definitif = !hexa->get_est_vide() && hexa->get_constru() != nullptr;
            if (est_pose_definitif) {
                VueConstruction* vc = new VueConstruction(hexa->get_constru(), vue);
                vc->setFixedSize(largeur_hex, hauteur_hex);
                vue->setConstruction(vc);
            }
        }
        connect(vue, &Vuehexagone::hexClicked, this, &VuePlateau::onHexClicked);
    }
    rafraichir();
}




void VuePlateau::rafraichir()
{
    int margeX = 40;
    int margeY = 40;
    for (const auto& pair : vues_hexagones) {
        const Coordonnee& c = pair.first;
        Vuehexagone* vue = pair.second;
        int grille_x = c.getX();
        int grille_y = c.getY();
        int pixelX = margeX + grille_x * (largeur_hex * 0.75);
        int pixelY = margeY + grille_y * hauteur_hex;
        if (grille_x % 2 != 0) {
            pixelY += hauteur_hex / 2;
        }
        vue->move(pixelX, pixelY);
        vue->resize(largeur_hex, hauteur_hex);
        vue->raise();
        vue->show();
    }
}

void VuePlateau::onHexClicked(int x, int y) {
    Coordonnee c(x, y);
    auto it = std::find(selection.begin(), selection.end(), c);
    if (it != selection.end()) {
        selection.erase(it);
        vues_hexagones[c]->setSelected(false);
    }
    else if (selection.size() < 3) {
        selection.push_back(c);
        vues_hexagones[c]->setSelected(true);
    }
    emit selectionChanged(selection.size());
}

void VuePlateau::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    rafraichir();
}
