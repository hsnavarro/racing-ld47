#pragma once

#include "particle.hpp"

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
