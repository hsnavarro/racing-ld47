#pragma once

#include <cmath>
#include <array>
#include "sfml.hpp"
#include "types.hpp"
#include "consts.hpp"

namespace sf {

using Line = std::array<sf::Vector2f, 2>;

}

inline s32 sign(f32 f) { return (f > EPS) - (f < -EPS); }

inline f32 crossProduct(sf::Vector2f lhs, sf::Vector2f rhs) {
  return lhs.x * rhs.y - lhs.y * rhs.x;
}

inline float dotProduct (const sf::Vector2f& a, const sf::Vector2f& b) {
  return a.x * b.x + a.y * b.y;
}

inline f32 getMagnitude(sf::Vector2f v) {
  return std::hypot(v.x, v.y);
}

inline sf::Vector2f getUnitVector(sf::Vector2f v) {
  if (getMagnitude(v) < EPS) return v;
  return v / getMagnitude(v);
}

inline sf::Vector2f getPerpendicular(sf::Vector2f v) {
  return { v.y, -v.x };
}

inline void rotate(sf::Vector2f& a, float angle) {
  float newX = a.x * cos(angle) - a.y * sin(angle);
  float newY = a.x * sin(angle) + a.y * cos(angle);

  a = { newX, newY };
}

inline bool linesIntersect(sf::Line a, sf::Line b) {
  auto a_b0 = sign(crossProduct(b[0] - a[0], a[1] - a[0]));
  auto a_b1 = sign(crossProduct(b[1] - a[0], a[1] - a[0]));
  auto b_a0 = sign(crossProduct(a[0] - b[0], b[1] - b[0]));
  auto b_a1 = sign(crossProduct(a[1] - b[0], b[1] - b[0]));
  return !(a_b0 * a_b1 > 0 or b_a0 * b_a1 > 0);
}

inline float getRotation(sf::Vector2f& a) {
  return to_deg(std::atan2(a.x, -a.y));
}