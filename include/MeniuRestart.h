#pragma once
#include "Clasament.h"
#include "Joc.h"
#include <ostream>

class MeniuRestart {
    int nrRestartari;
    int vieti;       // vieti ramase (incepe de la 3)
    int scorTotal;   // scorul adunat de la toate rundele

    // clasa sablon instantiata cu doua tipuri (int si double)
    Clasament<int> clasamentScoruri{5};   // top 5 scoruri
    Clasament<double> clasamentTimpi{5};  // top 5 timpi (secunde)

    static void afiseazaOptiuni(int vietiRamase);
    bool asteaptaDecizie();
public:
    MeniuRestart();
    bool gestioneazaJoc(Joc& joc);

    // inregistreaza o runda terminata (bucla SFML): aduna scorul si scade o viata
    [[maybe_unused]] void inregistreazaRunda(int scorRunda, double durataSec = 0.0);

    [[nodiscard]] int getVieti() const;
    [[nodiscard]] int getScorTotal() const;
    [[nodiscard, maybe_unused]] const Clasament<int>& getClasamentScoruri() const;
    [[nodiscard, maybe_unused]] const Clasament<double>& getClasamentTimpi() const;

    friend std::ostream& operator<<(std::ostream& os, const MeniuRestart& m);
};
