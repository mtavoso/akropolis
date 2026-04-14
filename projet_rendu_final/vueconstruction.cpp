#include "vueconstruction.h"
#include "plateau/Place.h"
#include <QPainter>
#include <cmath>

void VueConstruction::paintEvent(QPaintEvent *) {
    if (!construction) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Calcul des points de l'hexagone
    QPolygonF hexagone;
    double w = width();
    double h = height();
    QPointF centre(w / 2.0, h / 2.0);
    double rayon = qMin(w, h) / 2.0 - 6;

    if (rayon < 1) rayon = 1;

    for (int i = 0; i < 6; ++i) {
        double angle_deg = 60 * i;
        double angle_rad = 3.14159 / 180.0 * angle_deg;
        hexagone << QPointF(centre.x() + rayon * cos(angle_rad),
                            centre.y() + rayon * sin(angle_rad));
    }

    // Dessin de l'hexagone
    painter.setBrush(determinerCouleur());
    painter.setPen(QPen(Qt::black, 2));
    painter.drawPolygon(hexagone);


    // Ajout du texte : étoiles ou carrière
    painter.setPen(Qt::white);

    // Gestion carrière
    if (construction->get_type_construction() == "carriere") {
        QFont f = painter.font();
        f.setPixelSize(10);
        painter.setFont(f);
        painter.drawText(rect(), Qt::AlignCenter, "Carrière");
    }

    // Gestion étoiles
    int nbEtoiles = getNombreEtoiles();
    if (nbEtoiles > 0) {
        QString etoiles = "";
        for(int i=0; i<nbEtoiles; i++) etoiles += "*";
        QFont f = painter.font();
        f.setBold(true);
        f.setPixelSize(18);
        painter.setFont(f);
        painter.drawText(rect(), Qt::AlignCenter, etoiles);
    }
}

VueConstruction::VueConstruction(Construction* c, QWidget *parent): QWidget(parent), construction(c) {
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

QColor VueConstruction::determinerCouleur() const {
    if (!construction) return Qt::transparent;

    std::string type = construction->get_type_construction();

    if (type == "Habitation") return QColor(0, 0, 255);
    if (type == "Marche") return QColor(255, 215, 0);
    if (type == "Caserne") return QColor(255, 69, 0);
    if (type == "Temple") return QColor(138, 43, 226);
    if (type == "Jardin") return QColor(34, 139, 34);
    if (type == "carriere") return QColor(169, 169, 169);

    if (type == "Place") {
        // On cast pour accéder aux méthodes spécifiques de place
        const Place* p = dynamic_cast<const Place*>(construction);
        if (p) {
            std::string c = p->getCouleur();
            if (c == "bleu") return QColor(0, 0, 255);
            if (c == "jaune") return QColor(255, 215, 0);
            if (c == "rouge") return QColor(255, 69, 0);
            if (c == "violet") return QColor(138, 43, 226);
            if (c == "vert") return QColor(34, 139, 34);
        }
        return Qt::black;
    }

    return Qt::white;
}

int VueConstruction::getNombreEtoiles() const {
    if (construction && construction->get_type_construction() == "Place") {
        const Place* p = dynamic_cast<const Place*>(construction);
        if (p) return p->getNbEtoiles();
    }
    return 0;
}

