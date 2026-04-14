#pragma once
#include <string>

class Memento {
private:
    std::string etat;

public:
    Memento(const std::string& state) : etat(state) {}
    std::string getEtat() const { return etat; }
};