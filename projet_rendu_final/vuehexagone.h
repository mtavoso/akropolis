#ifndef HEXA_H
#define HEXA_H
#pragma once
#include <QWidget>
#include "vueconstruction.h"
#include "plateau/Hexagone.h"
#include "plateau/Coordonnee.h"

class Vuehexagone : public QWidget {
    Q_OBJECT

private:
    Hexagone* hexagone;
    int x_coord;
    int y_coord;
    VueConstruction* vueConstruction;
    bool clicked;

public:
    Vuehexagone(const Coordonnee& coord,Hexagone* hexa, QWidget* parent = nullptr);
    void setConstruction(VueConstruction* c);
    bool isSelected() const { return clicked; }
    void setSelected(bool s) { clicked = s; update(); }
signals:
    void hexClicked(int x, int y);
protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;
};

#endif
