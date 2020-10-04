#pragma once

#include "rigidbody.hpp"
#include "sfml.hpp"

class Game;

struct CarState {
  Rigidbody rigidbody;
  sf::RectangleShape shape;

  float time;
};

class Ghost {
public:
  Ghost(Game&);

  CarState interpolateStates(CarState&, CarState&, f64);
  void addState();
  CarState getState(float);

  void activateRendering();
  void render();
  void reset();

  void clear();
  void completeLap(float);

  std::vector<CarState> lastStates;
  sf::Clock timeActive;

  bool isRendered = false;
  float timeToCompleteLap;

  Game &game;
};
