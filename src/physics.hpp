#pragma once

#include <optional>

#include "sfml.hpp"
#include "algebra.hpp"
#include "types.hpp"

class Game;
class Car;

namespace physics {

std::optional<sf::Vector2f> getCollisionVector(
  const Car& car,
  sf::Line line
);

void resolveCollisions(Game&);

};

