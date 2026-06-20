#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Joc;
class Matrice;
class Jucator;
class EntitateJoc;
class AfisajHUD;
struct PowerUp;

// deseneaza starea jocului (header, grila, status bar) intr-o fereastra SFML.
// nu detine resursele - primeste referinte la fonta si fereastra partajate.
class GameRenderer {
    sf::Font& font;
    sf::RenderWindow& window;

    // geometria grilei, recalculata la fiecare drawGrid in functie de dimensiuni
    float cellSize = 0.f;
    float gridStartX = 0.f;
    float gridStartY = 0.f;

    void drawHeader(int vieti, int runda, int scorRunda, int scorTotal);
    void drawGrid(const Matrice& matrice, const Jucator& jucator,
                  const std::vector<std::shared_ptr<EntitateJoc>>& entitati,
                  const std::vector<PowerUp>& powerups, unsigned int culoareTema);
    void drawCell(int row, int col, char key, char overlay, sf::Color overlayColor);
    void drawStatusBar(const std::vector<std::shared_ptr<EntitateJoc>>& entitati);
    void drawHud(const AfisajHUD& hud);

public:
    GameRenderer(sf::Font& font, sf::RenderWindow& window);

    // deseneaza tot ecranul de joc; nu apeleaza clear()/display() (le face apelantul)
    void render(const Joc& joc, int vieti, int scorTotal);
};
