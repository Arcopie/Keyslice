#include "../include/GameRenderer.h"
#include "../include/AfisajHUD.h"
#include "../include/EntitateJoc.h"
#include "../include/InamicNormal.h"
#include "../include/InamicPericulos.h"
#include "../include/InamicRapid.h"
#include "../include/InamicVanator.h"
#include "../include/Joc.h"
#include "../include/Jucator.h"
#include "../include/Matrice.h"
#include "../include/MiniBoss.h"
#include "../include/PowerUp.h"
#include "../include/Pozitie.h"
#include "../include/SFMLUtils.h"
#include "../include/Utils.h"
#include <algorithm>
#include <string>

namespace {
constexpr int WINDOW_W = 750;
constexpr int WINDOW_H = 520;
constexpr int HEADER_H = 80;
constexpr int STATUS_H = 40;

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

// alege culoarea unei entitati pe baza tipului ei concret (RTTI)
sf::Color entityColor(const EntitateJoc& e) {
    if (dynamic_cast<const InamicPericulos*>(&e))
        return hexColor(0xef5350);
    if (dynamic_cast<const MiniBoss*>(&e))
        return hexColor(0xce93d8);
    if (dynamic_cast<const InamicVanator*>(&e))
        return hexColor(0x26c6da);
    if (dynamic_cast<const InamicRapid*>(&e))
        return hexColor(0x29b6f6);
    if (dynamic_cast<const InamicNormal*>(&e))
        return hexColor(0xffb300);
    return sf::Color::White;
}
} // namespace

GameRenderer::GameRenderer(sf::Font& font, sf::RenderWindow& window)
    : font(font), window(window) {}

void GameRenderer::drawHeader(int vieti, int runda, int scorRunda, int scorTotal,
                              bool areScut) {
    std::string s = "Vieti: " + std::to_string(vieti) +
                          "   Runda: " + std::to_string(runda) +
                          "   Scor: " + std::to_string(scorRunda) +
                          "   Total: " + std::to_string(scorTotal);
    if (areScut)
        s += "   [SCUT]";
    putText(window, font, s, static_cast<float>(WINDOW_W) / 2.f, 30.f, 22u,
            hexColor(0xe0e0e0), true);

    sf::RectangleShape sep({static_cast<float>(WINDOW_W), 2.f});
    sep.setPosition({0.f, static_cast<float>(HEADER_H) - 1.f});
    sep.setFillColor(hexColor(0x555566));
    window.draw(sep);
}

void GameRenderer::drawCell(int row, int col, char key, char overlay,
                            sf::Color overlayColor) {
    const float x = gridStartX + static_cast<float>(col) * cellSize;
    const float y = gridStartY + static_cast<float>(row) * cellSize;
    const float cx = x + cellSize / 2.f;
    const float cy = y + cellSize / 2.f;

    sf::RectangleShape cell({cellSize - 2.f, cellSize - 2.f});
    cell.setPosition({x + 1.f, y + 1.f});
    cell.setFillColor(hexColor(0x0d0d1a));
    cell.setOutlineThickness(1.f);
    cell.setOutlineColor(hexColor(0x1e1e2e));
    window.draw(cell);

    const std::string keyStr(1, key);
    if (overlay == 0) {
        putText(window, font, keyStr, cx, cy,
                static_cast<unsigned>(cellSize * 0.42f), hexColor(0x666677), false);
    } else {
        // tasta ramane vizibila in coltul stanga-sus ca reper de teleportare
        putText(window, font, keyStr, x + cellSize * 0.20f, y + cellSize * 0.20f,
                static_cast<unsigned>(cellSize * 0.28f), hexColor(0x666677), false);
        const std::string overlayStr(1, overlay);
        putText(window, font, overlayStr, cx, cy,
                static_cast<unsigned>(cellSize * 0.6f), overlayColor, true);
    }
}

void GameRenderer::drawGrid(
    const Matrice& matrice, const Jucator& jucator,
    const std::vector<std::shared_ptr<EntitateJoc>>& entitati,
    const std::vector<PowerUp>& powerups, unsigned int culoareTema) {
    const int rows = matrice.getLinii();
    const int cols = matrice.getColoane();

    const float margin = 20.f;
    const float availW = static_cast<float>(WINDOW_W) - 2.f * margin;
    const float availH =
        static_cast<float>(WINDOW_H - HEADER_H - STATUS_H) - 2.f * margin;
    cellSize = std::min(availW / static_cast<float>(cols),
                        availH / static_cast<float>(rows));

    const float totalW = cellSize * static_cast<float>(cols);
    const float totalH = cellSize * static_cast<float>(rows);
    gridStartX = (static_cast<float>(WINDOW_W) - totalW) / 2.f;
    gridStartY = static_cast<float>(HEADER_H) +
                 (static_cast<float>(WINDOW_H - HEADER_H - STATUS_H) - totalH) / 2.f;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            const char key = matrice.getTasta(i, j);
            char overlay = 0;
            sf::Color overlayColor = sf::Color::White;

            const Pozitie p(i, j);
            if (jucator.getPoz() == p) {
                overlay = '@';
                overlayColor = hexColor(0x00e676);
            } else {
                for (const auto& e : entitati) {
                    if (e->esteActiv() && e->esteLA(p)) {
                        overlay = e->getSimbol();
                        overlayColor = entityColor(*e);
                        break;
                    }
                }
                // power-up-urile (`*`) au prioritate vizuala doar pe celule libere
                if (overlay == 0) {
                    for (const auto& pu : powerups) {
                        if (pu.activ && pu.pozitie == p) {
                            overlay = pu.getSimbol();
                            overlayColor = hexColor(0xfff176);
                            break;
                        }
                    }
                }
            }
            drawCell(i, j, key, overlay, overlayColor);
        }
    }

    // border exterior (2px) coloreaza tema rundei curente
    sf::RectangleShape border({totalW, totalH});
    border.setPosition({gridStartX, gridStartY});
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(2.f);
    border.setOutlineColor(hexColor(culoareTema));
    window.draw(border);
}

void GameRenderer::drawStatusBar(
    const std::vector<std::shared_ptr<EntitateJoc>>& entitati) {
    const MiniBoss* boss = nullptr;
    for (const auto& e : entitati) {
        if (const auto* b = dynamic_cast<const MiniBoss*>(e.get())) {
            if (b->esteActiv()) {
                boss = b;
                break;
            }
        }
    }

    const float cy =
        static_cast<float>(WINDOW_H - STATUS_H) + static_cast<float>(STATUS_H) / 2.f;
    if (boss != nullptr) {
        const std::string s = "MiniBoss activ: " +
                              std::to_string(boss->getVietiRamase()) + "/" +
                              std::to_string(boss->getVietiMaxime()) + " vieti";
        putText(window, font, s, static_cast<float>(WINDOW_W) / 2.f, cy, 20u,
                hexColor(0xce93d8), true);
    } else {
        // functie sablon numaraDeTyp<InamicVanator> (RTTI)
        const int nrVanator = numaraDeTyp<InamicVanator>(entitati);
        const std::string s =
            "litera / cifra = teleport + slice   |   Vanatori: " +
            std::to_string(nrVanator) + "   |   ESC = iesire";
        putText(window, font, s, static_cast<float>(WINDOW_W) / 2.f, cy, 16u,
                hexColor(0x666677), false);
    }
}

void GameRenderer::drawHud(const AfisajHUD& hud) {
    const std::string pu = hud.getPowerUpActiv();
    // afiseaza kills total (getTotalOmorati) si power-up activ pe header
    const std::string killsStr = "Kills: " + std::to_string(hud.getTotalOmorati());
    if (!pu.empty()) {
        putText(window, font, killsStr + "   |   Power-up: " + pu,
                static_cast<float>(WINDOW_W) / 2.f, 60.f, 16u,
                hexColor(0xfff176), true);
    } else {
        putText(window, font, killsStr, static_cast<float>(WINDOW_W) / 2.f,
                60.f, 16u, hexColor(0xb0bec5), false);
    }
    const std::string banner = hud.getBanner();
    if (!banner.empty()) {
        putText(window, font, banner, static_cast<float>(WINDOW_W) / 2.f,
                static_cast<float>(WINDOW_H) / 2.f, 56u, hexColor(0xffee58), true);
    }
}

void GameRenderer::render(const Joc& joc, int vieti, int scorTotal) {
    drawHeader(vieti, joc.getRunda(), joc.getScorRunda(),
               scorTotal + joc.getScorRunda(), joc.areJucatorulScut());
    drawGrid(joc.getMatrice(), joc.getJucator(), joc.getEntitati(),
             joc.getPowerups(), joc.getCuloareTema());
    drawStatusBar(joc.getEntitati());
    drawHud(joc.getHud());
}
