#pragma once

#include "sfml.hpp"
#include "rigidbody.hpp"
#include "types.hpp"
#include "consts.hpp"

const float CAR_ENGINE_ACCELERATION  = 80;
const float CAR_BRAKE_ACCELERATION   = -150;
const float CAR_REVERSE_ACCELERATION = -20;

const float CAR_ANGULAR_VELOCITY = 2;

const float CAR_FORWARD_DRAG = 0.02f;
const float CAR_LATERAL_DRAG = 0.9999f;
const float CAR_ANGULAR_DRAG = 0.99f;

const int CAR_WIDTH  = 10;
const int CAR_HEIGHT = 20;

class Car {
public:
  Car(sf::Vector2f, sf::Vector2f);

  void update(float);

  void resolveCollision(sf::Vector2f collisionVector);

public:
  Rigidbody rigidbody;
  sf::RectangleShape shape;

  bool turnRight = false;
  bool turnLeft = false;
  bool goForward = false;
  bool goReverse = false;

  // These are constants!
  float engineAcceleration = CAR_ENGINE_ACCELERATION;
  float brakeAcceleration = CAR_BRAKE_ACCELERATION;
  float reverseAcceleration = CAR_REVERSE_ACCELERATION;
  float angularVelocity = CAR_ANGULAR_VELOCITY;
};
