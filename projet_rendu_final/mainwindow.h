#pragma once

#include <QMainWindow>
#include <QStackedWidget>

#include "vuemenue.h"
#include "vuepartie.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onLancerPartie();
    void chargerPartie();

private:
    // Le widget qui gère les pages de menu vers jeu
    QStackedWidget* stackedWidget;

    // Les deux écrans principaux
    VueMenu* vueMenu;
    VuePartie* vuePartie;
};
