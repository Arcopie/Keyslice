#include "../include/InamicVanator.h"
#include <utility>

InamicVanator::InamicVanator(int id, const Pozitie& poz, char simbol)
    : EntitateJoc(id, poz, "InamicVanator"), simbol(simbol) {
    // urmareste jucatorul pe linie sau coloana (Pattern Strategy)
    setStrategie(std::make_shared<StrategieUrmarire>());
}

InamicVanator::InamicVanator(const InamicVanator& e)
    : EntitateJoc(e), simbol(e.simbol) {}

void swap(InamicVanator& a, InamicVanator& b) noexcept {
    using std::swap;
    swap(static_cast<EntitateJoc&>(a), static_cast<EntitateJoc&>(b));
    swap(a.simbol, b.simbol);
}

InamicVanator& InamicVanator::operator=(InamicVanator e) {
    swap(*this, e);
    return *this;
}

bool InamicVanator::aplicaLovitura() {
    if (!entitate.esteInViata()) return false;
    entitate.setInViata(false);
    return true;
}

bool InamicVanator::esteActiv() const { return entitate.esteInViata(); }

bool InamicVanator::esteLA(const Pozitie& p) const {
    return entitate.esteInViata() && entitate.getPoz() == p;
}

char InamicVanator::getSimbol() const { return simbol; }

std::unique_ptr<EntitateJoc> InamicVanator::clone() const {
    return std::make_unique<InamicVanator>(*this);
}

void InamicVanator::afiseazaIntern(std::ostream& os) const {
    os << "[" << simbol << "!] " << entitate;
}
