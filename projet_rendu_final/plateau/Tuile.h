#pragma once
#include "../abstract/Construction.h"
#include "./Carriere.h"
#include "./Jardin.h"
#include "./Temple.h"
#include "./Marche.h"
#include "./Caserne.h"
#include "./Habitation.h"
#include "./Place.h"
#include "../reglesconstru.h"


#include <vector>


class Tuile {
private:
    int id_tuile;
    std::vector<Construction*> agencement;
    Construction* creerConstruction(TypeConstruction t);
public:
    void afficher_tuile();
    void rotation_tuile(int nb_rota);
    int get_id() const { return id_tuile; }
    bool operator==(const Tuile& autre) const;
    Construction* get_constru(int rang) { return agencement[rang]; }
    Tuile(int id, TypeConstruction t1, TypeConstruction t2, TypeConstruction t3) :id_tuile(id) 
    {
        agencement.push_back(creerConstruction(t1));
        agencement.push_back(creerConstruction(t2));
        agencement.push_back(creerConstruction(t3));
    };
    Tuile(int id, TypeConstruction t1, TypeConstruction t2, TypeConstruction t3, TypeConstruction t4) :id_tuile(id)
    {
        agencement.push_back(creerConstruction(t1));
        agencement.push_back(creerConstruction(t2));
        agencement.push_back(creerConstruction(t3));
        agencement.push_back(creerConstruction(t4));

    };



    ~Tuile() {
        for (auto c : agencement)
            delete c;
    }
};





