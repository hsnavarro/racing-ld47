#include "game.hpp"

#include <cstdio>

#include "sfml.hpp"
#include "physics.hpp"
#include "algebra.hpp"
#include "utils.hpp"

Game::Game() : backgroundMusic { BACKGROUND_MUSIC_FILE }, ui { *this }, car { *this } {}

void Game::run() {
  setup();
  while (window.isOpen()) {
    switch (state) {
      case State::MAIN_MENU: {
        printf("main menu!\n");
        setupRacing();
        state = State::RACING;
      }
      break;

      case State::RACING: {
        handleEvents();
        updateRacing();
        renderRacing();
      }
      break;

      case State::END_GAME: {
        handleEvents();
        //updateRacing();
        renderRacing();
      }
      break;
    }

    window.display();
  }
}

void Game::setup() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
  window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racing Game", sf::Style::Default, settings);
  window.setFramerateLimit(DISPLAY_FPS);

  backgroundMusic.setVolume(75.f);
  //backgroundMusic.play();

  camera.setSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  camera.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

  frameClock.restart();
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
}

void Game::setupRacing() {
  currentCircuit = &circuits[0];
  currentCircuit->startRace();
  placeCamera();
  ghosts.clear();
  onCountdown = true;

  ui.setup();
}

void Game::updateRacing() {
  ui.update();

  if (onCountdown) {
    lapTime.restart();
  } else {
    f32 frameDuration = frameClock.restart().asSeconds();

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
}

void Game::renderRacing() {
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

  placeCamera();
  ui.render();
  window.setView(camera);
}

void Game::handleEventRacing(sf::Event& event) {
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

      case sf::Keyboard::J:
        car.rigidbody.applyPointAngularVelocity(10.0f);
      break;

      case sf::Keyboard::K:
        car.rigidbody.applyPointAngularVelocity(-10.0f);
      break;

      case sf::Keyboard::R:
        setupRacing();
      break;

      default:
      break;
    }
  }
}

void Game::handleEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) window.close();

    if (event.type == sf::Event::KeyPressed or event.type == sf::Event::KeyReleased) {
      switch (event.key.code) {
        // Todo(naum): remove on release
        case sf::Keyboard::Escape:
          window.close();
        break;
      }
    }

    if (state == State::RACING and !onCountdown)
      handleEventRacing(event);
  }
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
      state = State::END_GAME;
      currentCircuit = nullptr;
    } else {
      currentCircuit = &circuits[currentCircuitIndex];
    }
  }

  currentGhost.clear();
}

void Game::placeCamera() {
  camera.setCenter(to_vector2f(car.rigidbody.position));

  if (IS_VARIABLE_ZOOM_ACTIVE) {
    float targetZoom = static_cast<float>(0.002 * getMagnitude(car.rigidbody.linearVelocity) + 0.5);
    currentZoom = lerp(currentZoom, targetZoom, 0.2f);
  } else currentZoom = 0.5;

  camera.setSize(currentZoom * SCREEN_SIZE);
}
