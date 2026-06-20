#include "../include/Joc.h"
#include "../include/AfisajHUD.h"
#include "../include/GestorPowerUp.h"
#include "../include/GestorRunde.h"
#include "../include/InamicNormal.h"
#include "../include/InamicPericulos.h"
#include "../include/InamicRapid.h"
#include "../include/InamicVanator.h"
#include "../include/MiniBoss.h"
#include "../include/Utils.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

Joc::Joc()
    : matrice(4, 10), jucator(Pozitie(1, 5)), timer(5.0, 6.0),
      ultimSpawnPericulos(std::chrono::steady_clock::now()),
      inghetPanaLa(std::chrono::steady_clock::now()), ruleaza(false),
      gameOver(false), nextId(1), totalOmorati(0), killsTotal(0),
      urmatorPragMiniBoss(100), urmatorPragRunda(200), runda(1), maxInamici(8),
      culoareTema(0x29b6f6), hud(std::make_shared<AfisajHUD>()) {
  // Pattern Observer: Joc detine si inregistreaza gestorii care reactioneaza
  gestorRunde = std::make_shared<GestorRunde>(this);
  gestorPowerUp = std::make_shared<GestorPowerUp>(this);
  adaugaObserver(gestorRunde);
  adaugaObserver(gestorPowerUp);
  adaugaObserver(hud);
}

Joc::~Joc() = default;

// gaseste o pozitie random care nu se suprapune cu jucatorul
Pozitie Joc::spawneazaPozitie() {
  Pozitie p = matrice.pozitieRandom();
  int inc = 0;
  while (p == jucator.getPoz() && inc < 20) {
    p = matrice.pozitieRandom();
    inc++;
  }
  return p;
}

// sterge entitatile inactive cu remove_if (STL)
void Joc::curataMortii() {
  entitati.erase(std::remove_if(entitati.begin(), entitati.end(),
                                [](const std::shared_ptr<EntitateJoc> &e) {
                                  return !e->esteActiv();
                                }),
                 entitati.end());
}

void Joc::adaugaInamicPericulos() {
  entitati.push_back(
      std::make_shared<InamicPericulos>(nextId++, spawneazaPozitie(), 2.0));
}

void Joc::adaugaMiniBoss() {
  // scor 100 -> 4 vieti, 200 -> 5, ..., max 8
  int vieti = std::min(3 + urmatorPragMiniBoss / 100, 8);
  entitati.push_back(
      std::make_shared<MiniBoss>(nextId++, spawneazaPozitie(), vieti));
  urmatorPragMiniBoss += 100;
}

void Joc::adaugaInamicRapid() {
  // inamicul rapid apare dupa scor 50 si se misca diagonal
  if (jucator.getScor() < 50)
    return;
  const char simboluri[] = "RSTUVWXY";
  char sim = simboluri[(nextId - 1) % 8];
  entitati.push_back(
      std::make_shared<InamicRapid>(nextId++, spawneazaPozitie(), sim));
}

void Joc::adaugaInamicVanator() {
  // vanatorul apare dupa scor 150 si urmareste jucatorul
  if (jucator.getScor() < 150)
    return;
  entitati.push_back(
      std::make_shared<InamicVanator>(nextId++, spawneazaPozitie()));
}

void Joc::adaugaInamic() {
  // functie sablon numaraDeTyp<InamicNormal> in loc de dynamic_cast manual
  int nrNormali = numaraDeTyp<InamicNormal>(entitati);
  if (nrNormali >= maxInamici)
    return;

  if (jucator.getScor() >= 150 && nextId % 4 == 0) {
    adaugaInamicVanator();
  } else if (nextId % 3 == 0 && jucator.getScor() >= 50) {
    adaugaInamicRapid();
  } else {
    const char simboluri[] = "EFGHIJKL";
    char sim = simboluri[(nextId - 1) % 8];
    auto inamic =
        std::make_shared<InamicNormal>(nextId++, spawneazaPozitie(), sim);
    // demonstreaza schimbarea strategiei la rulare (Pattern Strategy)
    if (nextId % 7 == 0)
      inamic->setStrategie(std::make_shared<StrategieZigzag>());
    entitati.push_back(inamic);
  }
}

void Joc::verificaRunda() {
  while (jucator.getScor() >= urmatorPragRunda) {
    runda++;
    urmatorPragRunda += 200;
    notifica("RUNDA_COMPLETA", runda);
  }
}

void Joc::colecteazaPowerUp() {
  for (auto &pu : powerups) {
    if (pu.activ && pu.pozitie == jucator.getPoz()) {
      pu.activ = false;
      notifica("POWERUP_COLECTAT", static_cast<int>(pu.tip));
    }
  }
  powerups.erase(std::remove_if(powerups.begin(), powerups.end(),
                                [](const PowerUp &p) { return !p.activ; }),
                 powerups.end());
}

// returneaza true daca jucatorul a fost atins fatal (game over);
// scutul absoarbe o coliziune.
bool Joc::verificaColiziune() {
  if (!jucator.atingeEntitate(entitati))
    return false;
  if (jucator.consumaScut())
    return false; // scutul a salvat jucatorul
  gameOver = true;
  ruleaza = false;
  // gestorRunde si gestorPowerUp nu mai sunt necesari dupa game over;
  // eliminam observatorii inainte de notificare (Pattern Observer complet)
  eliminaObserver(gestorRunde);
  eliminaObserver(gestorPowerUp);
  notifica("GAME_OVER", jucator.getScor());
  return true;
}

bool Joc::proceseazaTasta(int tasta) {
  if (tasta == 27 || tasta == 24 || tasta == EOF) { // 27=ESC, 24=Ctrl+X
    ruleaza = false;
    return false;
  }
  char ch = static_cast<char>(tasta);
  if (ch >= 'A' && ch <= 'Z')
    ch = ch - 'A' + 'a';

  Pozitie dest(0, 0);
  if (!matrice.gasestePozitie(ch, dest))
    return false;

  Pozitie pozVeche = jucator.getPoz();
  jucator.teleport(dest);

  // colecteaza un power-up daca exista pe noua pozitie
  colecteazaPowerUp();

  // aterizat fix pe o entitate -> game over (daca nu are scut)
  if (verificaColiziune())
    return true;

  int omorati = jucator.slice(entitati, pozVeche);
  if (omorati > 0) {
    totalOmorati += omorati;
    killsTotal += omorati;
    curataMortii();
    notifica("INAMIC_OMORAT", killsTotal);
    notifica("SCOR_SCHIMBAT", jucator.getScor());
    while (totalOmorati >= 2) {
      adaugaInamicPericulos();
      totalOmorati -= 2;
    }
  }

  // spawn mini-boss la fiecare prag de 100 puncte
  while (jucator.getScor() >= urmatorPragMiniBoss)
    adaugaMiniBoss();

  // schimbare de runda la fiecare prag (Pattern Observer)
  verificaRunda();

  return true;
}

void Joc::mutaEntitati() {
  if (esteInghetat())
    return; // power-up INGHET: inamicii nu se misca
  for (auto &e : entitati)
    e->muta(jucator.getPoz(), matrice.getLinii(), matrice.getColoane());
}

void Joc::afiseazaEcran(int vieti, int scorTotal) const {
  clearScreen();
  std::cout << "SLICE GAME" << std::endl;
  std::cout << "Vieti: " << vieti << " | Runda: " << runda
            << " | Scor runda: " << jucator.getScor()
            << " | Scor total: " << scorTotal + jucator.getScor()
            << " | Sliceuri: " << jucator.getSliceuri()
            << " | Entitati: " << EntitateJoc::getContor() << std::endl;

  // functie sablon filtreaza<shared_ptr<EntitateJoc>> - entitatile active
  auto activi = filtreaza<std::shared_ptr<EntitateJoc>>(
      entitati,
      [](const std::shared_ptr<EntitateJoc> &e) { return e->esteActiv(); });
  std::cout << "Apasa litera/cifra = teleportare + slice | active: "
            << activi.size() << std::endl;
  std::cout << "ESC / Ctrl+X = iesire | ! = periculos | B = MiniBoss | V = "
               "Vanator | * = power-up"
            << std::endl;
  if (esteInghetat())
    std::cout << ">>> INGHET activ <<<" << std::endl;
  if (jucator.areScut())
    std::cout << ">>> SCUT activ <<<" << std::endl;

  // dynamic_cast pentru info specifice MiniBoss
  for (const auto &e : entitati) {
    if (const auto *boss = dynamic_cast<const MiniBoss *>(e.get())) {
      if (boss->esteActiv()) {
        std::cout << ">>> MINI-BOSS activ! Vieti: " << boss->getVietiRamase()
                  << "/" << boss->getVietiMaxime() << " <<<" << std::endl;
      }
    }
  }
  std::cout << std::endl;
  matrice.afiseaza(jucator, entitati);
  std::cout << std::endl;
}

void Joc::ruleazaJocul(int vieti, int scorTotal) {
  srand((unsigned)time(nullptr));
  ruleaza = true;
  gameOver = false;
  ultimSpawnPericulos = std::chrono::steady_clock::now();
  adaugaInamic();
  afiseazaEcran(vieti, scorTotal);

  while (ruleaza) {
    bool trebuieRedesnat = false;

    if (timer.trebuieSpawn()) {
      adaugaInamic();
      timer.resetSpawn();
      trebuieRedesnat = true;
    }

    double secDePericulos =
        std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                      ultimSpawnPericulos)
            .count();
    if (secDePericulos >= 2.0) {
      adaugaInamicPericulos();
      ultimSpawnPericulos = std::chrono::steady_clock::now();
      trebuieRedesnat = true;
    }

    int inainte = (int)entitati.size();
    curataMortii();
    if ((int)entitati.size() != inainte)
      trebuieRedesnat = true;

    if (timer.trebuieMiscare()) {
      mutaEntitati();
      timer.resetMiscare();
      trebuieRedesnat = true;
      if (verificaColiziune())
        break;
    }

    if (verificaColiziune())
      break;

    if (tastaDisponibila()) {
      int tasta = citesteTasta();
      if (proceseazaTasta(tasta))
        trebuieRedesnat = true;
    }

    if (trebuieRedesnat)
      afiseazaEcran(vieti, scorTotal);
    sleepMs(100);
  }

  afiseazaEcran(vieti, scorTotal);
  if (gameOver) {
    std::cout << "GAME OVER! Un inamic te-a atins!" << std::endl;
    std::cout << "Scor runda: " << jucator.getScor() << std::endl;
  }
  sleepMs(500);
}

// un singur pas de joc fara render/input - pentru bucla SFML.
void Joc::tick() {
  if (gameOver)
    return;

  // prima rulare: porneste timerul de periculos si spawneaza primul inamic
  if (!ruleaza) {
    ruleaza = true;
    ultimSpawnPericulos = std::chrono::steady_clock::now();
    adaugaInamic();
  }

  if (timer.trebuieSpawn()) {
    adaugaInamic();
    timer.resetSpawn();
  }

  double secDePericulos =
      std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                    ultimSpawnPericulos)
          .count();
  if (secDePericulos >= 2.0) {
    adaugaInamicPericulos();
    ultimSpawnPericulos = std::chrono::steady_clock::now();
  }

  curataMortii();

  if (timer.trebuieMiscare()) {
    mutaEntitati();
    timer.resetMiscare();
    if (verificaColiziune())
      return;
  }

  verificaColiziune();
}

// ---- comenzi folosite de observatori ----
void Joc::setIntervalMiscare(double s) { timer.setIntervalMiscare(s); }
void Joc::setIntervalSpawn(double s) { timer.setIntervalSpawn(s); }
void Joc::setMaxInamici(int n) { maxInamici = n; }
void Joc::setCuloareTema(unsigned int c) { culoareTema = c; }

void Joc::activeazaEfectPowerUp(TipPowerUp tip) {
  switch (tip) {
  case TipPowerUp::INGHET:
    inghetPanaLa = std::chrono::steady_clock::now() + std::chrono::seconds(3);
    break;
  case TipPowerUp::SCUT:
    jucator.activeazaScut();
    break;
  case TipPowerUp::DUBLU_SCOR:
    jucator.activeazaDubluScor(5.0);
    break;
  }
}

void Joc::spawnPowerUp() {
  TipPowerUp t = static_cast<TipPowerUp>(rand() % 3);
  powerups.emplace_back(spawneazaPozitie(), t);
}

bool Joc::esteInghetat() const {
  return std::chrono::steady_clock::now() < inghetPanaLa;
}

// cppcheck-suppress unusedFunction
const Matrice &Joc::getMatrice() const { return matrice; }
// cppcheck-suppress unusedFunction
const Jucator &Joc::getJucator() const { return jucator; }
// cppcheck-suppress unusedFunction
const std::vector<std::shared_ptr<EntitateJoc>> &Joc::getEntitati() const {
  return entitati;
}
// cppcheck-suppress unusedFunction
const std::vector<PowerUp> &Joc::getPowerups() const { return powerups; }
// cppcheck-suppress unusedFunction
const AfisajHUD &Joc::getHud() const { return *hud; }
// cppcheck-suppress unusedFunction
unsigned int Joc::getCuloareTema() const { return culoareTema; }
// cppcheck-suppress unusedFunction
int Joc::getRunda() const { return runda; }

bool Joc::esteGameOver() const { return gameOver; }
int Joc::getScorRunda() const { return jucator.getScor(); }
// cppcheck-suppress unusedFunction
bool Joc::areJucatorulScut() const { return jucator.areScut(); }

std::ostream &operator<<(std::ostream &os, const Joc &j) {
  os << "Joc: " << j.jucator << " | " << j.timer;
  return os;
}
