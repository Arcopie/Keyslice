#include "../include/Observer.h"
#include <algorithm>
#include <utility>

void ISubiect::adaugaObserver(std::shared_ptr<IObserver> obs) {
    if (obs)
        observatori.push_back(std::move(obs));
}

void ISubiect::eliminaObserver(const std::shared_ptr<IObserver>& obs) {
    observatori.erase(std::remove(observatori.begin(), observatori.end(), obs),
                      observatori.end());
}

void ISubiect::notifica(const std::string& tip, int valoare) {
    for (const auto& o : observatori)
        if (o)
            o->onEveniment(tip, valoare);
}
