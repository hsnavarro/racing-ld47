#pragma once

#include "sfml.hpp"
#include "rigidbody.hpp"
#include "types.hpp"

class Car {
public:
  RigidBody properties;
  sf::RectangleShape shape;
  bool turnRight = false;
  bool turnLeft = false;
  bool goForward = false;
  bool goReverse = false;

  Car(sf::Vector2f, sf::Vector2f);

  void rotate(float);
  void simulate(float);
  void update(float);
};