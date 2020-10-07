#pragma once

#include "types.hpp"
#include "car.hpp"
#include "particle-system.hpp"
#include "physics.hpp"
#include "circuit.hpp"
#include "ghost.hpp"
#include "ui.hpp"
#include "audio-system.hpp"

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

  void renderMainMenu();

  void setupRacing();
  void updateRacing();
  void renderRacing();

  void handleEvents();
  void handleEventRacing(const sf::Event&);
  void handleEventMainMenu(const sf::Event&);

  void setupEndGame();
  void updateEndGame();

  void completeLap();

  void placeCamera();

public:
  State state = State::MAIN_MENU;
  bool onCountdown;
  bool hasEscaped = false;

  AudioSystem audioSystem;
  sf::RenderWindow window;
  UI ui;

  sf::View camera;
  float currentZoom = 10.0f;

  sf::Clock frameClock;
  sf::Clock lapTime;
  float lastLapTime = -1.0f;
  float bestLapTime = -1.0f;

  Car car;

  Circuit* currentCircuit = nullptr;
  std::vector<Circuit> circuits;
  size_t currentCircuitIndex = 0;

  sf::RenderTexture circuitRenderTexture;
  sf::Sprite circuitSprite;

  sf::RenderTexture roadTopRenderTexture;
  sf::Sprite roadTopSprite;

  Ghost currentGhost;
  std::vector<Ghost> ghosts;
  std::vector<Ghost> newGhosts;
};
