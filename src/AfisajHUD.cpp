#include "../include/AfisajHUD.h"
#include "../include/PowerUp.h"

AfisajHUD::AfisajHUD()
    : scor(0), runda(1), totalOmorati(0),
      bannerPanaLa(std::chrono::steady_clock::now()) {}

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
        powerUpActiv = numePowerUp(static_cast<TipPowerUp>(valoare));
    } else if (tip == "GAME_OVER") {
        scor = valoare;
        powerUpActiv.clear();
    }
}

int AfisajHUD::getScor() const { return scor; }
int AfisajHUD::getRunda() const { return runda; }
int AfisajHUD::getTotalOmorati() const { return totalOmorati; }
const std::string& AfisajHUD::getPowerUpActiv() const { return powerUpActiv; }

std::string AfisajHUD::getBanner() const {
    if (std::chrono::steady_clock::now() < bannerPanaLa)
        return banner;
    return "";
}
