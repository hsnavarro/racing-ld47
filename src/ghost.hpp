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
  Ghost();

  CarState interpolateStates(CarState&, CarState&, f64) const;
  void addState(const Game&);
  CarState getState(float) const;
  CarState getCurrentState() const;

  void activateRendering();
  void render(sf::RenderWindow&);
  void reset();

  void clear();
  void completeLap(float);

public:
  std::vector<CarState> lastStates;
  sf::Clock timeActive;

  bool isRendered = false;
  float timeToCompleteLap;
};
