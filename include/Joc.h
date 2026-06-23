#pragma once
#include "EntitateJoc.h"
#include "Jucator.h"
#include "Matrice.h"
#include "Observer.h"
#include "PowerUp.h"
#include "Timer.h"
#include <chrono>
#include <memory>
#include <ostream>
#include <vector>

class GestorRunde;
class GestorPowerUp;
class AfisajHUD;

// compune Matrice, Jucator, Timer si un vector de entitati polimorfice.
// emite evenimente (Pattern Observer) si detine gestorii care reactioneaza.
class Joc : public ISubiect {
  Matrice matrice;
  Jucator jucator;
  std::vector<std::shared_ptr<EntitateJoc>> entitati;
  std::vector<PowerUp> powerups;
  Timer timer;
  std::chrono::steady_clock::time_point ultimSpawnPericulos;
  std::chrono::steady_clock::time_point inghetPanaLa; // power-up INGHET
  bool ruleaza;
  bool gameOver;
  int nextId;
  int totalOmorati;        // contor consumat pentru spawn-ul de periculosi
  int killsTotal;          // total kills (pentru evenimente)
  int urmatorPragMiniBoss; // urmatorul scor la care apare mini-boss
  int urmatorPragRunda;    // urmatorul scor la care se schimba runda
  int runda;
  int maxInamici;          // configurat de GestorRunde
  unsigned int culoareTema; // configurat de GestorRunde

  // observatori concreti (detinuti de Joc)
  std::shared_ptr<GestorRunde> gestorRunde;
  std::shared_ptr<GestorPowerUp> gestorPowerUp;
  std::shared_ptr<AfisajHUD> hud;

  void curataMortii();
  void adaugaInamicPericulos();
  void adaugaMiniBoss();
  void adaugaInamicRapid();
  void adaugaInamicVanator();
  Pozitie spawneazaPozitie(); // gaseste pozitie random departe de jucator
  void verificaRunda();       // emite "RUNDA_COMPLETA" la atingerea pragului
  void colecteazaPowerUp();   // colecteaza '*' de pe pozitia jucatorului
  bool verificaColiziune();   // true daca jucatorul a fost atins (cu scut/game over)
public:
  Joc();
  ~Joc() override;

  void adaugaInamic();
  bool proceseazaTasta(int tasta);
  void mutaEntitati();
  void afiseazaEcran(int vieti, int scorTotal) const;
  void ruleazaJocul(int vieti, int scorTotal);

  // avanseaza jocul cu un pas (spawn + miscare pe baza timerelor), fara render;
  // apelat din bucla SFML in locul lui ruleazaJocul
  [[maybe_unused]] void tick();

  // ---- comenzi folosite de observatori (Pattern Observer) ----
  void setIntervalMiscare(double s);
  void setIntervalSpawn(double s);
  void setMaxInamici(int n);
  void setCuloareTema(unsigned int c);
  void activeazaEfectPowerUp(TipPowerUp tip);
  void spawnPowerUp();
  [[nodiscard]] bool esteInghetat() const;

  // getteri read-only pentru renderer
  [[nodiscard, maybe_unused]] const Matrice& getMatrice() const;
  [[nodiscard, maybe_unused]] const Jucator& getJucator() const;
  [[nodiscard, maybe_unused]] const std::vector<std::shared_ptr<EntitateJoc>>& getEntitati() const;
  [[nodiscard, maybe_unused]] const std::vector<PowerUp>& getPowerups() const;
  [[nodiscard, maybe_unused]] const AfisajHUD& getHud() const;
  [[nodiscard, maybe_unused]] unsigned int getCuloareTema() const;
  [[nodiscard, maybe_unused]] int getRunda() const;

  [[nodiscard]] bool esteGameOver() const;
  [[nodiscard]] int getScorRunda() const;
  [[nodiscard]] bool areJucatorulScut() const;
  [[nodiscard]] int getNrVanatori() const;
  friend std::ostream &operator<<(std::ostream &os, const Joc &j);
};
