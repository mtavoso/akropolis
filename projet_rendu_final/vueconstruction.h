#pragma once
#include <QWidget>
#include "abstract/Construction.h"

class VueConstruction : public QWidget {
    Q_OBJECT
public:
    explicit VueConstruction(Construction* c, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Construction* construction;
    QColor determinerCouleur() const;
    int getNombreEtoiles() const;
};
