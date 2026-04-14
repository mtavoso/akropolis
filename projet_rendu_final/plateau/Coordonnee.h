#pragma once
#include <vector>

class Coordonnee {
private:
    int x;
    int y;

public:
    Coordonnee(int x_, int y_) : x(x_), y(y_) {}

    int getX() const { return x; }
    int getY() const { return y; }

    // On crée une relation d'ordre pour qu'une Coordonnée puisse être utilisée comme clé dans une map :
    bool operator<(const Coordonnee& other) const {
        if (x < other.getX()) return true;
        if (x > other.getX()) return false;
        return y < other.getY();
    }
    bool operator==(const Coordonnee& other) const{
		return (x == other.getX()) && (y == other.getY());
	}

    // Renvoi les 6 coordonnées voisines :
    std::vector<Coordonnee> get_voisines() const;

    // Vérifie si une autre coordonée est sa voisine :
    bool est_voisine(const Coordonnee& c) const;
	~Coordonnee() = default;
};

// Vérifie si 3 coordonnées sont des voisines :
bool trio_coordonnees_voisines(const Coordonnee& c1, const Coordonnee& c2, const Coordonnee& c3) ;
