#pragma once

#include "car.hpp"
#include "algebra.hpp"
#include "consts.hpp"

const float MAX_PARTICLE_RADIUS = 2.5;
const float MAX_PARTICLE_LIFE_TIME = 0.3;

class Particle {
public:
  Particle();

  void update(float);

  Rigidbody rigidbody;
  sf::CircleShape shape;
  float lifeTime;
  bool isExpired = false;
  bool wasLaunched = false;
};
