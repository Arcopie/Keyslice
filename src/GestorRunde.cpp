#include "../include/GestorRunde.h"
#include "../include/Joc.h"
#include "../include/Utils.h"
#include <iostream>

GestorRunde::GestorRunde(Joc* joc) : joc(joc), runda(1) {}

void GestorRunde::onEveniment(const std::string& tip, int valoare) {
    if (tip != "RUNDA_COMPLETA")
        return;

    runda = valoare;

    // creste dificultatea in functie de runda
    double intervalMiscare;
    double intervalSpawn;
    int maxInamici;
    unsigned int culoare;
    switch (runda) {
        case 1:  intervalMiscare = 6.0; intervalSpawn = 5.0; maxInamici = 8;  culoare = 0x29b6f6; break; // albastru
        case 2:  intervalMiscare = 4.5; intervalSpawn = 4.0; maxInamici = 10; culoare = 0x66bb6a; break; // verde
        case 3:  intervalMiscare = 3.0; intervalSpawn = 3.0; maxInamici = 12; culoare = 0xffa726; break; // portocaliu
        default: intervalMiscare = 2.0; intervalSpawn = 2.0; maxInamici = 14; culoare = 0xef5350; break; // rosu
    }
    joc->setIntervalMiscare(intervalMiscare);
    joc->setIntervalSpawn(intervalSpawn);
    joc->setMaxInamici(maxInamici);
    joc->setCuloareTema(culoare);

    // functie sablon filtreaza<std::string> - log-ul mesajelor importante
    std::vector<std::string> logMesaje = {
        "BOSS spawnat", "Runda " + std::to_string(runda), "kill normal"};
    auto mesajeImportante = filtreaza<std::string>(
        logMesaje, [](const std::string& s) {
            return s.find("Runda") != std::string::npos ||
                   s.find("BOSS") != std::string::npos;
        });
    for (const auto& m : mesajeImportante)
        std::cout << "[RUNDE] " << m << std::endl;
}

int GestorRunde::getRunda() const { return runda; }
