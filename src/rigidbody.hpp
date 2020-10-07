#pragma once

#include <cmath>

#include "consts.hpp"
#include "algebra.hpp"

const f64 COLLISION_LINEAR_TO_ANGULAR = 0.001;

class Rigidbody {
public:
  Rigidbody(const f64 = 0.3, const f64 = 1.5, const f64 = 0.9);

  void update(const f64, const f64, const f64);
  void resolveCollision(const sf::Vector2<f64>&);

  void applyPointAngularVelocity(const f64);
  void applyPointLinearVelocity(const sf::Vector2<f64>&);

  bool isGoingForward() const;

  void reset();

  void setForwardDrag(const f64);

public:
  sf::Vector2<f64> position = { 0.0, 0.0 };
  sf::Vector2<f64> direction = { 0.0, -1.0 };
  sf::Vector2<f64> linearVelocity = { 0.0, 0.0 };
  f64 angularVelocity = 0.0;

  f64 forwardDrag = 0.3;
  f64 lateralDrag = 1.5;
  f64 kAngularDrag = 0.9;
};
