#pragma once

#include "rigidbody.hpp"
#include "algebra.hpp"
#include "consts.hpp"

const float MAX_PARTICLE_SIZE = 2.5;
const float MAX_PARTICLE_LIFE_TIME = 0.5;

enum class ParticleType { SMOKE, TIRE_TRACK };

class Particle {
public:
  Particle(float, float, float, ParticleType);

  void update(float);
  sf::Shape& shape();

  void setPosition(const sf::Vector2f&);

public:
  Rigidbody rigidbody;
  sf::RectangleShape rectangleShape;
  sf::CircleShape circleShape;
  ParticleType type;

  float lifeTime;
  bool isExpired = false;
  bool wasLaunched = false;
};
