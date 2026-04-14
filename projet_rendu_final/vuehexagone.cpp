#include "vuehexagone.h"
#include <QPainter>
#include <QMouseEvent>
#define _USE_MATH_DEFINES
#include <cmath>

Vuehexagone::Vuehexagone(const Coordonnee& coord, Hexagone* hexa, QWidget* parent): QWidget(parent), x_coord(coord.getX()), y_coord(coord.getY()), hexagone(hexa){
    setMinimumSize(80, 80);
    clicked = false;
}

void Vuehexagone::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    double w = width();
    double h = height();
    QPointF center(w / 2.0, h / 2.0);
    double rayon = (h / 2.0) - 8;
    QPolygonF hex;
    for (int i = 0; i < 6; i++) {
        double angle_deg = 60 * i;
        double angle_rad = M_PI / 180.0 * angle_deg;
        hex << QPointF(center.x() + rayon * cos(angle_rad),
                       center.y() + rayon * sin(angle_rad));
    }

    if (clicked)
        painter.setPen(QPen(Qt::red, 3));
    else
        painter.setPen(QPen(Qt::black, 2));

    painter.setBrush(QColor(220, 220, 220));
    painter.drawPolygon(hex);
    int etage = hexagone->get_etage();
    if (etage > 0) {
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        QRectF rect(center.x() - 15, center.y() + 10, 30, 15);
        painter.drawText(rect, Qt::AlignCenter, QString("E%1").arg(etage));
    }
    painter.setBrush(QColor(220, 220, 220));
    painter.drawPolygon(hex);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 8));
    QRect rect(center.x() - 20, center.y() - 10, 40, 20);
    QString debugCoord = QString("%1,%2").arg(x_coord).arg(y_coord);
    painter.drawText(rect, Qt::AlignCenter, debugCoord);
}


void Vuehexagone::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit hexClicked(x_coord, y_coord);
    }
}

void Vuehexagone::setConstruction(VueConstruction* c)
{
    vueConstruction = c;
    if (vueConstruction) {
        vueConstruction->setParent(this);
        int size = width() - 10;
        vueConstruction->setGeometry((width() / 2 - size / 2)-5, (height() / 2 - size / 2)+10, size, size);
        vueConstruction->show();
    }
    update();
}
