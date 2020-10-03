#include "game.hpp"
#include "sfml.hpp"

#include <iostream>

const std::string PATH_TO_IMAGE = "./imgs/test-circuit.jpg";

Game::Game() : car{ INITIAL_CAR_POSITION, INITIAL_CAR_DIRECTION } {
  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
  window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racing Game", sf::Style::Default, settings);
  window.setFramerateLimit(DISPLAY_FPS);

  ParticleSystem smokeParticles;

  // test
  if(texture.loadFromFile(PATH_TO_IMAGE)) {
    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setScale(2.5, 2);
  }

  clock.restart();
  totalTime.restart();
}

void Game::update() {
  f32 frameDuration = clock.restart().asSeconds();

  while (frameDuration > 0.0) {
    float deltaTime = std::min(frameDuration, SIMULATION_DELTA_TIME);

    smokeParticles.update(deltaTime);
    car.update(deltaTime);

    smokeParticles.emissionFromCar(car);

    frameDuration -= deltaTime;
  }
}

void Game::render() {
  window.clear();

  // test
  window.draw(sprite);

  window.draw(car.shape);

  for(auto& particle : smokeParticles.particles) 
    window.draw(particle.shape);

  window.display();
}

void Game::handleEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
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

        // Todo(naum): remove on release
        case sf::Keyboard::Escape:
          window.close();
          break;

        default:
          break;
      }
    }
  }
}

f32 Game::getTime() const {
  return totalTime.getElapsedTime().asSeconds();
}
