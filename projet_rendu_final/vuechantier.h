#pragma once
#include <QWidget>
#include <QHBoxLayout>
#include <QScrollArea>
#include <vector>
#include "plateau/Chantier.h"
#include "vuetuile.h"

class VueChantier : public QWidget {
    Q_OBJECT
public:
    explicit VueChantier(QWidget *parent = nullptr);
    void updateChantier();
    void forcerSelection(int index);

signals:
    void tuileSelectionnee(int index);

private slots:
    void onTuileSelectionnee(int index);

private:
    QHBoxLayout* layoutChantier;
    std::vector<VueTuile*> listeVueTuiles;
};
