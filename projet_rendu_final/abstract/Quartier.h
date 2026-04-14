#pragma once
#include "Construction.h"
#include <vector>

class Quartier : public Construction {
protected:
	virtual std::string get_type_construction() const  = 0;
	virtual void afficher_construction() const = 0;
public:
	using Construction::Construction;
    virtual bool est_valide(const std::vector<Construction*>& voisins) const = 0;
	virtual bool est_valide_variante(const std::vector<Construction*>& voisins, const int etage) const = 0;
	virtual ~Quartier() = default;	
};
