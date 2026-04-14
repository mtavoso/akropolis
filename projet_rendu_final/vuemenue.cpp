#include "vuemenue.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

VueMenu::VueMenu(QWidget *parent) : QWidget(parent) {
    // Layout principal vertical
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(50, 20, 50, 20);

    // Titre
    titreJeu = new QLabel("AKROPOLIS");
    titreJeu->setAlignment(Qt::AlignCenter);
    titreJeu->setStyleSheet("font-size: 36px; font-weight: bold; color: #d35400; margin-bottom: 20px;");
    mainLayout->addWidget(titreJeu);

    // Groupe mode solo
    groupModeSolo = new QGroupBox("Mode Solo");
    groupModeSolo->setCheckable(true);
    groupModeSolo->setChecked(false);

    QHBoxLayout* layoutSolo = new QHBoxLayout(groupModeSolo);
    radioSoloFacile = new QRadioButton("Facile");
    radioSoloMoyen = new QRadioButton("Moyen");
    radioSoloDifficile = new QRadioButton("Difficile");
    radioSoloFacile->setChecked(true);

    layoutSolo->addWidget(radioSoloFacile);
    layoutSolo->addWidget(radioSoloMoyen);
    layoutSolo->addWidget(radioSoloDifficile);

    mainLayout->addWidget(groupModeSolo);

    groupNbJoueurs = new QGroupBox("Nombre de joueurs");
    QHBoxLayout* layoutNb = new QHBoxLayout(groupNbJoueurs);

    radio2Joueurs = new QRadioButton("2");
    radio3Joueurs = new QRadioButton("3");
    radio4Joueurs = new QRadioButton("4");
    radio2Joueurs->setChecked(true);

    layoutNb->addWidget(radio2Joueurs);
    layoutNb->addWidget(radio3Joueurs);
    layoutNb->addWidget(radio4Joueurs);

    mainLayout->addWidget(groupNbJoueurs);

    // Groupe Noms des Joueurs
    groupNoms = new QGroupBox("Identité des joueurs");
    layoutNoms = new QVBoxLayout(groupNoms);

    for(int i = 0; i < 4; ++i) {
        QLineEdit* edit = new QLineEdit();
        edit->setPlaceholderText(QString("Nom du Joueur %1").arg(i + 1));
        listeEditsNoms.push_back(edit);
        layoutNoms->addWidget(edit);
    }
    mainLayout->addWidget(groupNoms);

    groupVariantes = new QGroupBox("Variantes du jeu");
    QVBoxLayout* layoutVar = new QVBoxLayout(groupVariantes);

    QStringList nomsVariantes = {
        "Habitations",
        "Marchés",
        "Casernes",
        "Temples",
        "Jardins"
    };

    for(const QString& nom : nomsVariantes) {
        QCheckBox* cb = new QCheckBox(nom);
        listeCheckVariantes.push_back(cb);
        layoutVar->addWidget(cb);
    }
    mainLayout->addWidget(groupVariantes);

    // Boutons d'action
    btnLancer = new QPushButton("Lancer une nouvelle partie");
    btnLancer->setMinimumHeight(50);
    btnLancer->setCursor(Qt::PointingHandCursor);
    btnLancer->setStyleSheet("background-color: #27ae60; color: white; font-size: 18px; font-weight: bold; border-radius: 5px;");

    btnReprendre = new QPushButton("Reprendre une partie sauvegardée");

    mainLayout->addWidget(btnLancer);
    mainLayout->addWidget(btnReprendre);
    mainLayout->addStretch();

    // Mise à jour de l'interface
    connect(groupModeSolo, &QGroupBox::toggled, this, &VueMenu::onConfigurationChanged);
    connect(radio2Joueurs, &QRadioButton::toggled, this, &VueMenu::onConfigurationChanged);
    connect(radio3Joueurs, &QRadioButton::toggled, this, &VueMenu::onConfigurationChanged);
    connect(radio4Joueurs, &QRadioButton::toggled, this, &VueMenu::onConfigurationChanged);

    // Lancement
    connect(btnLancer, &QPushButton::clicked, this, &VueMenu::signalLancerPartie);
    connect(btnReprendre, &QPushButton::clicked, this, &VueMenu::signalReprendrePartie);

    // Initialisation visuelle correcte
    updateAffichageNoms();
}

void VueMenu::onConfigurationChanged() {
    updateAffichageNoms();
}

void VueMenu::updateAffichageNoms() {
    bool isSolo = groupModeSolo->isChecked();
    int nbJoueursVoulus = 2;

    if (isSolo) {
        groupNbJoueurs->setEnabled(false);
        nbJoueursVoulus = 1;
    } else {
        groupNbJoueurs->setEnabled(true);
        if (radio3Joueurs->isChecked()) nbJoueursVoulus = 3;
        else if (radio4Joueurs->isChecked()) nbJoueursVoulus = 4;
        else nbJoueursVoulus = 2;
    }
    for(int i = 0; i < listeEditsNoms.size(); ++i) {
        if (i < nbJoueursVoulus) {
            listeEditsNoms[i]->setVisible(true);
            listeEditsNoms[i]->setPlaceholderText(isSolo ? "Votre Nom " : QString("Nom du Joueur %1").arg(i + 1));
        } else {
            listeEditsNoms[i]->setVisible(false);
            listeEditsNoms[i]->clear();
        }
    }
}

int VueMenu::getNbJoueurs() const {
    if (groupModeSolo->isChecked()) return 2;
    if (radio3Joueurs->isChecked()) return 3;
    if (radio4Joueurs->isChecked()) return 4;
    return 2;
}

QVector<QString> VueMenu::getNomsJoueurs() const {
    QVector<QString> noms;
    for(auto edit : listeEditsNoms) {
        if(edit->isVisible() && !edit->text().isEmpty()) {
            noms.push_back(edit->text());
        } else if (edit->isVisible()) {
            // Nom par défaut si l'utilisateur laisse vide
            noms.push_back(QString("Joueur %1").arg(noms.size() + 1));
        }
    }
    return noms;
}

QVector<bool> VueMenu::getVariantes() const {
    QVector<bool> result;
    for(auto cb : listeCheckVariantes) {
        result.push_back(cb->isChecked());
    }
    return result;
}

bool VueMenu::isModeSolo() const {
    return groupModeSolo->isChecked();
}

int VueMenu::getNiveauDifficulte() const {
    if (radioSoloDifficile->isChecked()) return 2;
    if (radioSoloMoyen->isChecked()) return 1;
    return 0;
}
