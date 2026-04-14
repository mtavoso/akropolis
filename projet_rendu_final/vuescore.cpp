#include "vuescore.h"
#include <QHeaderView>
#include "jeu/Partie.h"

VueScore::VueScore(QWidget *parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* titre = new QLabel("SCORE ACTUEL");
    titre->setStyleSheet("font-weight: bold; font-size: 14px; color: #2c3e50;");
    titre->setAlignment(Qt::AlignCenter);
    layout->addWidget(titre);

    // Tableau
    tableauScore = new QTableWidget(5, 3, this);
    QStringList headers;
    headers << "Type" << "Points" << "Étoiles";
    tableauScore->setHorizontalHeaderLabels(headers);
    tableauScore->verticalHeader()->setVisible(false);
    tableauScore->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableauScore->setEditTriggers(QAbstractItemView::NoEditTriggers); // Lecture seule
    tableauScore->setFixedHeight(180);

    layout->addWidget(tableauScore);

    // Total
    labelTotal = new QLabel("Total : 0");
    labelTotal->setStyleSheet("font-size: 16px; font-weight: bold; color: #e74c3c; margin-top: 10px;");
    labelTotal->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelTotal);
}

void VueScore::updateScore(Joueur* joueur) {
    if (!joueur) return;
    const std::vector<bool>* variantes = Partie::get_instance().get_variantes();
    if (variantes) joueur->get_score().maj_score(joueur->get_cite(), *variantes);
    Score& s = joueur->get_score();
    QStringList types = {"Habitations", "Marchés", "Casernes", "Temples", "Jardins"};
    for(int i=0; i<5; ++i) {
        // Colonne 1 : Type
        tableauScore->setItem(i, 0, new QTableWidgetItem(types[i]));

        // Colonne 2 : Points
        tableauScore->setItem(i, 1, new QTableWidgetItem(QString::number(s.get_points_quartiers()[i])));

        // Colonne 3 : Étoiles
        tableauScore->setItem(i, 2, new QTableWidgetItem(QString::number(s.get_etoiles_places()[i])));
    }
    labelTotal->setText("Total : " + QString::number(s.get_valeur_score()));
}
