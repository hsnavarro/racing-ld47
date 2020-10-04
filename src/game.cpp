#include "game.hpp"

#include <cstdio>

#include "sfml.hpp"
#include "physics.hpp"
#include "algebra.hpp"
#include "utils.hpp"

Game::Game() :
    car { INITIAL_CAR_POSITION, INITIAL_CAR_DIRECTION, *this }
{

  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
  window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racing Game", sf::Style::Default, settings);
  window.setFramerateLimit(DISPLAY_FPS);

  sf::View cameraView;
  sf::View uiView;
  sf::View minimapView;
  //TODO: encapsulate UI stuff
  //
  uiView.setSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  uiView.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

  cameraView.setSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  cameraView.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

  minimapView.setSize(2*SCREEN_WIDTH,2*SCREEN_HEIGHT);
  minimapView.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
  minimapView.setViewport(sf::FloatRect(0.80f, 0.05f, 0.40f, 0.40f));

  camera = cameraView;
  ui = uiView;
  minimap = minimapView;

  if (!font.loadFromFile("assets/fonts/Monocons.ttf")) {
    printf("fail to load font!\n");
  }

  clock.restart();
  totalTime.restart();
  lapTime.restart();

  // Circuits
  Circuit::loadAtlas();

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

  /*
  for (size_t i = 0; i < currentCircuitIndex; i++)
    circuits[i].render();
    */

  if (currentCircuit)
    currentCircuit->render(camera);

  for(auto& ghost : ghosts) ghost.render(window);
  car.render(camera);

  circuit.render(minimap);
  car.renderIcon(minimap);

  // UI
  char text[30];

  placeCamera();

  window.setView(ui);

  const auto maximumSize = [this] {
    sf::Text tmp("000.00s", font, UI_DEFAULT_FONT_SIZE);
    return tmp.getLocalBounds().width;
  }();

  float verticalOffset = 0.0f;

  snprintf(text, 30, "%.2fs", lapTime.getElapsedTime().asSeconds());
  sf::Text lapTimeText(text, font, UI_DEFAULT_FONT_SIZE);
  drawTextRight(lapTimeText, maximumSize, 10.0f, window);

  verticalOffset += UI_DEFAULT_MARGIN + lapTimeText.getLocalBounds().height;

  if (lastLapTime > 0.0f) {
    snprintf(text, 30, "%.2fs", lastLapTime);
    sf::Text lastLapText(text, font, UI_DEFAULT_FONT_SIZE);
    drawTextRight(lastLapText, maximumSize, 10.0f+verticalOffset, window);
    verticalOffset += UI_DEFAULT_MARGIN + lastLapText.getLocalBounds().height;
  }


  snprintf(text, 30, "%.2fs", circuit.lapTimeLimit);
  sf::Text lapTimeLimitText(text, font, UI_DEFAULT_FONT_SIZE);
  drawTextRight(lapTimeLimitText, maximumSize, 10.0f+verticalOffset, window);

  snprintf(text, 30, "%.2f px/s", getMagnitude(car.rigidbody.linearVelocity));
  sf::Text speedText(text, font, UI_DEFAULT_FONT_SIZE);
  drawTextRight(speedText, SCREEN_WIDTH - 10.0f, SCREEN_HEIGHT - 40.0f, window);

  window.setView(camera);
  window.display();
}

void Game::placeCamera() {
  camera.setCenter(to_vector2f(car.rigidbody.position));

  //TODO(Naum): add sigmoid
  float targetZoom = static_cast<float>(0.002 * getMagnitude(car.rigidbody.linearVelocity) + 0.5);
  currentZoom = lerp(currentZoom, targetZoom, 0.2f);
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
          car.isDriftActive = keepActive;
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
          currentCircuit = &circuits[0];
          currentCircuit->startRace();
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
