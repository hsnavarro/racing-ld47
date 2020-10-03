#include "game.hpp"
#include "sfml.hpp"

const std::string PATH_TO_IMAGE = "./imgs/test-circuit.jpg";

Game::Game() : car{ INITIAL_CAR_POSITION, INITIAL_CAR_DIRECTION } {
  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
  window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racing Game", sf::Style::Default, settings);
  window.setFramerateLimit(DISPLAY_FPS);

  // test
  if(texture.loadFromFile(PATH_TO_IMAGE)) {
    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setScale(3.0, 2.5);
  }

}

void Game::update(float timeElapsed) {
  car.update(timeElapsed);
}

void Game::render() {
  window.clear();

  // test
  window.draw(sprite);

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

    default:
      break;
    }
  }
}