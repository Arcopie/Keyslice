#include "../include/StrategieMiscare.h"
#include <cstdlib>

namespace {
// limiteaza o coordonata in intervalul [0, limita)
int clamp(int v, int limita) {
    if (v < 0) return 0;
    if (v >= limita) return limita - 1;
    return v;
}
} // namespace

// ---- StrategieOrthogonala ----
Pozitie StrategieOrthogonala::calculeazaMutare(const Pozitie& curent, const Pozitie&,
                                               int nrLinii, int nrColoane) const {
    const int directii[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int d = rand() % 4;
    int lin = clamp(curent.getLin() + directii[d][0], nrLinii);
    int col = clamp(curent.getCol() + directii[d][1], nrColoane);
    return Pozitie(lin, col);
}
std::string StrategieOrthogonala::numeStrategie() const { return "Ortogonala"; }

// ---- StrategieDiagonala ----
Pozitie StrategieDiagonala::calculeazaMutare(const Pozitie& curent, const Pozitie&,
                                             int nrLinii, int nrColoane) const {
    const int directii[][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    int d = rand() % 4;
    int lin = clamp(curent.getLin() + directii[d][0], nrLinii);
    int col = clamp(curent.getCol() + directii[d][1], nrColoane);
    return Pozitie(lin, col);
}
std::string StrategieDiagonala::numeStrategie() const { return "Diagonala"; }

// ---- StrategieUrmarire ----
// se apropie cu un pas pe axa cu cea mai mare distanta fata de jucator
Pozitie StrategieUrmarire::calculeazaMutare(const Pozitie& curent, const Pozitie& jucator,
                                            int nrLinii, int nrColoane) const {
    int lin = curent.getLin();
    int col = curent.getCol();
    int dlin = jucator.getLin() - lin;
    int dcol = jucator.getCol() - col;

    if (std::abs(dlin) >= std::abs(dcol)) {
        if (dlin != 0)
            lin += (dlin > 0 ? 1 : -1);
        else if (dcol != 0)
            col += (dcol > 0 ? 1 : -1);
    } else {
        col += (dcol > 0 ? 1 : -1);
    }
    return Pozitie(clamp(lin, nrLinii), clamp(col, nrColoane));
}
std::string StrategieUrmarire::numeStrategie() const { return "Urmarire"; }

// ---- StrategieZigzag ----
// merge dreapta pe randurile pare, stanga pe cele impare; la capat coboara
Pozitie StrategieZigzag::calculeazaMutare(const Pozitie& curent, const Pozitie&,
                                          int nrLinii, int nrColoane) const {
    int lin = curent.getLin();
    int col = curent.getCol();
    if (lin % 2 == 0)
        col += 1;
    else
        col -= 1;

    if (col >= nrColoane) {
        col = nrColoane - 1;
        lin += 1;
    } else if (col < 0) {
        col = 0;
        lin += 1;
    }
    if (lin >= nrLinii)
        lin = 0;
    return Pozitie(clamp(lin, nrLinii), clamp(col, nrColoane));
}
std::string StrategieZigzag::numeStrategie() const { return "Zigzag"; }
