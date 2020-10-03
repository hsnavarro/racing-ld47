#pragma once

#include "car.hpp"
#include "algebra.hpp"
#include "consts.hpp"

const float MAX_PARTICLE_RADIUS = 2.5;
const int MAX_PARTICLE_LIFE_TIME = 1;
const int MAX_NUM_PARTICLES = 25;

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
