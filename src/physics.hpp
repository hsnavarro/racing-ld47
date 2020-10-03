#pragma once

#include <optional>

#include "sfml.hpp"
#include "algebra.hpp"
#include "types.hpp"

const size_t COLLISION_CHECKS_MAX = 10;

class Game;
class Car;

bool carIntersectsLine(const Car&, sf::Line);

namespace physics {

std::optional<sf::Vector2f> getCollisionVector(
  const Car& car,
  sf::Line line
);

void resolveCollisions(Game&);

};

