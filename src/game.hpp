#pragma once

#include "types.hpp"
#include "car.hpp"

class Game {
public:
  sf::Clock totalTime;
  sf::Clock clock;
  Car car;
  sf::RenderWindow window;

  // test
  sf::Texture texture;
  sf::Sprite sprite;

  Game();

  void update();
  void render();
  void handleEvents();
  f32  getTime() const;
};
