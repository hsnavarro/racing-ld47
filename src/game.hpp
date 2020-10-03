#pragma once

#include "car.hpp"

class Game {
public:
  Car car;
  sf::RenderWindow window;

  Game();

  void update(float timeElapsed);
  void render();
  void handleEvent(sf::Event& event);
};