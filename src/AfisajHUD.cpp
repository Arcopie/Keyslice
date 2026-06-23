#include "../include/AfisajHUD.h"
#include "../include/PowerUp.h"

AfisajHUD::AfisajHUD()
    : scor(0), runda(1), totalOmorati(0),
      bannerPanaLa(std::chrono::steady_clock::now()),
      powerUpPanaLa(std::chrono::steady_clock::now()),
      powerUpAreDurata(false) {}

void AfisajHUD::onEveniment(const std::string& tip, int valoare) {
    if (tip == "SCOR_SCHIMBAT") {
        scor = valoare;
    } else if (tip == "INAMIC_OMORAT") {
        totalOmorati = valoare;
    } else if (tip == "RUNDA_COMPLETA") {
        runda = valoare;
        banner = "RUNDA " + std::to_string(runda) + "!";
        bannerPanaLa = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    } else if (tip == "POWERUP_COLECTAT") {
        auto t = static_cast<TipPowerUp>(valoare);
        powerUpActiv = numePowerUp(t);
        switch (t) {
            case TipPowerUp::INGHET:
                powerUpAreDurata = true;
                powerUpPanaLa = std::chrono::steady_clock::now() + std::chrono::seconds(3);
                break;
            case TipPowerUp::DUBLU_SCOR:
                powerUpAreDurata = true;
                powerUpPanaLa = std::chrono::steady_clock::now() + std::chrono::seconds(5);
                break;
            case TipPowerUp::SCUT:
                powerUpAreDurata = false; // se sterge la evenimentul SCUT_CONSUMAT
                break;
        }
    } else if (tip == "SCUT_CONSUMAT") {
        powerUpActiv.clear();
    } else if (tip == "GAME_OVER") {
        scor = valoare;
        powerUpActiv.clear();
    }
}

int AfisajHUD::getScor() const { return scor; }
int AfisajHUD::getRunda() const { return runda; }
// cppcheck-suppress unusedFunction
int AfisajHUD::getTotalOmorati() const { return totalOmorati; }

// cppcheck-suppress unusedFunction
std::string AfisajHUD::getPowerUpActiv() const {
    if (powerUpActiv.empty()) return "";
    // SCUT nu are durata - ramane afisat pana la SCUT_CONSUMAT
    if (!powerUpAreDurata) return powerUpActiv;
    // INGHET / DUBLU_SCOR: verifica daca efectul mai e activ
    if (std::chrono::steady_clock::now() < powerUpPanaLa)
        return powerUpActiv;
    return "";
}

// cppcheck-suppress unusedFunction
std::string AfisajHUD::getBanner() const {
    if (std::chrono::steady_clock::now() < bannerPanaLa)
        return banner;
    return "";
}
