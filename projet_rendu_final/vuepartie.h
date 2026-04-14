#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLCDNumber>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QMessageBox>

#include "vueplateau.h"
#include "vuechantier.h"
#include "vuescore.h"

#include "jeu/Partie.h"
#include "jeu/Joueur.h"

class VuePartie : public QWidget {
    Q_OBJECT // Macro indispensable pour les signaux/slots Qt

public:
    explicit VuePartie(QWidget *parent = nullptr);

    // Méthode appelée pour rafraîchir tout l'affichage (Score, Pierres, Plateau...)
    void updateInterface();

private slots:
    // Les fonctions appelées quand on clique sur les boutons
    void onRotationClicked();
    void onConfirmerClicked();
    void onSauvegarderClicked();
    void onAnnulerClicked();
    void onTuileChantierChoisie(int index);
    void onPlacerClicked();
    void onJoueurVisuelChanged();

private:
    VuePlateau* vuePlateau;
    VueChantier* vueChantier;
    VueScore* vueScore;
    QPushButton* btnAnnuler;


    QLabel* labelNomJoueur;
    QLCDNumber* lcdPierres;

    QPushButton* btnRotation;
    QPushButton* btnConfirmer;
    QComboBox* comboJoueurVisuel;

    int indexTuileSelectionnee;

    bool tuileEnCoursDePlacement;
    int indexJoueurCourant;

    QHBoxLayout* layoutPrincipal;
    QVBoxLayout* layoutDroit;
    QPushButton* btnSauvegarder;
    QPushButton* btnPlacer;
};
