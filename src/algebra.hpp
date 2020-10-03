#pragma once

#include "sfml.hpp"
#include <cmath>

inline float dotProduct (const sf::Vector2f& a, const sf::Vector2f& b) { 
  return a.x * b.x + a.y * b.y;
}

inline float module(const sf::Vector2f& a) {
  return sqrt(dotProduct(a, a));
}

inline void rotate(sf::Vector2f& a, float angle) {
  float newX = a.x * cos(angle) - a.y * sin(angle);
  float newY = a.x * sin(angle) + a.y * cos(angle);

  a = { newX, newY };
}