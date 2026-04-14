#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "jeu/Joueur.h"

class VueScore : public QWidget {
    Q_OBJECT

public:
    explicit VueScore(QWidget *parent = nullptr);

    void updateScore(Joueur* joueur);

private:
    QTableWidget* tableauScore;
    QLabel* labelTotal;
};
