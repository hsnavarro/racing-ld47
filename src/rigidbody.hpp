#pragma once

#include "consts.hpp"
#include "algebra.hpp"
#include <cmath>

class RigidBody {
public:
  sf::Vector2f position;
  sf::Vector2f direction;
  sf::Vector2f linearVelocity = { 0.0f, 0.0f };

  RigidBody(sf::Vector2f, sf::Vector2f);

  void rotate(float);
  void simulate(float, float);
  bool isObjectGoingForward();
};