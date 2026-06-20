#pragma once
#include "Observer.h"
#include <chrono>
#include <string>

// Observer concret #3: asculta toate evenimentele si pastreaza starea afisata
// (scor, runda, power-up activ, banner). Nu depinde de SFML - renderer-ul
// citeste aceste date si le deseneaza (asa ramane functionala si varianta CI).
class AfisajHUD : public IObserver {
    int scor;
    int runda;
    int totalOmorati;
    std::string powerUpActiv;
    std::string banner;
    std::chrono::steady_clock::time_point bannerPanaLa;
public:
    AfisajHUD();
    void onEveniment(const std::string& tip, int valoare) override;

    [[nodiscard]] int getScor() const;
    [[nodiscard]] int getRunda() const;
    [[nodiscard]] int getTotalOmorati() const;
    [[nodiscard]] const std::string& getPowerUpActiv() const;
    [[nodiscard]] std::string getBanner() const; // "" daca a expirat
};
