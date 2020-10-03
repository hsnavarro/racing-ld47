#include "game.hpp"

#include "sfml.hpp"
#include "physics.hpp"

const std::string PATH_TO_IMAGE = "assets/gfx/test-circuit.jpg";

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
    physics::resolveCollisions(*this);

    smokeParticles.emissionFromCar(car);

    frameDuration -= deltaTime;
  }
}

void Game::render() {
  window.clear();

  window.draw(car.shape);

  // test
  //window.draw(sprite);

  for(auto& particle : smokeParticles.particles) 
    window.draw(particle.shape);

  //
  sf::Vertex verts[] = { line[0], line[1] };
  window.draw(verts, 2, sf::Lines);

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
