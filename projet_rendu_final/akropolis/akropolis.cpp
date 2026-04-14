
#include <iostream>
#include <vector>
#include <string>
#include "../jeu/Partie.h"
#include "../jeu/Caretaker.h"

int main()
{
    std::cout << "=== AKROPOLIS ===\n\n";

    // MENU DE DÉPART 
    std::cout << "1. Nouvelle Partie\n";
    std::cout << "2. Charger la derniere sauvegarde\n";
    std::cout << "> Choix : ";

    int choixMenu;
    std::cin >> choixMenu;
    std::cin.ignore(); // Nettoyage buffer

    // CAS CHARGEMENT 
    if (choixMenu == 2) {
        try {
            Caretaker caretaker;
            // 1. Lecture du fichier
            Memento m = caretaker.chargerMemento("sauvegarde.txt");

            // 2. Initialisation "bidon" pour créer l'objet Singleton Partie
            std::vector<bool> dummyVars(NB_VARIANTES, false);
            // On lance une partie factice qui sera immédiatement écrasée
            Partie& partie = Partie::get_instance(true, dummyVars, "JoueurTemp", "facile");

            // 3. On écrase l'état avec la sauvegarde
            partie.retablirMemento(m);

            // 4. On lance le jeu
            partie.jouer();

            return 0; // Fin du programme après la partie chargée
        }
        catch (std::exception& e) {
            std::cerr << "/!\\ Erreur de chargement : " << e.what() << std::endl;
            std::cout << "Lancement d'une nouvelle partie par defaut...\n\n";
        }
    }

        std::vector<bool> variantes;
        variantes.resize(NB_VARIANTES, false);

        char reponse;
        bool solo = true;

        std::cout << "=== AKROPOLIS ===\n\n";

        // Mode de jeu
        std::cout << "Souhaitez-vous jouer en mode solo ? (o/n) : ";
        std::cin >> reponse;
        solo = (reponse == 'o' || reponse == 'O');

        std::cin.ignore(); // nettoyage buffer

        if (!solo) {
            // MULTI
            int nb_joueurs;
            std::vector<std::string> noms;

            std::cout << "\nNombre de joueurs : ";
            std::cin >> nb_joueurs;
            std::cin.ignore();

            for (int i = 0; i < nb_joueurs; i++) {
                std::string nom;
                std::cout << "Nom du joueur " << (i + 1) << " : ";
                std::getline(std::cin, nom);
                noms.push_back(nom);
            }

            // Variantes
            std::cout << "\nActivation des variantes (o = oui, n = non)\n";

            char reponse;

            for (std::size_t i = 0; i < NB_VARIANTES; ++i) {
                Variante v = static_cast<Variante>(i);

                std::cout << "Variante " << (i + 1)
                        << " (" << to_string(v) << ") ? (o/n) : ";

                std::cin >> reponse;
                variantes[i] = (reponse == 'o' || reponse == 'O');
            }

            Partie& partie = Partie::get_instance(
                nb_joueurs,
                true,
                variantes,
                noms
            );

            partie.jouer();
        }
        else {
            // SOLO
            std::string nom;
            std::string difficulte;

            std::cout << "\nNom du joueur : ";
            std::getline(std::cin, nom);

            std::cout << "\nChoisissez la difficulte de l'IA :\n";
            std::cout << "1 - Facile\n";
            std::cout << "2 - Moyen\n";
            std::cout << "3 - Difficile\n";
            std::cout << "> ";

            std::cin >> reponse;

            switch (reponse) {
            case '1': difficulte = "facile"; break;
            case '2': difficulte = "moyen"; break;
            case '3': difficulte = "difficile"; break;
            default: difficulte = "moyen";
            }

            // Variantes
            std::cout << "\nActivation des variantes (o = oui, n = non)\n";

            char reponse;

            for (std::size_t i = 0; i < NB_VARIANTES; ++i) {
                Variante v = static_cast<Variante>(i);

                std::cout << "Variante " << (i + 1)
                        << " (" << to_string(v) << ") ? (o/n) : ";

                std::cin >> reponse;
                variantes[i] = (reponse == 'o' || reponse == 'O');
            }

            Partie& partie = Partie::get_instance(
                true,
                variantes,
                nom,
                difficulte
            );

            partie.jouer();
        }

        return 0;
    }

