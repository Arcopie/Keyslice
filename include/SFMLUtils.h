#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>

// converteste o tasta SFML in caracterul ASCII corespunzator
// (litera mica / cifra / 27 pentru ESC); 0 daca tasta nu este relevanta
int sfKeyToChar(sf::Keyboard::Key key);

// construieste un sf::Color dintr-un cod hex de forma 0xRRGGBB
sf::Color hexColor(unsigned int rgb);
