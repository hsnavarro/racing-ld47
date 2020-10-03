#include "game.hpp"

Game::Game() : car{ INITIAL_CAR_POSITION, INITIAL_CAR_DIRECTION } {
  window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racing Game");
}

void Game::update(float timeElapsed) {
  car.update(timeElapsed);
}

void Game::render() {
  window.clear();
  window.draw(car.shape);
  window.display();
}

void Game::handleEvent(sf::Event& event) {
  if (event.type == sf::Event::Closed) window.close();

  if (event.type == sf::Event::KeyPressed or event.type == sf::Event::KeyReleased) {
    bool keepActive = (event.type == sf::Event::KeyPressed);

    switch (event.key.code) {
    case sf::Keyboard::W:
      car.goForward = keepActive;
      break;

    case sf::Keyboard::S:
      car.goReverse = keepActive;
      break;

    case sf::Keyboard::A:
      car.turnLeft = keepActive;
      break;

    case sf::Keyboard::D:
      car.turnRight = keepActive;
      break;
    }
  }
}