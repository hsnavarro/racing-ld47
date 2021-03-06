#include "game.hpp"

#include <cstdio>
#include <iostream>

#include "sfml.hpp"
#include "physics.hpp"
#include "algebra.hpp"

Game::Game() : ui{ *this }, car{ *this } {}

void Game::run() {
  setup();
  while (window.isOpen()) {
    switch (state) {
    case State::MAIN_MENU: {
      handleEvents();
      renderMainMenu();
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
      updateEndGame();
      renderRacing();
    }
                        break;
    }

    window.display();
  }
}

void Game::setupEndGame() {
  circuits.clear();

  Circuit circuit{ *this };
  circuit.loadFromFile("assets/circuits/escape.cir");

  auto checkpoint = circuit.checkpoints.back();
  circuit.checkpoints.clear();
  circuit.checkpoints.push_back(checkpoint);

  circuits.push_back(circuit);

  /*
  const auto w = static_cast<int>(circuits[0].tiles[0].size() * CIRCUIT_TILE_SIZE);
  const auto h = static_cast<int>(circuits[0].tiles.size() * CIRCUIT_TILE_SIZE);

  if (!circuitRenderTexture.create(w, h)) {
    printf("Could not create circuit render texture!\n");
  }

  circuitRenderTexture.clear(sf::Color::Transparent);
  circuits[0].draw(circuitRenderTexture);
  circuitSprite.setTexture(circuitRenderTexture.getTexture());
  circuitSprite.setTextureRect({ 0, h, w, -h });
  */

  roadTopRenderTexture.clear(sf::Color::Transparent);

  currentCircuit = &circuits[0];
  currentCircuit->startRace();
  ghosts.clear();

  car.rigidbody.setForwardDrag(0.0);
}

void Game::updateEndGame() {
  updateRacing();
}

void Game::setup() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
  window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racing Game", sf::Style::Titlebar | sf::Style::Close, settings);
  window.setFramerateLimit(DISPLAY_FPS);

  audioSystem.backgroundMusic.setVolume(20.f);
  audioSystem.backgroundMusic.play();

  audioSystem.engineStartFX.setVolume(30.f);
  audioSystem.engineStartFX.play();

  audioSystem.engineFX.setVolume(30.0f);

  camera.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  camera.setCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

  frameClock.restart();
  lapTime.restart();

  // Circuits
  Circuit::loadAtlas();
}

void Game::setupRacing() {
  // Circuits
  Circuit circuit{ *this };

  circuit.loadFromFile("assets/circuits/back-forth-0.cir");
  circuits.push_back(circuit);

  circuit.loadFromFile("assets/circuits/back-forth-1.cir");
  circuits.push_back(circuit);

  circuit.loadFromFile("assets/circuits/back-forth-2.cir");
  circuits.push_back(circuit);

  circuit.loadFromFile("assets/circuits/back-forth-3.cir");
  circuits.push_back(circuit);

  circuit.loadFromFile("assets/circuits/back-forth-4.cir");
  circuits.push_back(circuit);

  circuit.loadFromFile("assets/circuits/back-forth-5.cir");
  circuits.push_back(circuit);

  circuit.loadFromFile("assets/circuits/back-forth-6.cir");
  circuits.push_back(circuit);

  // Render targets
  const auto w = static_cast<int>(circuits[0].tiles[0].size() * CIRCUIT_TILE_SIZE);
  const auto h = static_cast<int>(circuits[0].tiles.size() * CIRCUIT_TILE_SIZE);

  if (!circuitRenderTexture.create(w, h)) {
    printf("Could not create circuit render texture!\n");
  }

  circuitRenderTexture.setSmooth(true);
  circuitRenderTexture.clear(sf::Color::Transparent);
  circuits[0].draw(circuitRenderTexture);
  circuitSprite.setTexture(circuitRenderTexture.getTexture());
  circuitSprite.setTextureRect({ 0, h, w, -h });

  if (!roadTopRenderTexture.create(w, h)) {
    printf("Could not create roadTop render texture!\n");
  }

  roadTopRenderTexture.setSmooth(true);
  roadTopRenderTexture.clear(sf::Color::Transparent);
  roadTopSprite.setTexture(roadTopRenderTexture.getTexture());
  roadTopSprite.setTextureRect({ 0, h, w, -h });


  //
  currentCircuit = &circuits[0];
  currentCircuit->startRace();
  placeCamera();
  ghosts.clear();
  onCountdown = true;

  ui.setup();
}

void Game::updateRacing() {
  audioSystem.backgroundMusic.play();
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
        currentCircuit->update();

      displayDeltaTime -= deltaTime;
    }

    car.updateParticles(frameDuration);
    currentGhost.addState(*this);
  }
}

void Game::renderMainMenu() {
  window.clear();
  ui.render();
  window.setView(camera);
}

void Game::renderRacing() {
  window.clear(sf::Color(0, 100, 0, 1));

  // Circuit
  if (state != State::END_GAME) {
    window.draw(circuitSprite);
    window.draw(roadTopSprite);
  }

  if (currentCircuit)
    currentCircuit->render(camera);

  for (auto& ghost : ghosts) ghost.render(window);
  car.render(camera);

  placeCamera();
  ui.render();
  window.setView(camera);
}

// A -> handbrake
// left stick -> turn left / right
// LT -> accelerate
// RT -> break / reverse

void Game::handleEventRacing(const sf::Event& event) {
  if (event.type == sf::Event::JoystickButtonPressed or event.type == sf::Event::JoystickButtonReleased) {

    const bool keepActive = event.type == sf::Event::JoystickButtonPressed;

    switch (event.joystickButton.button) {
    case XBOX_BUTTONS::A:
      if (!car.isHandBrakeActive) audioSystem.handbrakeFX.play();
      car.isHandBrakeActive = keepActive;
      break;
    }
  }

  if (event.type == sf::Event::JoystickMoved) {
    float leftStick = sf::Joystick::getAxisPosition(JOYSTICK_ID, sf::Joystick::X);
    float leftTrigger = sf::Joystick::getAxisPosition(JOYSTICK_ID, sf::Joystick::Z);
    float rightTrigger = sf::Joystick::getAxisPosition(JOYSTICK_ID, sf::Joystick::R);

    car.applyGoForward(::getRatio(-100.f, 100.f, rightTrigger));
    car.applyGoReverse(::getRatio(-100.f, 100.f, leftTrigger));
    car.applyTurn(leftStick/100.f);

    if (fabs(leftStick) < ANALOG_TOLERANCE) car.applyTurn(0.f);
  }

  if (event.type == sf::Event::KeyPressed or event.type == sf::Event::KeyReleased) {

    const bool keepActive = event.type == sf::Event::KeyPressed;
    const float ratio = keepActive ? 1.f : 0.f;

    switch (event.key.code) {
    case sf::Keyboard::W:
    case sf::Keyboard::I:
    case sf::Keyboard::N:
    case sf::Keyboard::Up:
      car.applyGoForward(ratio);
      break;

    case sf::Keyboard::S:
    case sf::Keyboard::K:
    case sf::Keyboard::M:
    case sf::Keyboard::Down:
      car.applyGoReverse(ratio);
      break;

    case sf::Keyboard::A:
    case sf::Keyboard::J:
    case sf::Keyboard::X:
    case sf::Keyboard::Left:
      car.applyTurn(-ratio);
      break;

    case sf::Keyboard::D:
    case sf::Keyboard::L:
    case sf::Keyboard::C:
    case sf::Keyboard::Right:
      car.applyTurn(ratio);
      break;

    case sf::Keyboard::Space:
      if (!car.isHandBrakeActive) audioSystem.handbrakeFX.play();
      car.isHandBrakeActive = keepActive;
      break;

    case sf::Keyboard::T:
      car.rigidbody.applyPointAngularVelocity(10.0f);
      break;

    case sf::Keyboard::Y:
      car.rigidbody.applyPointAngularVelocity(-10.0f);
      break;

    default:
      break;
    }
  }
}

void Game::handleEventMainMenu(const sf::Event& event) {
  if (event.type == sf::Event::JoystickButtonPressed) {

    switch (event.joystickButton.button) {
    case XBOX_BUTTONS::A:
    case XBOX_BUTTONS::START:
      setupRacing();
      state = State::RACING;
      break;

    default:
      break;
    }
  }

  if (event.type == sf::Event::KeyPressed) {

    switch (event.key.code) {
    case sf::Keyboard::Space:
    case sf::Keyboard::Enter:
      setupRacing();
      state = State::RACING;
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

    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::P) audioSystem.toggleMute();
    }

    if ((state == State::RACING or state == State::END_GAME) and !onCountdown and !hasEscaped)
      handleEventRacing(event);

    if (state == State::MAIN_MENU)
      handleEventMainMenu(event);
  }
}

void Game::completeLap() {
  if (!currentCircuit) return;

  lastLapTime = lapTime.restart().asSeconds();

  if (state == State::END_GAME) {
    ui.clock.restart();
    hasEscaped = true;
    return;
  }

  currentCircuit->resetCheckpoints();
  currentGhost.completeLap(lastLapTime);

  // successful lap
  if (lastLapTime <= currentCircuit->lapTimeLimit) {
    newGhosts.push_back(currentGhost);

    currentCircuitIndex++;
    if (currentCircuitIndex == circuits.size()) {
      setupEndGame();
      state = State::END_GAME;

    } else {
      currentCircuit = &circuits[currentCircuitIndex];

      // apply circuit texture
      circuitSprite.setColor({ 192, 192, 192, 255 });
      circuitRenderTexture.draw(circuitSprite);
      circuitSprite.setColor({ 255, 255, 255, 255 });

      currentCircuit->draw(circuitRenderTexture);
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
