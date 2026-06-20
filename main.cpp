#include "include/EntitateJoc.h"
#include "include/Exceptii.h"
#include "include/Joc.h"
#include "include/Matrice.h"
#include "include/MeniuRestart.h"
#include "include/Utils.h"
#include <iostream>

#ifndef GITHUB_ACTIONS
#include "include/GameRenderer.h"
#include "include/MenuScreen.h"
#include "include/SFMLUtils.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <optional>
#endif

namespace {

// modul terminal - pastrat pentru CI / fallback fara interfata grafica
[[maybe_unused]] int ruleazaTerminal() {
  std::cout << "Inamicii se spawneaza - elimina-i efectuand un slice "
            << std::endl;
  std::cout << "Apasa o litera sau cifra pentru a te teleporta + slice"
            << std::endl;
  std::cout << "! = inamic periculos (nu il poti omori)" << std::endl;
  std::cout << "B = MiniBoss (necesita mai multe lovituri)" << std::endl;
  std::cout << "@ = jucatorul tau" << std::endl;
  std::cout << "La scor multiplu de 100 apare un MiniBoss" << std::endl;

  Matrice mat(4, 10);
  std::cout << std::endl << mat;

  Joc joc;
  std::cout << joc << std::endl;
  std::cout << "Entitati active total: " << EntitateJoc::getContor()
            << std::endl;

  std::cout << std::endl
            << "Apasa orice tasta pentru a incepe jocul" << std::endl;
  citesteTasta();

  MeniuRestart meniu;
  bool joaca = true;
  while (joaca) {
    Joc jocNou;
    joaca = meniu.gestioneazaJoc(jocNou);
  }

  std::cout << meniu << std::endl;
  std::cout << "Vieti ramase: " << meniu.getVieti()
            << " | Scor total: " << meniu.getScorTotal() << std::endl;
  return 0;
}

#ifndef GITHUB_ACTIONS
// incearca fonta bundle-uita, apoi fonturi de sistem (Windows) ca fallback
bool incarcaFont(sf::Font &font) {
  const char *candidati[] = {
      "assets/JetBrainsMono-Regular.ttf",
      "JetBrainsMono-Regular.ttf",
      "C:/Windows/Fonts/consola.ttf",
      "C:/Windows/Fonts/arial.ttf",
  };
  for (const char *cale : candidati)
    if (font.openFromFile(cale))
      return true;
  return false;
}

// modul grafic SFML
int ruleazaGui() {
  sf::RenderWindow window(sf::VideoMode({750, 520}), "Slice Game",
                          sf::Style::Close);
  window.setFramerateLimit(60);

  sf::Font font;
  if (!incarcaFont(font)) {
    std::cerr
        << "Nu am putut incarca o fonta (assets/JetBrainsMono-Regular.ttf)."
        << std::endl;
    return 1;
  }

  std::srand(static_cast<unsigned>(std::time(nullptr)));

  GameRenderer renderer(font, window);
  MenuScreen menu(font, window);
  MeniuRestart meniu;

  enum class State { MENU_START, PLAYING, GAME_OVER, CLASAMENT };
  State state = State::MENU_START;
  State stareAnterioara = State::MENU_START; // pentru revenirea din CLASAMENT
  std::unique_ptr<Joc> joc;
  int scorRundaAfisat = 0;
  std::chrono::steady_clock::time_point startRunda;

  while (window.isOpen()) {
    while (const std::optional<sf::Event> ev = window.pollEvent()) {
      if (ev->is<sf::Event::Closed>()) {
        window.close();
      } else if (state == State::CLASAMENT) {
        if (menu.handleEventClasament(*ev) == MenuAction::INAPOI)
          state = stareAnterioara;
      } else if (state == State::MENU_START || state == State::GAME_OVER) {
        const MenuAction action = menu.handleEvent(*ev, meniu.getVieti());
        if (action == MenuAction::RESTART) {
          joc = std::make_unique<Joc>();
          scorRundaAfisat = 0;
          startRunda = std::chrono::steady_clock::now();
          state = State::PLAYING;
        } else if (action == MenuAction::CLASAMENT) {
          stareAnterioara = state;
          state = State::CLASAMENT;
        } else if (action == MenuAction::EXIT) {
          window.close();
        }
      } else if (state == State::PLAYING) {
        if (const auto *kp = ev->getIf<sf::Event::KeyPressed>()) {
          if (kp->code == sf::Keyboard::Key::Escape) {
            window.close();
          } else {
            const int tasta = sfKeyToChar(kp->code);
            if (tasta != 0 && joc)
              joc->proceseazaTasta(tasta);
          }
        }
      }
    }

    if (!window.isOpen())
      break;

    if (state == State::PLAYING && joc) {
      joc->tick();
      if (joc->esteGameOver()) {
        scorRundaAfisat = joc->getScorRunda();
        const double durata = std::chrono::duration<double>(
                                  std::chrono::steady_clock::now() - startRunda)
                                  .count();
        meniu.inregistreazaRunda(scorRundaAfisat, durata);
        state = State::GAME_OVER;
      }
    }

    window.clear(hexColor(0x0d0d1a));
    if (state == State::PLAYING && joc) {
      renderer.render(*joc, meniu.getVieti(), meniu.getScorTotal());
    } else if (state == State::CLASAMENT) {
      menu.renderClasament(meniu.getClasamentScoruri(),
                           meniu.getClasamentTimpi());
    } else {
      menu.render(scorRundaAfisat, meniu.getScorTotal(), meniu.getVieti(),
                  state == State::GAME_OVER);
    }
    window.display();
  }
  return 0;
}
#endif // GITHUB_ACTIONS

} // namespace

int main() {
  try {
#ifdef GITHUB_ACTIONS
    return ruleazaTerminal();
#else
    return ruleazaGui();
#endif
  } catch (const ExceptieJoc &e) {
    std::cerr << "Eroare joc: " << e.what() << std::endl;
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "Eroare: " << e.what() << std::endl;
    return 1;
  }
}
