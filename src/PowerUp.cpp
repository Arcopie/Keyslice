#include "../include/PowerUp.h"

const char* numePowerUp(TipPowerUp tip) {
    switch (tip) {
        case TipPowerUp::INGHET:     return "Inghet";
        case TipPowerUp::SCUT:       return "Scut";
        case TipPowerUp::DUBLU_SCOR: return "Dublu scor";
    }
    return "?";
}
