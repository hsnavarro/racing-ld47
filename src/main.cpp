#include "sfml.hpp"
#include "game.hpp"
#include <iostream>
#include <cmath>

int main() {

  Game game;
  sf::Clock clock;
  sf::Event event;

  float timeToUpdateSimulation = (1 / float(SIMULATION_FPS));

  while (game.window.isOpen()) {
    float frameTime = clock.restart().asSeconds();

    while (frameTime > 0.0) {
      float deltaTime = std::min(frameTime, timeToUpdateSimulation);
      game.update(deltaTime);
      frameTime -= deltaTime;
    }

    game.render();

    while (game.window.pollEvent(event)) {
      game.handleEvent(event);
    }
  }
}