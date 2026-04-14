#include "mainwindow.h"
#include <QMessageBox>
#include <vector>
#include <string>
#include "jeu/Partie.h"
#include "jeu/Caretaker.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Configuration de la fenêtre principale
    resize(1280, 720);
    setWindowTitle("Akropolis");

    // Création du gestionnaire de pages
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Création des Vues
    vueMenu = new VueMenu(this);
    vuePartie = new VuePartie(this);

    // Ajout des vues à la pile
    stackedWidget->addWidget(vueMenu);
    stackedWidget->addWidget(vuePartie);

    // Connexion du Signal menu au slot MainWindow
    connect(vueMenu, &VueMenu::signalLancerPartie, this, &MainWindow::onLancerPartie);
    connect(vueMenu, &VueMenu::signalReprendrePartie, this, &MainWindow::chargerPartie);
}

MainWindow::~MainWindow() {
}

void MainWindow::onLancerPartie() {
    try {
        // Recuperation des donnees
        int nbJoueurs = vueMenu->getNbJoueurs();
        QVector<QString> nomsQt = vueMenu->getNomsJoueurs();
        QVector<bool> variantesQt = vueMenu->getVariantes();
        bool estSolo = vueMenu->isModeSolo();

        // Conversion des noms
        std::vector<std::string> nomsStd;
        for(const QString& nom : std::as_const(nomsQt)) {
            nomsStd.push_back(nom.toStdString());
        }

        // Conversion des variantes
        std::vector<bool> vecVariantes;
        for(int i = 0; i < 5; ++i) {
            vecVariantes.push_back(variantesQt.value(i, false));
        }

        if (estSolo) {
            std::string nomHumain = "Humain";
            if (!nomsStd.empty()) {
                nomHumain = nomsStd[0];
            }

            // Récupérer et convertir la difficulté
            int diff = vueMenu->getNiveauDifficulte();
            std::string niveauStr = "moyen";
            if (diff == 0) niveauStr = "facile";
            else if (diff == 2) niveauStr = "difficile";
            Partie::get_instance(true, vecVariantes, nomHumain, niveauStr);
        }
        else {
            Partie::get_instance(nbJoueurs, true, vecVariantes, nomsStd);
        }
        // On demande à la VuePartie de se mettre à jour avec les nouvelles données
        vuePartie->updateInterface();

        // On bascule l'affichage sur le jeu
        stackedWidget->setCurrentWidget(vuePartie);

    }
    catch (std::exception& e) {
        QMessageBox::critical(this, "Erreur de lancement", e.what());
    }
    catch (...) {
        QMessageBox::critical(this, "Erreur", "Une erreur inconnue est survenue lors du lancement.");
    }
}

void MainWindow::chargerPartie() {
    try {
        Caretaker caretaker;
        // Charger le fichier
        Memento m = caretaker.chargerMemento("savegame_qt.txt");

        // Préparer l'instance unique de partie et on met des valeurs bidons qui seront écrasées
        std::vector<bool> dummyVars(5, false);
        Partie& partie = Partie::get_instance(true, dummyVars, "Temp", "facile");

        // Restaurer les vraies données
        partie.retablirMemento(m);

        // Mettre à jour VuePartie avec les nouvelles données
        vuePartie->updateInterface();

        // Changer l'écran pour afficher le jeu
        stackedWidget->setCurrentWidget(vuePartie);
    } catch (std::exception& e) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger la sauvegarde.");
    }
}
