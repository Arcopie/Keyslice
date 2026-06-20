#include "../include/GestorPowerUp.h"
#include "../include/Joc.h"
#include "../include/PowerUp.h"

GestorPowerUp::GestorPowerUp(Joc* joc) : joc(joc), praguriAtinse(0) {}

void GestorPowerUp::onEveniment(const std::string& tip, int valoare) {
    if (tip == "INAMIC_OMORAT") {
        // la fiecare 5 kills (cumulati) spawneaza un power-up pe harta
        while (valoare / 5 > praguriAtinse) {
            praguriAtinse++;
            joc->spawnPowerUp();
        }
    } else if (tip == "POWERUP_COLECTAT") {
        // valoare = TipPowerUp serializat
        joc->activeazaEfectPowerUp(static_cast<TipPowerUp>(valoare));
    }
}
