#pragma once

#include <optional>

#include "sfml.hpp"
#include "algebra.hpp"
#include "types.hpp"

const size_t COLLISION_CHECKS_MAX = 10;

class Game;
class Car;

bool carIntersectsLine(const Car&, const sf::Line&);

namespace physics {

  void resolveCollisions(Game&);

};
