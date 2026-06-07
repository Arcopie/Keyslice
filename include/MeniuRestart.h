#pragma once
#include "Joc.h"
#include <ostream>

class MeniuRestart {
    int nrRestartari;
    int vieti;       // vieti ramase (incepe de la 3)
    int scorTotal;   // scorul adunat de la toate rundele

    static void afiseazaOptiuni(int vietiRamase);
    bool asteaptaDecizie();
public:
    MeniuRestart();
    bool gestioneazaJoc(Joc& joc);

    // inregistreaza o runda terminata (bucla SFML): aduna scorul si scade o viata
    void inregistreazaRunda(int scorRunda);

    [[nodiscard]] int getVieti() const;
    [[nodiscard]] int getScorTotal() const;

    friend std::ostream& operator<<(std::ostream& os, const MeniuRestart& m);
};
