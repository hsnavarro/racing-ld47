#pragma once

#include "sfml.hpp"
#include "rigidbody.hpp"
#include "types.hpp"
#include "consts.hpp"

class Car {
public:
  RigidBody properties;
  sf::RectangleShape shape;
  bool turnRight = false;
  bool turnLeft = false;
  bool goForward = false;
  bool goReverse = false;
  float engineAcceleration = CAR_ENGINE_ACCELERATION;
  float brakeAcceleration = CAR_BRAKE_ACCELERATION;
  float reverseAcceleration = CAR_REVERSE_ACCELERATION;
  float angularVelocity = CAR_ANGULAR_VELOCITY;

  Car(sf::Vector2f, sf::Vector2f);

  void rotate(float);
  void simulate(float);
  void update(float);
};