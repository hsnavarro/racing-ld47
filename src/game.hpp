#pragma once

#include "types.hpp"
#include "car.hpp"
#include "particle-system.hpp"
#include "physics.hpp"
#include "circuit.hpp"
#include "ghost.hpp"

class Game {
public:
  Game();

  void update();
  void render();
  void handleEvents();
  void placeCamera();

  f32  getTime() const;
  void completeLap();

public:
  sf::RenderWindow window;
  sf::Font font;
  sf::View camera;
  sf::View ui;
  float currentZoom = 10.0f;
  sf::View minimap;

  sf::Clock totalTime;
  sf::Clock clock;
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
