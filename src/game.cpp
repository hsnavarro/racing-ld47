#include "game.hpp"

#include "sfml.hpp"
#include "physics.hpp"
#include "algebra.hpp"

#include <cstdio>

Game::Game() :
    car { INITIAL_CAR_POSITION, INITIAL_CAR_DIRECTION, *this },
    circuit { *this }
{

  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
  window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racing Game", sf::Style::Default, settings);
  window.setFramerateLimit(DISPLAY_FPS);

  sf::View cameraView;
  sf::View uiView;
  //TODO: encapsulate UI stuff
  uiView.setSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  uiView.setCenter(SCREEN_WIDTH+SCREEN_WIDTH/2,SCREEN_HEIGHT+SCREEN_HEIGHT/2);
  cameraView.setSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  cameraView.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
  camera = cameraView;

  if (!font.loadFromFile("assets/fonts/arial.ttf")) {
    printf("fail to load font!\n");
  }

  clock.restart();
  totalTime.restart();
  lapTime.restart();

  ghosts.push_back(Ghost(*this));

  // Test
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
  circuit.setLapTimeLimit(50.0f);
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
  ghosts.back().addState();
}

void Game::render() {
  window.clear();

  circuit.render();
  for(auto& ghost : ghosts) ghost.render();
  car.render();

  // UI
  char text[10];
  snprintf(text, 10, "%.2f", lapTime.getElapsedTime().asSeconds());

  placeCamera();

  window.setView(ui);
  sf::Text lapTimeText(text, font, 30);
  lapTimeText.setPosition(10.0f, 10.0f);
  window.draw(lapTimeText);

  window.setView(camera);
  window.display();
}

void Game::placeCamera() {
  camera.setCenter(car.rigidbody.position);

  //TODO(Naum): add sigmoid
  float zoomValue = 0.002f * getMagnitude(car.rigidbody.linearVelocity) + 0.5f;
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
          car.rigidbody.applyPointAngularVelocity(1.0f);
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
  circuit.resetCheckpoints();
  float lapTimeTaken = lapTime.restart().asSeconds();

  if(lapTimeTaken <= circuit.lapTimeLimit) ghosts.back().completeLap(lapTimeTaken);
  else ghosts.back().reset();
}
