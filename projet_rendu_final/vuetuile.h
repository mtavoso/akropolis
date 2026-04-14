#pragma once
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include "plateau/Tuile.h"

class VueTuile : public QWidget {
    Q_OBJECT

public:
    explicit VueTuile(Tuile* tuile, QWidget *parent = nullptr);

    void setSelectionnee(bool etat) {
        estSelectionnee = etat;
        update();
    }

signals:
    void tuileClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Tuile* tuileModele;
    bool estSelectionnee;
};
