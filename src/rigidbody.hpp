#pragma once

#include <cmath>

#include "consts.hpp"
#include "algebra.hpp"

const f64 COLLISION_LINEAR_TO_ANGULAR = 0.001;

class Rigidbody {
public:
  Rigidbody();
  Rigidbody(f64, f64, f64);

  void update(f64, f64, f64);
  void resolveCollision(sf::Vector2<f64> collisionVector);

  void applyPointAngularVelocity(f64);
  void applyPointLinearVelocity(sf::Vector2<f64>);

  bool isGoingForward() const;

  void reset();

public:
  sf::Vector2<f64> position = { 0.0, 0.0 };
  sf::Vector2<f64> direction = { 0.0, -1.0 };
  sf::Vector2<f64> linearVelocity = { 0.0, 0.0 };
  f64 angularVelocity = 0.0;

  f64 kForwardDrag = 0.3;
  f64 kLateralDrag = 1.5;
  f64 kAngularDrag = 0.9;
};
