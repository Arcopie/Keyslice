#include "../include/MeniuRestart.h"
#include "../include/Utils.h"
#include <iostream>

MeniuRestart::MeniuRestart() : nrRestartari(0), vieti(3), scorTotal(0) {}

void MeniuRestart::afiseazaOptiuni(int vietiRamase) {
  std::cout << std::endl;
  std::cout << "Vieti ramase: " << vietiRamase << std::endl;
  if (vietiRamase > 0) {
    std::cout << "Apasa [R] pentru a reincepe jocul" << std::endl;
  } else {
    std::cout << "Nu mai ai vieti! Nu poti da restart." << std::endl;
  }
  std::cout << "Apasa [ESC] pentru a iesi" << std::endl;
}

bool MeniuRestart::asteaptaDecizie() {
  while (true) {
    if (tastaDisponibila()) {
      int tasta = citesteTasta();
      if ((tasta == 'r' || tasta == 'R') && vieti > 0) {
        nrRestartari++;
        return true;
      }
      if (tasta == 27 || tasta == EOF)
        return false;
    }
    sleepMs(100);
  }
}

bool MeniuRestart::gestioneazaJoc(Joc &joc) {
  joc.ruleazaJocul(vieti, scorTotal);
  if (joc.esteGameOver()) {
    scorTotal += joc.getScorRunda();
    vieti--;
    // verifica daca scorul ar intra in clasament inainte de a-l adauga
    if (clasamentScoruri.esteRecord(joc.getScorRunda()))
      std::cout << ">>> NOU RECORD! <<<" << std::endl;
    // inregistreaza in clasamente (Clasament<int> si Clasament<double>)
    clasamentScoruri.adauga("Jucator", joc.getScorRunda());
    clasamentTimpi.adauga("Runda " + std::to_string(joc.getRunda()),
                          joc.getScorRunda() / 10.0);
    afiseazaOptiuni(vieti);
    if (vieti <= 0) {
      // nu mai poate da restart
      std::cout << std::endl;
      std::cout << "  SCOR TOTAL FINAL: " << scorTotal << " " << std::endl;
      std::cout << clasamentScoruri;
      sleepMs(500);
      return false;
    }
    return asteaptaDecizie();
  }
  return false;
}

void MeniuRestart::inregistreazaRunda(int scorRunda, double durataSec) {
  scorTotal += scorRunda;
  if (vieti > 0)
    vieti--;
  clasamentScoruri.adauga("Jucator", scorRunda);
  if (durataSec > 0.0)
    clasamentTimpi.adauga("Runda", durataSec);
}

int MeniuRestart::getVieti() const { return vieti; }
int MeniuRestart::getScorTotal() const { return scorTotal; }

// cppcheck-suppress unusedFunction
const Clasament<int> &MeniuRestart::getClasamentScoruri() const {
  return clasamentScoruri;
}
// cppcheck-suppress unusedFunction
const Clasament<double> &MeniuRestart::getClasamentTimpi() const {
  return clasamentTimpi;
}

std::ostream &operator<<(std::ostream &os, const MeniuRestart &m) {
  os << "MeniuRestart[restartari: " << m.nrRestartari << " | vieti: " << m.vieti
     << " | scor total: " << m.scorTotal << "]";
  return os;
}
