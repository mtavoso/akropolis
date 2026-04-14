#pragma once

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <QLabel>

class VueMenu : public QWidget {
    Q_OBJECT

public:
    explicit VueMenu(QWidget *parent = nullptr);
    int getNbJoueurs() const;
    QVector<QString> getNomsJoueurs() const;
    QVector<bool> getVariantes() const;
    bool isModeSolo() const;
    int getNiveauDifficulte() const;

signals:
    void signalLancerPartie();
    void signalReprendrePartie();

private slots:
    void onConfigurationChanged();

private:
    QLabel* titreJeu;

    QGroupBox* groupModeSolo;
    QRadioButton* radioSoloFacile;
    QRadioButton* radioSoloMoyen;
    QRadioButton* radioSoloDifficile;

    QGroupBox* groupNbJoueurs;
    QRadioButton* radio2Joueurs;
    QRadioButton* radio3Joueurs;
    QRadioButton* radio4Joueurs;

    QGroupBox* groupNoms;
    QVBoxLayout* layoutNoms;
    QVector<QLineEdit*> listeEditsNoms;

    QGroupBox* groupVariantes;
    QVector<QCheckBox*> listeCheckVariantes;

    QPushButton* btnLancer;
    QPushButton* btnReprendre;

    void updateAffichageNoms();
};
