#include "../include/MiniBoss.h"
#include <utility>

MiniBoss::MiniBoss(int id, const Pozitie& poz, int vietiMaxime)
    : EntitateJoc(id, poz, "MiniBoss"),
      vietiRamase(vietiMaxime), vietiMaxime(vietiMaxime) {
    if (vietiMaxime < 1)
        throw ExceptieEntitateInvalida("vietiMaxime trebuie sa fie >= 1");
    // miscare ortogonala aleatoare (Pattern Strategy)
    setStrategie(std::make_shared<StrategieOrthogonala>());
}

MiniBoss::MiniBoss(const MiniBoss& e)
    : EntitateJoc(e), vietiRamase(e.vietiRamase), vietiMaxime(e.vietiMaxime) {}

void swap(MiniBoss& a, MiniBoss& b) noexcept {
    using std::swap;
    swap(static_cast<EntitateJoc&>(a), static_cast<EntitateJoc&>(b));
    swap(a.vietiRamase, b.vietiRamase);
    swap(a.vietiMaxime, b.vietiMaxime);
}

MiniBoss& MiniBoss::operator=(MiniBoss e) {
    swap(*this, e);
    return *this;
}

// decrementeaza vietiRamase; returneaza true cand moare
bool MiniBoss::aplicaLovitura() {
    if (vietiRamase <= 0) return false;
    --vietiRamase;
    return vietiRamase == 0;
}

int MiniBoss::getScorKill() const { return 30; }

bool MiniBoss::esteActiv() const { return vietiRamase > 0; }

bool MiniBoss::esteLA(const Pozitie& p) const {
    return vietiRamase > 0 && entitate.getPoz() == p;
}

char MiniBoss::getSimbol() const { return 'B'; }

std::unique_ptr<EntitateJoc> MiniBoss::clone() const {
    return std::make_unique<MiniBoss>(*this);
}

int MiniBoss::getVietiRamase() const { return vietiRamase; }
int MiniBoss::getVietiMaxime() const { return vietiMaxime; }

void MiniBoss::afiseazaIntern(std::ostream& os) const {
    os << "[B] " << entitate
       << " vieti: " << vietiRamase << "/" << vietiMaxime;
}
