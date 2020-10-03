#pragma once

#include "consts.hpp"
#include "algebra.hpp"
#include <cmath>

class RigidBody {
public:
  float acceleration = CAR_ACCELERATION;
  float brakeAcceleration = CAR_BRAKE_ACCELERATION;
  float reverseAcceleration = CAR_REVERSE_ACCELERATION;
  float angularVelocity = CAR_ANGULAR_VELOCITY;
  sf::Vector2f position;
  sf::Vector2f direction;
  sf::Vector2f linearVelocity = { 0.0f, 0.0f };

  RigidBody(sf::Vector2f, sf::Vector2f);

  void rotate(float);
  void simulate(float, Orientation);
  bool isObjectGoingForward();
};