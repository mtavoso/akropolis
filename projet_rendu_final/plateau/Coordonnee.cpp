#include "Coordonnee.h"
#include <vector>

// Renvoie les 6 coordonnées voisines d'un hexagone

std::vector<Coordonnee> Coordonnee::get_voisines() const
{
    std::vector<Coordonnee> v;
    v.reserve(6);

    if (x % 2 == 0)  // colonne paire
    {
        v.emplace_back(x + 1, y); 
        v.emplace_back(x, y + 1);
        v.emplace_back(x - 1, y); 
        v.emplace_back(x + 1, y - 1);
        v.emplace_back(x, y - 1);
        v.emplace_back(x - 1, y - 1); 
    }
    else             // colonne impaire
    {
        v.emplace_back(x + 1, y); // droite
        v.emplace_back(x, y + 1); // haut
        v.emplace_back(x - 1, y); // gauche
        v.emplace_back(x + 1, y + 1); // haut-droite
        v.emplace_back(x, y - 1); // bas
        v.emplace_back(x - 1, y + 1); // haut-gauche
    }

    return v;
}


// Vérifie si une autre coordonnée est voisine immédiate

bool Coordonnee::est_voisine(const Coordonnee& c) const
{
    int dx = c.getX() - x;
    int dy = c.getY() - y;

    if (x % 2 == 0) 
    {
        return
            (dx == 1 && dy == 0) ||   // droite
            (dx == 0 && dy == 1) ||   // haut
            (dx == -1 && dy == 0) ||  // gauche
            (dx == 1 && dy == -1) ||  // bas-droite
            (dx == 0 && dy == -1) ||  // bas
            (dx == -1 && dy == -1);   // bas-gauche
    }
    else 
    {
        return
            (dx == 1 && dy == 0) ||   
            (dx == 0 && dy == 1) ||   
            (dx == -1 && dy == 0) ||  
            (dx == 1 && dy == 1) ||   
            (dx == 0 && dy == -1) ||  
            (dx == -1 && dy == 1);    
    }
}


// Vérifie si les trois coordonnées forment un triangle d'hexagones,
// cad chaque coordonnée touche les deux autres.
bool trio_coordonnees_voisines(const Coordonnee& c1,
    const Coordonnee& c2,
    const Coordonnee& c3)
{
    return
        c1.est_voisine(c2) &&
        c1.est_voisine(c3) &&
        c2.est_voisine(c3);
}
