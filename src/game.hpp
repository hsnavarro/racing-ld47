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
  void placeCamera();

  f32  getTime() const;
  void completeLap();

  sf::RenderWindow window;
  sf::Font font;
  sf::View camera;
  sf::View ui;

  sf::Clock totalTime;
  sf::Clock clock;
  sf::Clock lapTime;

  Car car;
  Circuit circuit;

  ParticleSystem smokeParticles;
};
