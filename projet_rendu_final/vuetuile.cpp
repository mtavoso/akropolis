#include "vuetuile.h"
#include "vueconstruction.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>

VueTuile::VueTuile(Tuile* t, QWidget *parent) : QWidget(parent), tuileModele(t) {
    estSelectionnee = false;
    setFixedSize(100, 100);

    // Placement des 3 hexagones
    for(int i=0; i<3; ++i) {
        Construction* c = (t != nullptr) ? t->get_constru(i) : nullptr;
        VueConstruction* vh = new VueConstruction(c, this);
        vh->setFixedSize(50, 44);
        vh->setAttribute(Qt::WA_TransparentForMouseEvents);
        int margeX = 5;
        int margeY = 5;
        if (i == 0) vh->move(margeX, margeY);
        else if (i == 1) vh->move(margeX, margeY + 44);
        else if (i == 2) vh->move(margeX + 37, margeY + 22);
    }

    setCursor(Qt::PointingHandCursor);
}

void VueTuile::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit tuileClicked(); // On signale qu'on a cliqué sur cette tuile
        event->accept();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void VueTuile::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    // Si la tuile est sélectionnée, on dessine le cadre rouge
    if (estSelectionnee) {
        QPainter painter(this);
        QPen pen(Qt::red);
        pen.setWidth(4);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(rect().adjusted(2, 2, -2, -2));
    }
}
