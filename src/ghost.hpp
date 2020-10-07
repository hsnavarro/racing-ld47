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

  CarState interpolateStates(const CarState&, const CarState&, const f64) const;
  void addState(const Game&);
  CarState getState(const float) const;
  CarState getCurrentState() const;

  void activateRendering();
  void render(sf::RenderWindow&) const;
  void reset();

  void clear();
  void completeLap(const float);

public:
  std::vector<CarState> lastStates;
  sf::Clock timeActive;

  bool isRendered = false;
  float timeToCompleteLap;
};
