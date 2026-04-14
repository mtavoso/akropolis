# Akropolis

Implémentation numérique du jeu de plateau Akropolis, réalisée dans le cadre de l'UV LO21 à l'UTC (Université de Technologie de Compiègne).

## Le jeu

Akropolis est un jeu de plateau dans lequel chaque joueur construit sa propre cité grecque en posant des tuiles hexagonales. Les tuiles sont composées de trois constructions : des carrières (ressources), des places (multiplicateurs) et des quartiers (points). Il existe cinq types de quartiers — habitations, marchés, temples, casernes et jardins — chacun avec ses propres règles de scoring. Le placement vertical est possible sous certaines conditions.

## Aperçu

<img width="956" height="610" alt="image" src="https://github.com/user-attachments/assets/3ac6cea8-2844-4f54-9d7a-5264180aeebe" />


## Fonctionnalités

- Jeu multijoueur (2 à 4 joueurs)
- Mode solo contre une IA avec trois niveaux de difficulté (facile, moyen, difficile)
- Cinq variantes de jeu offrant des bonus de points conditionnels
- Sauvegarde et reprise de partie
- Interface console en ASCII
- Interface graphique Qt avec prévisualisation des coups en temps réel

## Architecture

Le projet est développé en C++ avec une conception orientée objet. Les principales classes sont `Partie` (singleton), `Joueur`, `Chantier`, `Cité`, `Tuile` et `Construction` (classe abstraite dont héritent `Carrière`, `Place` et les cinq types de quartiers).

Trois design patterns sont utilisés : Singleton pour `Partie`, Strategy pour les niveaux de difficulté de l'IA, et Memento pour le système de sauvegarde.

L'interface Qt est composée de vues dédiées : `VuePartie`, `VuePlateau`, `VueChantier`, `VueScore` et `VueMenu`.

## Contribution personnelle

- Choix d'architecture
- Développement du mode solo (IA)
- Implémentation des classes Partie, Joueur, Cité et supervisation globale du projet


## Compilation

Prérequis : C++17, Qt 5 ou 6, CMake.

```bash
mkdir build && cd build
cmake ..
make
```

## Équipe

Paul Gaillot, Iyed Chtioui, Ryad Meziane, Matys Tavoso, Gaspar Oliveira.
