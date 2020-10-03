#pragma once

#include <cmath>

#include "consts.hpp"
#include "algebra.hpp"

class RigidBody {
public:
  sf::Vector2f position;
  sf::Vector2f direction;
  sf::Vector2f linearVelocity = { 0.0f, 0.0f };

  RigidBody(sf::Vector2f = { 0.0f, 0.0f }, sf::Vector2f = { 0.0f, 0.0f });

  void rotate(float);
  void simulate(float, float);
  bool isObjectGoingForward();
};