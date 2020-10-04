#include "game.hpp"

#include <cstdio>

#include "sfml.hpp"
#include "physics.hpp"
#include "algebra.hpp"
#include "utils.hpp"

Game::Game() : car { *this }, ui { *this } {}

void Game::setup() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
  window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racing Game", sf::Style::Default, settings);
  window.setFramerateLimit(DISPLAY_FPS);

  camera.setSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  camera.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

  if (!font.loadFromFile("assets/fonts/Monocons.ttf")) {
    printf("fail to load font!\n");
  }

  clock.restart();
  totalTime.restart();
  lapTime.restart();

  // Circuits
  Circuit::loadAtlas();

  /* Test Physics

  {
    Circuit circuit {*this};
    circuit.loadFromFile("assets/circuits/test-circuit.cir");
    circuits.push_back(circuit);
  }

  */

  {
    Circuit circuit {*this};
    circuit.loadFromFile("assets/circuits/progress-0.cir");
    circuits.push_back(circuit);
  }

  {
    Circuit circuit {*this};
    circuit.loadFromFile("assets/circuits/progress-1.cir");
    circuits.push_back(circuit);
  }

  {
    Circuit circuit {*this};
    circuit.loadFromFile("assets/circuits/progress-2.cir");
    circuits.push_back(circuit);
  }

  currentCircuit = &circuits[0];
  currentCircuit->startRace();

  placeCamera();
}

void Game::update() {
  f32 frameDuration = clock.restart().asSeconds();

  float displayDeltaTime = frameDuration;

  while (displayDeltaTime > 0.0) {
    float deltaTime = std::min(displayDeltaTime, SIMULATION_DELTA_TIME);

    car.update(deltaTime);

    physics::resolveCollisions(*this);
    if (currentCircuit)
      currentCircuit->update(deltaTime);

    displayDeltaTime -= deltaTime;
  }

  car.updateParticles(frameDuration);
  currentGhost.addState(*this);
}

void Game::render() {
  window.clear();

  // Circuit
  /*
  for (size_t i = 0; i < currentCircuitIndex; i++)
    circuits[i].render();
    */

  if (currentCircuit)
    currentCircuit->render(camera);

  for(auto& ghost : ghosts) ghost.render(window);
  car.render(camera);

  //
  ui.render();

  //
  placeCamera();
  window.setView(camera);
  window.display();
}

void Game::placeCamera() {
  camera.setCenter(to_vector2f(car.rigidbody.position));

  if(IS_VARIABLE_ZOOM_ACTIVE) {
    //TODO(Naum): add sigmoid
    float targetZoom = static_cast<float>(0.002 * getMagnitude(car.rigidbody.linearVelocity) + 0.5);
    currentZoom = lerp(currentZoom, targetZoom, 0.2f);
  } else currentZoom = 0.5;

  camera.setSize(currentZoom * SCREEN_SIZE);
}

void Game::handleEvents() {
  sf::Event event;
  sf::Vector2f size;
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

        case sf::Keyboard::Space:
          car.isHandBrakeActive = keepActive;
        break;

        // Todo(naum): remove on release
        case sf::Keyboard::Escape:
          window.close();
        break;

        case sf::Keyboard::J:
          car.rigidbody.applyPointAngularVelocity(10.0f);
        break;

        case sf::Keyboard::K:
          car.rigidbody.applyPointAngularVelocity(-10.0f);
        break;

        case sf::Keyboard::R:
          if (currentCircuit) {
            currentCircuit = &circuits[0];
            currentCircuit->startRace();
          }
          ghosts.clear();
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

void Game::completeLap() {
  if (!currentCircuit) return;

  lastLapTime = lapTime.restart().asSeconds();

  currentCircuit->resetCheckpoints();
  currentGhost.completeLap(lastLapTime);


  // successful lap
  if (lastLapTime <= currentCircuit->lapTimeLimit) {
    newGhosts.push_back(currentGhost);

    currentCircuitIndex++;
    if (currentCircuitIndex == circuits.size()) {
      printf("win!\n");
      currentCircuit = nullptr;
    } else {
      currentCircuit = &circuits[currentCircuitIndex];
    }
  }

  currentGhost.clear();
}
