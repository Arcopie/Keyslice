#include "../include/SFMLUtils.h"
#include <cstdint>

int sfKeyToChar(sf::Keyboard::Key key) {
    using K = sf::Keyboard::Key;
    // litere a-z (enumeratorii sunt contigui in SFML)
    if (key >= K::A && key <= K::Z)
        return 'a' + (static_cast<int>(key) - static_cast<int>(K::A));
    // cifre 0-9
    if (key >= K::Num0 && key <= K::Num9)
        return '0' + (static_cast<int>(key) - static_cast<int>(K::Num0));
    // taste de punctuatie folosite in grila (randul 2: ;  randul 3: , . /)
    if (key == K::Semicolon) return ';';
    if (key == K::Comma)     return ',';
    if (key == K::Period)    return '.';
    if (key == K::Slash)     return '/';
    if (key == K::Escape)
        return 27;
    return 0;
}

sf::Color hexColor(unsigned int rgb) {
    return sf::Color(static_cast<std::uint8_t>((rgb >> 16) & 0xFFu),
                     static_cast<std::uint8_t>((rgb >> 8) & 0xFFu),
                     static_cast<std::uint8_t>(rgb & 0xFFu));
}
