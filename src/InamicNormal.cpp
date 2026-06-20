#include "../include/InamicNormal.h"
#include <utility>

InamicNormal::InamicNormal(int id, const Pozitie& poz, char simbol)
    : EntitateJoc(id, poz, "InamicNormal"), simbol(simbol) {
    // miscare ortogonala aleatoare (Pattern Strategy)
    setStrategie(std::make_shared<StrategieOrthogonala>());
}

InamicNormal::InamicNormal(const InamicNormal& e)
    : EntitateJoc(e), simbol(e.simbol) {}

void swap(InamicNormal& a, InamicNormal& b) noexcept {
    using std::swap;
    swap(static_cast<EntitateJoc&>(a), static_cast<EntitateJoc&>(b));
    swap(a.simbol, b.simbol);
}

InamicNormal& InamicNormal::operator=(InamicNormal e) {
    swap(*this, e);
    return *this;
}

bool InamicNormal::aplicaLovitura() {
    if (!entitate.esteInViata()) return false;
    entitate.setInViata(false);
    return true;
}

bool InamicNormal::esteActiv() const { return entitate.esteInViata(); }

bool InamicNormal::esteLA(const Pozitie& p) const {
    return entitate.esteInViata() && entitate.getPoz() == p;
}

char InamicNormal::getSimbol() const { return simbol; }

std::unique_ptr<EntitateJoc> InamicNormal::clone() const {
    return std::make_unique<InamicNormal>(*this);
}

void InamicNormal::afiseazaIntern(std::ostream& os) const {
    os << "[" << simbol << "] " << entitate;
}
