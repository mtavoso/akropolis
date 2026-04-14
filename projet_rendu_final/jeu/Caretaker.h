#pragma once
#include "Memento.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

class Caretaker {
public:
    void sauvegarderMemento(const Memento& m, const std::string& filename) {
        std::ofstream fichier(filename);
        if (fichier.is_open()) {
            fichier << m.getEtat();
            fichier.close();
            std::cout << "[Succes] Partie sauvegardee dans " << filename << std::endl;
        }
        else {
            std::cerr << "[Erreur] Impossible de creer le fichier de sauvegarde." << std::endl;
        }
    }

    Memento chargerMemento(const std::string& filename) {
        std::ifstream fichier(filename);
        std::stringstream buffer;
        if (fichier.is_open()) {
            buffer << fichier.rdbuf();
            fichier.close();
            std::cout << "[Succes] Partie chargee !" << std::endl;
            return Memento(buffer.str());
        }
        else {
            throw std::runtime_error("Fichier de sauvegarde introuvable.");
        }
    }
};