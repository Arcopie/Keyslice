#pragma once
#include "EntitateJoc.h"
#include "Jucator.h"
#include "Matrice.h"
#include "Timer.h"
#include <chrono>
#include <memory>
#include <ostream>
#include <vector>

// compune Matrice, Jucator, Timer si un vector de entitati polimorfice
class Joc {
  Matrice matrice;
  Jucator jucator;
  std::vector<std::shared_ptr<EntitateJoc>> entitati;
  Timer timer;
  std::chrono::steady_clock::time_point ultimSpawnPericulos;
  bool ruleaza;
  bool gameOver;
  int nextId;
  int totalOmorati;
  int urmatorPragMiniBoss; // urmatorul scor la care apare mini-boss

  void curataMortii();
  void adaugaInamicPericulos();
  void adaugaMiniBoss();
  void adaugaInamicRapid();
  Pozitie spawneazaPozitie(); // gaseste pozitie random departe de jucator
public:
  Joc();

  void adaugaInamic();
  bool proceseazaTasta(int tasta);
  void mutaEntitati();
  void afiseazaEcran(int vieti, int scorTotal) const;
  void ruleazaJocul(int vieti, int scorTotal);

  // avanseaza jocul cu un pas (spawn + miscare pe baza timerelor), fara render;
  // apelat din bucla SFML in locul lui ruleazaJocul
  [[maybe_unused]] void tick();

  // getteri read-only pentru renderer
  [[nodiscard, maybe_unused]] const Matrice& getMatrice() const;
  [[nodiscard, maybe_unused]] const Jucator& getJucator() const;
  [[nodiscard, maybe_unused]] const std::vector<std::shared_ptr<EntitateJoc>>& getEntitati() const;

  [[nodiscard]] bool esteGameOver() const;
  [[nodiscard]] int getScorRunda() const;
  friend std::ostream &operator<<(std::ostream &os, const Joc &j);
};
