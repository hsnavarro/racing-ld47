#include "game.hpp"

#include "sfml.hpp"
#include "physics.hpp"

#include <cstdio>

Game::Game() :
    car { INITIAL_CAR_POSITION, INITIAL_CAR_DIRECTION },
    circuit { *this },
    smokeParticles { *this }
{

  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
  window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racing Game", sf::Style::Default, settings);
  window.setFramerateLimit(DISPLAY_FPS);

  circuit.setWalls(
    {
      {{ {  50.0f,  50.0f }, {  50.0f, 550.0f } }},
      {{ {  50.0f, 550.0f }, { 750.0f, 550.0f } }},
      {{ { 750.0f, 550.0f }, { 750.0f,  50.0f } }},
      {{ { 750.0f,  50.0f }, {  50.0f,  50.0f } }},

      {{ { 120.0f, 120.0f }, { 680.0f, 120.0f } }},
      {{ { 680.0f, 120.0f }, { 680.0f, 480.0f } }},
      {{ { 680.0f, 480.0f }, { 120.0f, 480.0f } }},
      {{ { 120.0f, 480.0f }, { 120.0f, 120.0f } }}
    }
  );

  circuit.setCheckpoints(
    {
      {{ { 400.0f,  50.0f }, { 400.0f, 120.0f } }},
      {{ { 680.0f, 300.0f }, { 750.0f, 300.0f } }},
      {{ { 400.0f, 480.0f }, { 400.0f, 550.0f } }},
      {{ {  50.0f, 300.0f }, { 120.0f, 300.0f } }}
    }
  );

  circuit.setTexture("assets/gfx/track-test.png");


  clock.restart();
  totalTime.restart();
}

void Game::update() {
  f32 frameDuration = clock.restart().asSeconds();

  while (frameDuration > 0.0) {
    float deltaTime = std::min(frameDuration, SIMULATION_DELTA_TIME);

    smokeParticles.update(deltaTime);
    car.update(deltaTime);

    physics::resolveCollisions(*this);
    circuit.update(deltaTime);

    smokeParticles.emissionFromCar(car);

    frameDuration -= deltaTime;
  }
}

void Game::render() {
  window.clear();

  circuit.render();
  smokeParticles.render();
  window.draw(car.shape);

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

        case sf::Keyboard::J:
          car.rigidbody.applyPointAngularVelocity(1.0f);

        default:
          break;
      }
    }
  }
}

f32 Game::getTime() const {
  return totalTime.getElapsedTime().asSeconds();
}

void Game::completeLap() {
  printf("Lap complete\n");
}
