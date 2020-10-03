#pragma once

#include "types.hpp"
#include "car.hpp"
#include "particle-system.hpp"
#include "physics.hpp"
#include "circuit.hpp"

class Game {
public:
  Game();

  void update();
  void render();
  void handleEvents();

  f32  getTime() const;
  void completeLap();

  sf::RenderWindow window;
  sf::Clock totalTime;
  sf::Clock clock;
  Car car;
  Circuit circuit;

  ParticleSystem smokeParticles;
};
