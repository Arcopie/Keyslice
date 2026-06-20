#pragma once
#include "Observer.h"

class Joc;

// Observer concret #1: reactioneaza la "RUNDA_COMPLETA" si creste dificultatea
// (viteza inamicilor, numarul maxim de inamici, tema de culoare).
class GestorRunde : public IObserver {
    Joc* joc; // back-pointer ne-detinator (Joc detine acest observer)
    int runda;
public:
    explicit GestorRunde(Joc* joc);
    void onEveniment(const std::string& tip, int valoare) override;
    [[nodiscard]] int getRunda() const;
};
