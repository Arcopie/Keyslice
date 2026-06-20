#pragma once
#include <memory>
#include <string>
#include <vector>

// Pattern OBSERVER - decupleaza emitatorul de evenimente (Joc) de componentele
// care reactioneaza (gestori de runde / power-up / HUD).

// interfata observatorului
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onEveniment(const std::string& tip, int valoare) = 0;
};

// mixin: orice clasa care vrea sa emita evenimente mosteneste ISubiect
class ISubiect {
    std::vector<std::shared_ptr<IObserver>> observatori;

public:
    virtual ~ISubiect() = default;

    void adaugaObserver(std::shared_ptr<IObserver> obs);
    void eliminaObserver(const std::shared_ptr<IObserver>& obs);

protected:
    void notifica(const std::string& tip, int valoare = 0);
};
