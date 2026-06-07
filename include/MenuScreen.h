#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class MenuAction { NONE, RESTART, EXIT };

// ecranul de meniu (start / game over). Deseneaza titlul, scorurile (optional)
// si butoanele Restart / Iesire. Restart e dezactivat cand nu mai sunt vieti.
class MenuScreen {
    sf::Font& font;
    sf::RenderWindow& window;
    bool isRestartHovered = false;
    bool isExitHovered = false;

    static sf::FloatRect restartBounds();
    static sf::FloatRect exitBounds();
    void drawButton(const std::string& label, const sf::FloatRect& bounds,
                    bool disabled, bool hovered);

public:
    MenuScreen(sf::Font& font, sf::RenderWindow& window);

    void render(int scorRunda, int scorTotal, int vieti, bool showScores);
    MenuAction handleEvent(const sf::Event& event, int vieti);
};
