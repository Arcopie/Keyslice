#include "../include/InamicRapid.h"
#include <utility>

InamicRapid::InamicRapid(int id, const Pozitie& poz, char simbol)
    : EntitateJoc(id, poz, "InamicRapid"), simbol(simbol) {
    // miscare diagonala aleatoare (Pattern Strategy)
    setStrategie(std::make_shared<StrategieDiagonala>());
}

InamicRapid::InamicRapid(const InamicRapid& e)
    : EntitateJoc(e), simbol(e.simbol) {}

void swap(InamicRapid& a, InamicRapid& b) noexcept {
    using std::swap;
    swap(static_cast<EntitateJoc&>(a), static_cast<EntitateJoc&>(b));
    swap(a.simbol, b.simbol);
}

InamicRapid& InamicRapid::operator=(InamicRapid e) {
    swap(*this, e);
    return *this;
}

bool InamicRapid::aplicaLovitura() {
    if (!entitate.esteInViata()) return false;
    entitate.setInViata(false);
    return true;
}

bool InamicRapid::esteActiv() const { return entitate.esteInViata(); }

bool InamicRapid::esteLA(const Pozitie& p) const {
    return entitate.esteInViata() && entitate.getPoz() == p;
}

char InamicRapid::getSimbol() const { return simbol; }

std::unique_ptr<EntitateJoc> InamicRapid::clone() const {
    return std::make_unique<InamicRapid>(*this);
}

void InamicRapid::afiseazaIntern(std::ostream& os) const {
    os << "[" << simbol << "*] " << entitate;
}
