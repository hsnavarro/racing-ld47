#pragma once

#include "types.hpp"
#include "car.hpp"
#include "particle-system.hpp"
#include "physics.hpp"
#include "circuit.hpp"
#include "ghost.hpp"
#include "ui.hpp"
#include "audio.hpp"

class Game {
public:
  enum class State {
    MAIN_MENU,
    RACING,
    END_GAME
  };

public:
  Game();

  void run();
  void setup();

  void setupRacing();
  void updateRacing();
  void renderRacing();

  void handleEvents();
  void handleEventRacing(sf::Event&);

  void completeLap();

  void placeCamera();

public:
  State state = State::MAIN_MENU;
  bool onCountdown;

  sf::RenderWindow window;
  UI ui;

  Audio backgroundMusic;

  sf::View camera;
  float currentZoom = 10.0f;

  sf::Clock frameClock;
  sf::Clock lapTime;
  float lastLapTime = -1.0f;

  Car car;

  Circuit* currentCircuit = nullptr;
  std::vector<Circuit> circuits;
  size_t currentCircuitIndex = 0;

  Ghost currentGhost;
  std::vector<Ghost> ghosts;
  std::vector<Ghost> newGhosts;
};
