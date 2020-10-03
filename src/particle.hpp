#pragma once

#include "car.hpp"
#include "algebra.hpp"
#include "consts.hpp"

class Particle {
public:
  Particle();

  void update(float);

  RigidBody properties;
  sf::CircleShape shape;
  float lifeTime;
  bool isExpired = false;
  bool wasLaunched = false;
};