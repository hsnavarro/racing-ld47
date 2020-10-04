#pragma once

#include "sfml.hpp"
#include <cstdio>

inline void drawTextRight(sf::Text& text, float x, float y, sf::RenderWindow& window) {
  const auto size = text.getLocalBounds().width;
  text.setPosition(x - size, y);
  window.draw(text);
}
