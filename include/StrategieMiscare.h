#pragma once
#include "Pozitie.h"
#include <string>

// Pattern STRATEGY - extrage logica de miscare a inamicilor in clase separate.
// Fiecare inamic detine un shared_ptr<StrategieMiscare> (vezi EntitateJoc).

// clasa de baza abstracta
class StrategieMiscare {
public:
    virtual ~StrategieMiscare() = default;
    // calculeaza noua pozitie pornind de la cea curenta; `jucator` e folosit
    // doar de strategiile care urmaresc jucatorul
    [[nodiscard]] virtual Pozitie calculeazaMutare(
        const Pozitie& curent, const Pozitie& jucator,
        int nrLinii, int nrColoane) const = 0;
    [[nodiscard]] virtual std::string numeStrategie() const = 0;
};

// strategia 1 - ortogonala (sus/jos/stanga/dreapta aleator)
class StrategieOrthogonala : public StrategieMiscare {
public:
    [[nodiscard]] Pozitie calculeazaMutare(const Pozitie& curent, const Pozitie& jucator,
                                           int nrLinii, int nrColoane) const override;
    [[nodiscard]] std::string numeStrategie() const override;
};

// strategia 2 - diagonala aleatoare
class StrategieDiagonala : public StrategieMiscare {
public:
    [[nodiscard]] Pozitie calculeazaMutare(const Pozitie& curent, const Pozitie& jucator,
                                           int nrLinii, int nrColoane) const override;
    [[nodiscard]] std::string numeStrategie() const override;
};

// strategia 3 - urmareste jucatorul pe linie sau coloana
class StrategieUrmarire : public StrategieMiscare {
public:
    [[nodiscard]] Pozitie calculeazaMutare(const Pozitie& curent, const Pozitie& jucator,
                                           int nrLinii, int nrColoane) const override;
    [[nodiscard]] std::string numeStrategie() const override;
};

// strategia 4 - zigzag (alternare stanga-dreapta pe linie)
class StrategieZigzag : public StrategieMiscare {
public:
    [[nodiscard]] Pozitie calculeazaMutare(const Pozitie& curent, const Pozitie& jucator,
                                           int nrLinii, int nrColoane) const override;
    [[nodiscard]] std::string numeStrategie() const override;
};
