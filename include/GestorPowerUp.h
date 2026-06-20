#pragma once
#include "Observer.h"

class Joc;

// Observer concret #2: spawneaza power-up-uri la fiecare 5 kills si activeaza
// efectul cand jucatorul colecteaza un '*'.
class GestorPowerUp : public IObserver {
    Joc* joc; // back-pointer ne-detinator
    int praguriAtinse; // cate praguri de 5 kills au fost deja procesate
public:
    explicit GestorPowerUp(Joc* joc);
    void onEveniment(const std::string& tip, int valoare) override;
};
