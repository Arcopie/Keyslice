#pragma once
#include "Pozitie.h"

// tipurile de power-up colectabile pe harta (simbol '*')
enum class TipPowerUp { INGHET, SCUT, DUBLU_SCOR };

struct PowerUp {
    Pozitie pozitie;
    TipPowerUp tip;
    bool activ = true;

    PowerUp(const Pozitie& poz, TipPowerUp tip) : pozitie(poz), tip(tip) {}
    [[nodiscard]] char getSimbol() const { return '*'; }
};

// numele scurt al efectului (pentru HUD / mesaje)
const char* numePowerUp(TipPowerUp tip);
