#include "particle-system.hpp"
#include "game.hpp"

#include <random>

Particle createParticle(ParticleType type) {

  if (type == ParticleType::SMOKE) {
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());

    std::uniform_real_distribution<float> lifeTimeRand(0, MAX_PARTICLE_LIFE_TIME);
    float lifeTime = lifeTimeRand(generator);

    std::uniform_real_distribution<float> particleSizeRand(0, MAX_PARTICLE_SIZE);
    float particleSize = particleSizeRand(generator);

    return Particle(lifeTime, particleSize, particleSize, type);

  } else {

    float lifeTime = 5.0f;
    float particleSize = 2.0;

    return Particle(lifeTime, particleSize, particleSize, type);
  }
}

ParticleSystem::ParticleSystem(ParticleType type, Game& _game) : particlesType{ type }, game{ _game } {
  while (particles.size() != MAX_NUM_PARTICLES_GENERATED) {
    particles.push_back(createParticle(particlesType));
  }
}

void ParticleSystem::update(float deltaTime) {
  for (auto& particle : particles) particle.update(deltaTime);

  std::vector<Particle> newParticles;
  for (auto& particle : particles)
    if (!particle.isExpired) newParticles.push_back(particle);

  for(int i = 0; i < MAX_NUM_PARTICLES_GENERATED; i++) {
    if(MAX_NUM_PARTICLES == (int) newParticles.size() ) break;
    newParticles.push_back(createParticle(particlesType));
  }

  particles = newParticles;
}

void ParticleSystem::emissionFromPoint(const sf::Vector2f& point, const sf::Vector2f& direction, float emissionRate) {

  int numberOfParticles = static_cast<int>(particles.size());
  int particlesLaunched = static_cast<int>(emissionRate * numberOfParticles);

  for (auto& particle : particles) {
    if (particle.wasLaunched) continue;
    if (!particlesLaunched) return;

    particlesLaunched--;

    particle.rigidbody.position = to_vector2f64(point);
    particle.rigidbody.direction = -to_vector2f64(direction);

    particle.setPosition(point);
    particle.wasLaunched = true;
  }
}

void ParticleSystem::emitToTexture(const sf::Vector2f& point, sf::RenderTexture& renderTexture) {
  particles[0].setPosition(point);
  renderTexture.draw(particles[0].shape());
}

void ParticleSystem::emissionFromLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& direction, float emissionRate) {

  const sf::Vector2f lineDirection = b - a;

  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());
  std::uniform_real_distribution<> doubleRand(0.0, 1.0);

  int numberOfParticles = static_cast<int>(particles.size());
  int particlesLaunched = static_cast<int>(emissionRate * numberOfParticles);

  for (auto& particle : particles) {
    if (particle.wasLaunched) continue;
    if (!particlesLaunched) return;

    particlesLaunched--;

    const sf::Vector2f particlePosition = a + float(doubleRand(generator)) * lineDirection;
    particle.rigidbody.position = to_vector2f64(particlePosition);
    particle.rigidbody.direction = -to_vector2f64(direction);

    particle.setPosition(particlePosition);
    particle.wasLaunched = true;
  }

}

void ParticleSystem::render() {
  for (auto& particle : particles) {
    if (particle.wasLaunched) game.window.draw(particle.shape());
  }
}
