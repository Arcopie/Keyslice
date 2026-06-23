#pragma once
#include "Entitate.h"
#include "Pozitie.h"
#include "StrategieMiscare.h"
#include <memory>
#include <ostream>

// clasa de baza abstracta pentru entitatile din joc
class EntitateJoc {
    static int contor; // nr total de entitati active
protected:
    Entitate entitate;
    std::shared_ptr<StrategieMiscare> strategie; // Pattern Strategy (poate fi null)

    EntitateJoc(int id, const Pozitie& poz, const char* tip);
    EntitateJoc(const EntitateJoc& e); // protejat - folosit de derivate
    friend void swap(EntitateJoc& a, EntitateJoc& b) noexcept;

public:
    virtual ~EntitateJoc();
    EntitateJoc& operator=(const EntitateJoc&) = delete;

    // functie virtuala pura specifica temei - incearca sa omoare entitatea
    virtual bool aplicaLovitura() = 0;
    // punctele acordate la omorierea acestei entitati (implicit 10, MiniBoss 30)
    [[nodiscard]] virtual int getScorKill() const;
    [[nodiscard]] virtual bool esteActiv() const = 0;
    [[nodiscard]] virtual bool esteLA(const Pozitie& p) const = 0;
    [[nodiscard]] virtual char getSimbol() const = 0;
    // constructor virtual (clone) - pastrat ca parte din ierarhia OOP, fara apelanti directi
    [[nodiscard]] [[maybe_unused]] virtual std::unique_ptr<EntitateJoc> clone() const = 0;

    // deplasare delegata strategiei (Pattern Strategy); non-virtuala
    void muta(const Pozitie& pozJucator, int nrLinii, int nrColoane);
    void setStrategie(std::shared_ptr<StrategieMiscare> s);

    [[nodiscard]] const Pozitie& getPoz() const;
    static int getContor();

    // NVI: operator<< apeleaza afiseazaIntern virtual
    friend std::ostream& operator<<(std::ostream& os, const EntitateJoc& e);

protected:
    virtual void afiseazaIntern(std::ostream& os) const = 0;
};

void swap(EntitateJoc& a, EntitateJoc& b) noexcept;
