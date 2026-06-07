#include "../include/MenuScreen.h"
#include "../include/SFMLUtils.h"
#include <string>

namespace {
constexpr float WINDOW_W = 750.f;

// deseneaza un text centrat in punctul (cx, cy)
void putText(sf::RenderWindow& window, const sf::Font& font, const std::string& s,
             float cx, float cy, unsigned int size, sf::Color color, bool bold) {
    sf::Text text(font, s, size);
    text.setFillColor(color);
    if (bold)
        text.setStyle(sf::Text::Bold);
    const sf::FloatRect b = text.getLocalBounds();
    text.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
    text.setPosition({cx, cy});
    window.draw(text);
}
} // namespace

MenuScreen::MenuScreen(sf::Font& font, sf::RenderWindow& window)
    : font(font), window(window) {}

sf::FloatRect MenuScreen::restartBounds() {
    return sf::FloatRect({215.f, 320.f}, {320.f, 54.f});
}

sf::FloatRect MenuScreen::exitBounds() {
    return sf::FloatRect({215.f, 395.f}, {320.f, 54.f});
}

void MenuScreen::drawButton(const std::string& label, const sf::FloatRect& bounds,
                            bool disabled, bool hovered) {
    sf::RectangleShape rect(bounds.size);
    rect.setPosition(bounds.position);
    sf::Color fill = hexColor(0x6a1b9a);
    if (disabled)
        fill = hexColor(0x3a3a4a);
    else if (hovered)
        fill = hexColor(0x8e24aa);
    rect.setFillColor(fill);
    rect.setOutlineThickness(2.f);
    rect.setOutlineColor(hexColor(0x555566));
    window.draw(rect);

    const sf::Color textColor = disabled ? hexColor(0x777788) : hexColor(0xffffff);
    putText(window, font, label, bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f, 22u, textColor, true);
}

void MenuScreen::render(int scorRunda, int scorTotal, int vieti, bool showScores) {
    // panou central pe fundalul deja sters de apelant
    sf::RectangleShape panel({600.f, 440.f});
    panel.setPosition({75.f, 40.f});
    panel.setFillColor(hexColor(0x2d0a5e));
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(hexColor(0x555566));
    window.draw(panel);

    putText(window, font, "SLICE GAME", WINDOW_W / 2.f, 110.f, 48u,
            hexColor(0xffffff), true);

    if (showScores) {
        putText(window, font, "Scor runda:  " + std::to_string(scorRunda),
                WINDOW_W / 2.f, 200.f, 24u, hexColor(0xe0e0e0), false);
        putText(window, font, "Scor total:  " + std::to_string(scorTotal),
                WINDOW_W / 2.f, 240.f, 24u, hexColor(0xe0e0e0), false);
    } else {
        putText(window, font, "Apasa Restart pentru a incepe", WINDOW_W / 2.f,
                220.f, 22u, hexColor(0xe0e0e0), false);
    }

    const bool restartDisabled = (vieti <= 0);
    drawButton("R  ->  Restart", restartBounds(), restartDisabled,
               isRestartHovered && !restartDisabled);
    drawButton("ESC  ->  Iesire", exitBounds(), false, isExitHovered);

    putText(window, font, "Vieti ramase: " + std::to_string(vieti), WINDOW_W / 2.f,
            462.f, 16u, hexColor(0x666677), false);
}

MenuAction MenuScreen::handleEvent(const sf::Event& event, int vieti) {
    if (const auto* mm = event.getIf<sf::Event::MouseMoved>()) {
        const sf::Vector2f p(static_cast<float>(mm->position.x),
                             static_cast<float>(mm->position.y));
        isRestartHovered = restartBounds().contains(p);
        isExitHovered = exitBounds().contains(p);
    } else if (const auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            const sf::Vector2f p(static_cast<float>(mb->position.x),
                                 static_cast<float>(mb->position.y));
            if (vieti > 0 && restartBounds().contains(p))
                return MenuAction::RESTART;
            if (exitBounds().contains(p))
                return MenuAction::EXIT;
        }
    } else if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        if (kp->code == sf::Keyboard::Key::R && vieti > 0)
            return MenuAction::RESTART;
        if (kp->code == sf::Keyboard::Key::Escape)
            return MenuAction::EXIT;
    }
    return MenuAction::NONE;
}
