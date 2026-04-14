#pragma once
#include <cstddef>
#include "../abstract/Construction.h"

enum class Variante : std::size_t {
    Habitation = 0,
    Marche = 1,
    Caserne = 2,
    Temple = 3,
    Jardin = 4,
    Count
};

constexpr std::size_t NB_VARIANTES = static_cast<std::size_t>(Variante::Count);
constexpr char VARIANTE_ABREVIATIONS[] = "HMCTJ";


constexpr std::size_t idx(Variante v) {
    return static_cast<std::size_t>(v);
}


// Conversion vers un nom lisible
inline const char* to_string(Variante v) {
    switch (v) {
        case Variante::Habitation: return "Habitation";
        case Variante::Marche:     return "Marche";
        case Variante::Caserne:    return "Caserne";
        case Variante::Temple:     return "Temple";
        case Variante::Jardin:     return "Jardin";
        default:                   return "Inconnue";
    }
}

inline void string_to_variante(const std::string& t, Variante& out) {
    if (t == "Habitation" || t == "bleu") { out = Variante::Habitation; }
    else if (t == "Marche" || t == "jaune") { out = Variante::Marche; }
    else if (t == "Caserne" || t == "rouge") { out = Variante::Caserne; }
    else if (t == "Temple" || t == "violet") { out = Variante::Temple; }
    else if (t == "Jardin" || t == "vert") { out = Variante::Jardin; }

    else { out = Variante::Count; }
}
