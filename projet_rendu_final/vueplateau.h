#pragma once

#include <QWidget>
#include <map>

#include "plateau/Cite.h"
#include "plateau/Coordonnee.h"
#include "vuehexagone.h"

class VuePlateau : public QWidget
{
    Q_OBJECT

private:
    Cite* cite;

    std::map<Coordonnee, Vuehexagone*> vues_hexagones;

    int largeur_hex;
    int hauteur_hex;

    std::vector<Coordonnee> selection;


public:
    explicit VuePlateau(Cite* c, QWidget* parent = nullptr);
    void rafraichir();

    std::vector<Coordonnee> getSelection() const {return selection;}

    void onHexClicked(int x, int y);
    void setCite(Cite* c);
    void recreerInterface();
signals:
    void selectionChanged(int nbSelection);

protected:
    void resizeEvent(QResizeEvent* event) override;
};
