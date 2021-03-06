#pragma once

#include <cmath>
#include <array>
#include "sfml.hpp"
#include "types.hpp"
#include "consts.hpp"

namespace sf {

  using Line = std::array<sf::Vector2f, 2>;

}

inline s32 sign(const f32 f) { return (f > EPS) - (f < -EPS); }
inline s32 sign(const f64 f) { return (f > EPS) - (f < -EPS); }

inline f32 crossProduct(const sf::Vector2f& lhs, const sf::Vector2f& rhs) { return lhs.x * rhs.y - lhs.y * rhs.x; }
inline f64 crossProduct(const sf::Vector2<f64>& lhs, const sf::Vector2<f64>& rhs) { return lhs.x * rhs.y - lhs.y * rhs.x; }

inline f32 dotProduct(const sf::Vector2f& a, const sf::Vector2f& b) { return a.x * b.x + a.y * b.y; }
inline f64 dotProduct(const sf::Vector2<f64>& a, const sf::Vector2<f64>& b) { return a.x * b.x + a.y * b.y; }

inline f32 getMagnitude(const sf::Vector2f& v) { return std::hypot(v.x, v.y); }
inline f64 getMagnitude(const sf::Vector2<f64>& v) { return std::hypot(v.x, v.y); }

inline sf::Vector2f getUnitVector(const sf::Vector2f& v) { if (getMagnitude(v) < EPS) return v; return v / getMagnitude(v); }
inline sf::Vector2<f64> getUnitVector(const sf::Vector2<f64>& v) { if (getMagnitude(v) < EPS) return v; return v / getMagnitude(v); }

inline sf::Vector2f getPerpendicular(const sf::Vector2f& v) { return { v.y, -v.x }; }
inline sf::Vector2<f64> getPerpendicular(const sf::Vector2<f64>& v) { return { v.y, -v.x }; }

inline void rotate(sf::Vector2f& a, const float angle) {
  float newX = a.x * cos(angle) - a.y * sin(angle);
  float newY = a.x * sin(angle) + a.y * cos(angle);
  a = { newX, newY };
}

inline void rotate(sf::Vector2<f64>& a, const f64 angle) {
  f64 newX = a.x * cos(angle) - a.y * sin(angle);
  f64 newY = a.x * sin(angle) + a.y * cos(angle);
  a = { newX, newY };
}

inline bool linesIntersect(const sf::Line& a, const sf::Line& b) {
  auto a_b0 = sign(crossProduct(b[0] - a[0], a[1] - a[0]));
  auto a_b1 = sign(crossProduct(b[1] - a[0], a[1] - a[0]));
  auto b_a0 = sign(crossProduct(a[0] - b[0], b[1] - b[0]));
  auto b_a1 = sign(crossProduct(a[1] - b[0], b[1] - b[0]));
  return !(a_b0 * a_b1 > 0 or b_a0 * b_a1 > 0);
}

inline float getRotation(const sf::Vector2f& a) { return to_deg(std::atan2(a.x, -a.y)); }
inline f64 getRotation(const sf::Vector2<f64>& a) { return static_cast<f64>(to_deg64(std::atan2(a.x, -a.y))); }

inline void drawLine(const sf::Line& line, const sf::Color& color, sf::RenderWindow& window) {
  sf::Vertex verts[] = { { line[0], color }, { line[1], color } };
  window.draw(verts, 2, sf::Lines);
}

inline sf::Vector2f to_vector2f(const sf::Vector2<f64>& v) { return { static_cast<float>(v.x), static_cast<float>(v.y) }; }
inline sf::Vector2<f64> to_vector2f64(const sf::Vector2f& v) { return { static_cast<f64>(v.x), static_cast<f64>(v.y) }; }

inline float lerp(const float a, const float b, const float t) { return a + t * (b - a); }
inline float getRatio(const float minValue, const float maxValue, const float measuredValue) { 
  return (measuredValue - minValue) / (maxValue - minValue);
}
