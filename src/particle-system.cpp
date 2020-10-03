#include "particle-system.hpp"
#include <random>

std::random_device rd;

ParticleSystem::ParticleSystem() {
  while (particles.size() != MAX_NUM_PARTICLES) particles.push_back(Particle());
}

void ParticleSystem::renew() {
  std::vector<Particle> newParticle;
  for (auto& particle : particles)
    if (!particle.isExpired) newParticle.push_back(particle);

  while (newParticle.size() != MAX_NUM_PARTICLES)
    newParticle.push_back(Particle());

  particles = newParticle;
}

void ParticleSystem::update(float deltaTime) {
  for (auto& particle : particles) particle.update(deltaTime);
  renew();
}

void ParticleSystem::emissionFromCar(Car& car) {
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());

  const auto transform = car.shape.getTransform();

  const sf::Vector2f bottomLeftPoint = transform.transformPoint(car.shape.getPoint(3));
  const sf::Vector2f bottomRightPoint = transform.transformPoint(car.shape.getPoint(2));

  const sf::Vector2f direction = bottomRightPoint - bottomLeftPoint;

  std::uniform_real_distribution<> doubleRand(0.0, 1.0);

  int numberOfParticles = static_cast<int>(particles.size());
  const float PARTICLES_EMISSION_RATIO = 1 / 2500.0f;
  int particlesLaunched = static_cast<int>(PARTICLES_EMISSION_RATIO *
                                           getMagnitude(car.rigidbody.linearVelocity) *
                                           numberOfParticles);

  for(auto& particle : particles) {
    if(particle.wasLaunched) continue;
    if(!particlesLaunched) return;

    particlesLaunched--;

    const sf::Vector2f particlePosition = bottomLeftPoint + float(doubleRand(generator)) * direction;
    particle.rigidbody.position = particlePosition;
    particle.rigidbody.direction = -car.rigidbody.direction;

    const float ballRadius = particle.shape.getRadius();

    particle.shape.setOrigin(ballRadius * 0.5f, ballRadius * 0.5f);
    particle.shape.setPosition(particlePosition);

    particle.wasLaunched = true;
  }
}
