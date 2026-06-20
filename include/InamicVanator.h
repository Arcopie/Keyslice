#pragma once
#include "EntitateJoc.h"

// inamic care urmareste activ jucatorul (Pattern Strategy: StrategieUrmarire).
// simbol 'V', apare dupa scor >= 150, eliminat dintr-o lovitura.
class InamicVanator : public EntitateJoc {
    char simbol;
public:
    explicit InamicVanator(int id, const Pozitie& poz, char simbol = 'V');
    InamicVanator(const InamicVanator& e);
    InamicVanator& operator=(InamicVanator e); // copy-and-swap

    bool aplicaLovitura() override;
    [[nodiscard]] bool esteActiv() const override;
    [[nodiscard]] bool esteLA(const Pozitie& p) const override;
    [[nodiscard]] char getSimbol() const override;
    [[nodiscard]] [[maybe_unused]] std::unique_ptr<EntitateJoc> clone() const override;

    friend void swap(InamicVanator& a, InamicVanator& b) noexcept;
protected:
    void afiseazaIntern(std::ostream& os) const override;
};
