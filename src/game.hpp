#pragma once

#include "types.hpp"
#include "car.hpp"
#include "particle-system.hpp"
#include "physics.hpp"

class Game {
public:
  Game();

  void update();
  void render();
  void handleEvents();
  f32  getTime() const;

  sf::Clock totalTime;
  sf::Clock clock;
  Car car;
  sf::RenderWindow window;
  ParticleSystem smokeParticles;

  // test
  sf::Texture texture;
  sf::Sprite sprite;

  sf::Line line {{ { 600, 500 }, { 600, 100 } }};
};
