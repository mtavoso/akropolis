#include "vuechantier.h"
#include "jeu/Partie.h"
#include <QLabel>
#include <QVBoxLayout>

VueChantier::VueChantier(QWidget *parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* titre = new QLabel("CHANTIER");
    titre->setStyleSheet("font-weight: bold; font-size: 14px; color: #2c3e50;");
    titre->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titre);

    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    QWidget* containerTuiles = new QWidget();
    layoutChantier = new QHBoxLayout(containerTuiles);
    layoutChantier->setAlignment(Qt::AlignLeft);
    layoutChantier->setContentsMargins(5, 5, 5, 5);

    scroll->setWidget(containerTuiles);
    mainLayout->addWidget(scroll);
}

void VueChantier::updateChantier() {
    QLayoutItem* item;
    while ((item = layoutChantier->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
    listeVueTuiles.clear();

    Chantier* chantier = Partie::get_instance().get_chantier();
    if (!chantier) return;

    for (int i = 0; i < chantier->get_taille(); ++i) {
        Tuile* t = chantier->get_tuile(i);

        QWidget* wrapper = new QWidget();
        QVBoxLayout* vlay = new QVBoxLayout(wrapper);
        vlay->setContentsMargins(0,0,0,0);
        vlay->setSpacing(2);

        VueTuile* vt = new VueTuile(t, this);
        listeVueTuiles.push_back(vt);

        connect(vt, &VueTuile::tuileClicked, this, [this, i](){
            this->onTuileSelectionnee(i);
        });

        int cout = chantier->calculer_cout_piocher_tuile(i);
        QLabel* lblPrix = new QLabel();

        if (cout == 0) {
            lblPrix->setText("Gratuit");
            lblPrix->setStyleSheet("color: green; font-weight: bold; font-size: 12px;");
        } else {
            lblPrix->setText(QString("%1 Pierre%2").arg(cout).arg(cout > 1 ? "s" : ""));
            lblPrix->setStyleSheet("color: #c0392b; font-weight: bold; font-size: 12px;");
        }
        lblPrix->setAlignment(Qt::AlignCenter);
        lblPrix->setFixedHeight(20);

        vlay->addWidget(vt);
        vlay->addWidget(lblPrix);
        vlay->setAlignment(Qt::AlignCenter);

        layoutChantier->addWidget(wrapper);
    }
}

void VueChantier::onTuileSelectionnee(int index) {
    for (size_t i = 0; i < listeVueTuiles.size(); ++i) {
        if ((int)i == index) {
            listeVueTuiles[i]->setSelectionnee(true);
        } else {
            listeVueTuiles[i]->setSelectionnee(false);
        }
    }
    emit tuileSelectionnee(index);
}
void VueChantier::forcerSelection(int index) {
    for (size_t i = 0; i < listeVueTuiles.size(); ++i) {
        if ((int)i == index) {
            listeVueTuiles[i]->setSelectionnee(true);
        } else {
            listeVueTuiles[i]->setSelectionnee(false);
        }
    }
}
