#pragma once

#include <cmath>

#include "consts.hpp"
#include "algebra.hpp"

const float COLLISION_LINEAR_TO_ANGULAR = 0.001f;

class Rigidbody {
public:
  Rigidbody();
  Rigidbody(sf::Vector2f, sf::Vector2f, float, float);

  void update(float, float, float);
  void resolveCollision(sf::Vector2f collisionVector);

  void applyPointAngularVelocity(float);
  void applyPointLinearVelocity(sf::Vector2f);

  bool isGoingForward() const;
  float getRotation() const;

public:
  sf::Vector2f position = { 0.0f, 0.0f };
  sf::Vector2f direction = { 0.0f, 1.0f };
  sf::Vector2f linearVelocity = { 0.0f, 0.0f };
  float angularVelocity = 0.0f;

  float kForwardDrag = 0.1f;
  float kAngularDrag = 0.9f;
};
