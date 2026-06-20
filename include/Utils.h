#pragma once
#include "EntitateJoc.h"
#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>

void sleepMs(int ms);
void clearScreen();
bool tastaDisponibila();
int citesteTasta();

// ---- functii sablon (>=2 instantieri fiecare) ----

// filtreaza un vector dupa un predicat; returneaza un vector nou.
// instantiata cu shared_ptr<EntitateJoc> (in Joc) si std::string (in GestorRunde)
template <typename T, typename Predicat>
std::vector<T> filtreaza(const std::vector<T>& sursa, Predicat pred) {
    std::vector<T> rezultat;
    std::copy_if(sursa.begin(), sursa.end(), std::back_inserter(rezultat), pred);
    return rezultat;
}

// numara entitatile active de un anumit tip derivat (RTTI).
// inlocuieste dynamic_cast-urile manuale; instantiata cu InamicNormal,
// InamicVanator etc.
template <typename TDeriv>
int numaraDeTyp(const std::vector<std::shared_ptr<EntitateJoc>>& entitati) {
    return static_cast<int>(std::count_if(
        entitati.begin(), entitati.end(),
        [](const std::shared_ptr<EntitateJoc>& e) {
            return dynamic_cast<TDeriv*>(e.get()) != nullptr && e->esteActiv();
        }));
}
