#pragma once
#include "../abstract/Construction.h"
#include <string>
#include<iostream>

class Carriere : public Construction  {
public:
	std::string get_type_construction() const override {return "carriere"; }
	void afficher_construction() const override { std::cout << "K ";return; }
	using Construction::Construction;
	~Carriere() = default;


};
