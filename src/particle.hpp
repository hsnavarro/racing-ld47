#pragma once

#include "rigidbody.hpp"
#include "algebra.hpp"
#include "consts.hpp"

const float MAX_PARTICLE_SIZE = 2.5f;
const float MAX_PARTICLE_LIFE_TIME = 0.2f;

enum class ParticleType { SMOKE, TIRE_TRACK, NOT_DEFINED };

class Particle {
public:
  Particle(const float = 0.0f, const float = 0.0f, const float = 0.0f, const ParticleType = ParticleType::NOT_DEFINED);

  void update(const float);
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
