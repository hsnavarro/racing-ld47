#pragma once

#include "car.hpp"

class Game {
public:
  Car car;
  sf::RenderWindow window;

  // test
  sf::Texture texture;
  sf::Sprite sprite;

  Game();

  void update(float timeElapsed);
  void render();
  void handleEvent(sf::Event& event);
};