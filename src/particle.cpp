#include "particle.hpp"

#include <random>

Particle::Particle() {
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());
  std::uniform_real_distribution<> lifeTimeRand(0, MAX_PARTICLE_LIFE_TIME);
  lifeTime = lifeTimeRand(generator);
  std::uniform_real_distribution<> particleRadiusRand(0, MAX_PARTICLE_RADIUS);
  shape.setRadius(particleRadiusRand(generator));
}

void Particle::update(float timeElapsed) {
  lifeTime -= timeElapsed;
  if (lifeTime < 0.0f) isExpired = true;
}