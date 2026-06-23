#include "../include/EntitateJoc.h"
#include <utility>

int EntitateJoc::contor = 0;

int EntitateJoc::getScorKill() const { return 10; }

EntitateJoc::EntitateJoc(int id, const Pozitie& poz, const char* tip)
    : entitate(id, poz, tip) {
    contor++;
}

EntitateJoc::EntitateJoc(const EntitateJoc& e)
    : entitate(e.entitate), strategie(e.strategie) {
    contor++;
}

EntitateJoc::~EntitateJoc() { contor--; }

// swap protejat pentru derivate - schimba entitatea interna si strategia
void swap(EntitateJoc& a, EntitateJoc& b) noexcept {
    using std::swap;
    swap(a.entitate, b.entitate);
    swap(a.strategie, b.strategie);
}

// Pattern Strategy: pozitia noua e calculata de strategia configurata.
// Daca nu exista strategie (ex: inamic stationar), entitatea nu se misca.
void EntitateJoc::muta(const Pozitie& pozJucator, int nrLinii, int nrColoane) {
    if (!esteActiv() || !strategie)
        return;
    entitate.setPoz(
        strategie->calculeazaMutare(entitate.getPoz(), pozJucator, nrLinii, nrColoane));
}

void EntitateJoc::setStrategie(std::shared_ptr<StrategieMiscare> s) {
    strategie = std::move(s);
}

const Pozitie& EntitateJoc::getPoz() const { return entitate.getPoz(); }

int EntitateJoc::getContor() { return contor; }

// NVI: apeleaza afiseazaIntern virtual
std::ostream& operator<<(std::ostream& os, const EntitateJoc& e) {
    e.afiseazaIntern(os);
    return os;
}
