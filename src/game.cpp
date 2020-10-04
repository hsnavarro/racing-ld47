#include "game.hpp"

#include <cstdio>

#include "sfml.hpp"
#include "physics.hpp"
#include "algebra.hpp"
#include "utils.hpp"

Game::Game() :
    car { INITIAL_CAR_POSITION, INITIAL_CAR_DIRECTION, *this },
    circuit { *this },
    currentGhost { *this }
{

  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
  window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racing Game", sf::Style::Default, settings);
  window.setFramerateLimit(DISPLAY_FPS);

  sf::View cameraView;
  sf::View uiView;
  //TODO: encapsulate UI stuff
  uiView.setSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  uiView.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
  cameraView.setSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  cameraView.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
  camera = cameraView;
  ui = uiView;

  if (!font.loadFromFile("assets/fonts/Monocons.ttf")) {
    printf("fail to load font!\n");
  }

  clock.restart();
  totalTime.restart();
  lapTime.restart();

  // Test
  Circuit::loadAtlas();
  //circuit.loadFromFile("assets/circuits/test.cir");
  circuit.loadFromFile("assets/circuits/small.cir");
  //circuit.loadFromFile("assets/circuits/cross.cir");
  circuit.setLapTimeLimit(50.0f);

  circuit.startRace();
}

void Game::update() {
  f32 frameDuration = clock.restart().asSeconds();

  float displayDeltaTime = frameDuration;

  while (displayDeltaTime > 0.0) {
    float deltaTime = std::min(displayDeltaTime, SIMULATION_DELTA_TIME);

    car.update(deltaTime);

    physics::resolveCollisions(*this);
    circuit.update(deltaTime);

    displayDeltaTime -= deltaTime;
  }

  car.updateParticles(frameDuration);
  currentGhost.addState();
}

void Game::render() {
  window.clear();

  circuit.render();
  for(auto& ghost : ghosts) ghost.render();
  car.render();

  // UI
  char text[30];

  placeCamera();

  window.setView(ui);

  const auto maximumSize = [this] {
    sf::Text tmp("000.00 s", font, 20);
    return tmp.getLocalBounds().width;
  }();

  snprintf(text, 30, "%.2f s", lapTime.getElapsedTime().asSeconds());
  sf::Text lapTimeText(text, font, 20);
  drawTextRight(lapTimeText, maximumSize + 5.0f, 10.0f, window);

  snprintf(text, 30, "%.2f px/s", getMagnitude(car.rigidbody.linearVelocity));
  sf::Text speedText(text, font, 20);
  drawTextRight(speedText, SCREEN_WIDTH - 10.0f, SCREEN_HEIGHT - 40.0f, window);

  window.setView(camera);
  window.display();
}

void Game::placeCamera() {
  camera.setCenter(to_vector2f(car.rigidbody.position));

  //TODO(Naum): add sigmoid
  float zoomValue = static_cast<float>(0.002 * getMagnitude(car.rigidbody.linearVelocity) + 0.5);
  camera.setSize(zoomValue*SCREEN_SIZE);
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
  circuit.resetCheckpoints();
  float lapTimeTaken = lapTime.restart().asSeconds();

  currentGhost.completeLap(lapTimeTaken);
  if(lapTimeTaken <= circuit.lapTimeLimit) ghosts.push_back(currentGhost);
  currentGhost.clear();
}
