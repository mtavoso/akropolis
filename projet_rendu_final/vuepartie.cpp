#include "vuepartie.h"
#include <QMessageBox>
#include <QString>
#include <iostream>
#include <QApplication>
#include "exceptions/JoueurException.h"
#include "jeu/IllustreConstructeur.h"
#include "jeu/Caretaker.h"
#include "jeu/Memento.h"

VuePartie::VuePartie(QWidget *parent) : QWidget(parent) {
    indexTuileSelectionnee = -1;

    layoutPrincipal = new QHBoxLayout(this);
    QVBoxLayout* layoutGauche = new QVBoxLayout();

    vuePlateau = new VuePlateau(nullptr, this);
    layoutGauche->addWidget(vuePlateau, 1);

    vueChantier = new VueChantier(this);
    vueChantier->setFixedHeight(160);
    layoutGauche->addWidget(vueChantier, 0);

    connect(vueChantier, &VueChantier::tuileSelectionnee, this, &VuePartie::onTuileChantierChoisie);

    layoutPrincipal->addLayout(layoutGauche, 3);

    layoutDroit = new QVBoxLayout();

    QLabel* labelVue = new QLabel("Voir la Cité de :");
    labelVue->setStyleSheet("font-weight: bold; margin-top: 10px;");
    layoutDroit->addWidget(labelVue);

    comboJoueurVisuel = new QComboBox();
    layoutDroit->addWidget(comboJoueurVisuel);

    //Quand on change le nom, on change le plateau affiché
    connect(comboJoueurVisuel, qOverload<int>(&QComboBox::currentIndexChanged), this, &VuePartie::onJoueurVisuelChanged);

    QGroupBox* boxInfos = new QGroupBox("Informations");
    QVBoxLayout* layoutInfos = new QVBoxLayout(boxInfos);

    labelNomJoueur = new QLabel("Nom: -");
    labelNomJoueur->setStyleSheet("font-weight: bold; font-size: 16px; color: #2c3e50;");

    QHBoxLayout* layoutPierres = new QHBoxLayout();
    layoutPierres->addWidget(new QLabel("Pierres :"));
    lcdPierres = new QLCDNumber();
    lcdPierres->setSegmentStyle(QLCDNumber::Flat);
    layoutPierres->addWidget(lcdPierres);

    layoutInfos->addWidget(labelNomJoueur);
    layoutInfos->addLayout(layoutPierres);
    layoutDroit->addWidget(boxInfos);

    vueScore = new VueScore(this);
    layoutDroit->addWidget(vueScore);

    btnRotation = new QPushButton("pivoter tuile");
    btnConfirmer = new QPushButton("confirmer pose");

    btnRotation->setStyleSheet("background-color: #e67e22; color: white; font-weight: bold; padding: 8px; border-radius: 5px;");
    btnConfirmer->setStyleSheet("background-color: #27ae60; color: white; font-weight: bold; padding: 8px; border-radius: 5px;");

    layoutDroit->addWidget(btnRotation);
    layoutDroit->addWidget(btnConfirmer);

    btnAnnuler = new QPushButton("annuler choix");
    btnAnnuler->setStyleSheet("background-color: #e74c3c; color: white; font-weight: bold;");
    btnAnnuler->setEnabled(false);
    layoutDroit->addWidget(btnAnnuler);

    layoutDroit->addStretch();
    btnSauvegarder = new QPushButton("sauvegarder et quitter");
    btnSauvegarder->setStyleSheet("background-color: #c0392b; color: white;");
    layoutDroit->addWidget(btnSauvegarder);

    layoutPrincipal->addLayout(layoutDroit, 1);

    btnPlacer = new QPushButton("placer et vérifier");
    btnPlacer->setStyleSheet("background-color: #3498db; color: white; font-weight: bold; padding: 8px; border-radius: 5px;");
    layoutDroit->addWidget(btnPlacer);
    connect(btnPlacer, &QPushButton::clicked, this, &VuePartie::onPlacerClicked);

    connect(btnRotation, &QPushButton::clicked, this, &VuePartie::onRotationClicked);
    connect(btnConfirmer, &QPushButton::clicked, this, &VuePartie::onConfirmerClicked);
    connect(btnAnnuler, &QPushButton::clicked, this, &VuePartie::onAnnulerClicked);
    connect(btnSauvegarder, &QPushButton::clicked, this, &VuePartie::onSauvegarderClicked);

    btnConfirmer->setEnabled(false);
    btnRotation->setEnabled(false);
    tuileEnCoursDePlacement = false;
    indexJoueurCourant = 0;
    btnPlacer->setEnabled(false);
}


void VuePartie::onAnnulerClicked() {
    Partie& partie = Partie::get_instance();
    Joueur* joueur = partie.get_joueurs()[indexJoueurCourant];

    joueur->changer_tuile();
    indexTuileSelectionnee = -1;
    updateInterface();
}

void VuePartie::onRotationClicked() {
    Partie& partie = Partie::get_instance();
    Joueur* joueur = partie.get_joueurs()[indexJoueurCourant];

    Tuile* tuileTemp = joueur->get_cite()->get_tuile_temporaire();
    if (tuileTemp != nullptr) {
        tuileTemp->rotation_tuile(1);
        vuePlateau->recreerInterface();
    }
}

void VuePartie::onJoueurVisuelChanged() {
    updateInterface();
}


void VuePartie::onSauvegarderClicked() {
    // Récupérer l'instance du jeu
    Partie& partie = Partie::get_instance();

    // Créer le Memento
    Memento m = partie.creerMemento();

    // Sauvegarder via le Caretaker
    Caretaker caretaker;
    caretaker.sauvegarderMemento(m, "savegame_qt.txt");

    // Petit message de confirmation
    QMessageBox::information(this, "Sauvegarde", "La partie a été sauvegardée");
    QApplication::quit();
}


void VuePartie::onTuileChantierChoisie(int index) {
    try {
        Partie& partie = Partie::get_instance();
        Joueur* joueur = partie.get_joueurs()[indexJoueurCourant];

        // on ne peut pas choisir une tuile si on regarde le plateau d'un autre car ce n'est pas son tour
        if (comboJoueurVisuel->currentIndex() != indexJoueurCourant) {
            comboJoueurVisuel->setCurrentIndex(indexJoueurCourant);
        }

        indexTuileSelectionnee = index;
        joueur->choisir_tuile(index, *partie.get_chantier());

        updateInterface();

    } catch (std::exception& e) {
        QMessageBox::warning(this, "Action Impossible", e.what());
    }
}

void VuePartie::onPlacerClicked() {
    Partie& partie = Partie::get_instance();
    Joueur* joueur = partie.get_joueurs()[indexJoueurCourant];

    std::vector<Coordonnee> selection = vuePlateau->getSelection();

    if (selection.size() != 3) {
        QMessageBox::warning(this, "Attention", "Vous devez sélectionner exactement 3 cases sur le plateau.");
        return;
    }

    Tuile* tuileMain = joueur->get_cite()->get_tuile_temporaire();
    if (!tuileMain) {
        QMessageBox::warning(this, "Attention", "Aucune tuile sélectionnée dans le chantier.");
        return;
    }

    try {
        joueur->placer_tuiles_temporairement(selection[0], selection[1], selection[2], tuileMain);
        updateInterface();
        btnConfirmer->setEnabled(true);

    } catch (JoueurException& e) {
        QMessageBox::warning(this, "Placement Impossible", QString::fromStdString(e.what()));
    } catch (std::exception& e) {
        QMessageBox::critical(this, "Erreur", e.what());
    }
}

void VuePartie::updateInterface() {
    Partie& partie = Partie::get_instance();
    indexJoueurCourant = partie.get_index_joueur_courant();
    std::vector<Joueur*>& joueurs = partie.get_joueurs();
    if (joueurs.empty()) return;

    comboJoueurVisuel->blockSignals(true);
    bool besoinMiseAJour = false;

    if (comboJoueurVisuel->count() != (int)joueurs.size()) {
        besoinMiseAJour = true;
    } else {
        if (comboJoueurVisuel->itemText(0) != QString::fromStdString(joueurs[0]->getNom())) {
            besoinMiseAJour = true;
        }
    }

    if (besoinMiseAJour) {
        comboJoueurVisuel->clear();
        for (Joueur* j : joueurs) {
            comboJoueurVisuel->addItem(QString::fromStdString(j->getNom()));
        }
    }
    comboJoueurVisuel->blockSignals(false);
    if (indexJoueurCourant >= (int)joueurs.size()) indexJoueurCourant = 0;
    Joueur* joueurActif = joueurs[indexJoueurCourant];
    int indexVisuel = comboJoueurVisuel->currentIndex();
    if (indexVisuel < 0 || indexVisuel >= (int)joueurs.size()) indexVisuel = 0;

    Joueur* joueurVu = joueurs[indexVisuel];

    labelNomJoueur->setText("Plateau de : " + QString::fromStdString(joueurVu->getNom()));
    lcdPierres->display(joueurVu->get_nb_pierres());
    vueScore->updateScore(joueurVu);
    vuePlateau->setCite(joueurVu->get_cite());
    vueChantier->updateChantier();
    if (indexTuileSelectionnee != -1) {
        vueChantier->forcerSelection(indexTuileSelectionnee);
    }

    // Gestion des boutons : on ne peut cliquer sur les boutons que si c'est mon tour et je regarde mon plateau

    bool estSurMonPlateau = (indexVisuel == indexJoueurCourant);

    if (estSurMonPlateau && joueurActif->get_cite()->get_tuile_temporaire() != nullptr) {
        btnRotation->setEnabled(true);
        btnAnnuler->setEnabled(true);
        btnPlacer->setEnabled(true);

        bool placementFait = !joueurActif->get_cite()->get_position_temporaire().empty();
        btnConfirmer->setEnabled(placementFait);

        vueChantier->setEnabled(false);

    } else {
        btnPlacer->setEnabled(false);
        btnConfirmer->setEnabled(false);
        btnRotation->setEnabled(false);
        btnAnnuler->setEnabled(false);
        vueChantier->setEnabled(estSurMonPlateau);
    }
}

void VuePartie::onConfirmerClicked() {
    try {
        Partie& partie = Partie::get_instance();
        Joueur* joueur = partie.get_joueurs()[indexJoueurCourant];
        joueur->confirmer_emplacement(*partie.get_chantier());
        joueur->changer_tuile();

        if (indexTuileSelectionnee != -1) {
            joueur->debiter_pierres(indexTuileSelectionnee);
            indexTuileSelectionnee = -1;
        }
        if (partie.get_chantier()->est_vide()) {
            std::vector<Joueur*> classement = partie.determiner_classement();

            QString msg = "Partie Terminée !\n\nClassement Final :\n";
            for (size_t i = 0; i < classement.size(); ++i) {
                msg += QString("%1. %2 : %3 points\n")
                .arg(i + 1)
                    .arg(QString::fromStdString(classement[i]->getNom()))
                    .arg(classement[i]->get_score().get_valeur_score());
            }

            QMessageBox::information(this, "Fin de Partie", msg);
            this->close();
            return;
        }
        QMessageBox::information(this, "Succès", "Tuile validée. Au joueur suivant.");
        indexJoueurCourant++;
        if (indexJoueurCourant >= (int)partie.get_joueurs().size()) {
            indexJoueurCourant = 0;
        }
        partie.set_index_joueur_courant(indexJoueurCourant);

        Joueur* prochainJoueur = partie.get_joueurs()[indexJoueurCourant];

        while (dynamic_cast<IllustreConstructeur*>(prochainJoueur) != nullptr) {
            prochainJoueur->jouer_tour(*partie.get_chantier(), *partie.get_variantes());
            prochainJoueur->changer_tuile();
            if (partie.get_chantier()->est_vide()) {
                std::vector<Joueur*> classement = partie.determiner_classement();

                QString msg = "Partie Terminée !\n\nClassement Final :\n";
                for (size_t i = 0; i < classement.size(); ++i) {
                    msg += QString("%1. %2 : %3 points\n")
                    .arg(i + 1)
                        .arg(QString::fromStdString(classement[i]->getNom()))
                        .arg(classement[i]->get_score().get_valeur_score());
                }

                QMessageBox::information(this, "Fin de Partie", msg);
                this->close();
                return;
            }
            // Joueur suivant
            indexJoueurCourant++;
            if (indexJoueurCourant >= (int)partie.get_joueurs().size()) {
                indexJoueurCourant = 0;
            }
            partie.set_index_joueur_courant(indexJoueurCourant);
            prochainJoueur = partie.get_joueurs()[indexJoueurCourant];
        }
        comboJoueurVisuel->setCurrentIndex(indexJoueurCourant);

        updateInterface();

    } catch (std::exception& e) {
        QMessageBox::critical(this, "Erreur", e.what());
    }
}
