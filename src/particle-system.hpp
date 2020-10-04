#pragma once

#include "particle.hpp"

const int MAX_NUM_PARTICLES = 15;

class Game;

class ParticleSystem {
public:
  ParticleSystem(Game&);

  void update(float);
  void render();
  void emissionFromCar(Car&);

  std::vector<Particle> particles;

  Game& game;
};
