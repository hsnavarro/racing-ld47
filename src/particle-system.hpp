#pragma once

#include "particle.hpp"

const int MAX_NUM_PARTICLES_GENERATED = 1;
const int MAX_NUM_PARTICLES = 500;

class Game;

class ParticleSystem {
public:
  ParticleSystem(ParticleType, Game&, const int);

  void update(float);
  void updateIndex();
  void render();
  void emissionFromPoint(const sf::Vector2f&, const sf::Vector2f&, const float = 1.0f);
  void emissionFromLine(const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&, const float = 1.0f);

  void emitToTexture(const sf::Vector2f&, sf::RenderTexture&);

public:
  int indexOfLastActiveParticle = -1;

  std::vector<Particle> particles;
  ParticleType particlesType;

  Game& game;
};
