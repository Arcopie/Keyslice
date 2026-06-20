#pragma once
#include "Entitate.h"
#include <chrono>
#include <vector>
#include <memory>
#include <ostream>

class EntitateJoc; // forward declaration

class Jucator {
    Entitate entitate;
    int scor;
    int nrSliceuri;
    bool scutActiv;                                       // power-up SCUT
    std::chrono::steady_clock::time_point dubluScorPanaLa; // power-up DUBLU_SCOR
public:
    explicit Jucator(const Pozitie& start);

    void teleport(const Pozitie& p);

    // ---- power-ups ----
    void activeazaScut();
    [[nodiscard]] bool areScut() const;
    bool consumaScut(); // foloseste scutul daca exista; true daca a absorbit lovitura
    void activeazaDubluScor(double secunde);
    [[nodiscard]] bool areDubluScor() const;
    [[nodiscard]] int getMultiplicatorScor() const; // 2 cat timp dublu scor e activ

    // slice pe traseul intre pozVeche si pozitia curenta; returneaza nr omorati
    int slice(std::vector<std::shared_ptr<EntitateJoc>>& entitati,
              const Pozitie& pozVeche);

    // verifica daca vreo entitate activa se afla pe pozitia jucatorului
    [[nodiscard]] bool atingeEntitate(
        const std::vector<std::shared_ptr<EntitateJoc>>& entitati) const;

    [[nodiscard]] int getScor() const;
    [[nodiscard]] int getSliceuri() const;
    [[nodiscard]] const Pozitie& getPoz() const;

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);
};
