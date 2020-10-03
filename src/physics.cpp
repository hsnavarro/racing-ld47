#include "physics.hpp"

#include <cmath>
#include <array>
#include <iostream>

#include "car.hpp"
#include "game.hpp"
#include "consts.hpp"
#include "algebra.hpp"

bool carIntersectsLine(const Car& car, sf::Line line) {
  // Generate points since SFML doesn't know how to do it...
  const auto transform = car.shape.getTransform();
  const sf::Vector2f points[4] = {
    transform.transformPoint(car.shape.getPoint(0)),
    transform.transformPoint(car.shape.getPoint(1)),
    transform.transformPoint(car.shape.getPoint(2)),
    transform.transformPoint(car.shape.getPoint(3))
  };

  for (int i = 1; i <= 4; i++)
    if (linesIntersect({ points[i-1], points[i%4] }, { line[0], line[1] }))
      return true;

  return false;
}

namespace physics {

// This line divides the plane into two semiplanes
// The "left" plane (CCW) is the region we don't want to be
// The "right" plane (CW) is the region we want to be
// The collision vector will be pointing from bad to good planes, if it exists
std::optional<sf::Vector2f> getCollisionVector(const Car& car, sf::Line line) {
  bool collided = false;

  sf::Vector2f collisionVector;

  // Generate points since SFML doesn't know how to do it...
  const auto transform = car.shape.getTransform();
  const sf::Vector2f points[4] = {
    transform.transformPoint(car.shape.getPoint(0)),
    transform.transformPoint(car.shape.getPoint(1)),
    transform.transformPoint(car.shape.getPoint(2)),
    transform.transformPoint(car.shape.getPoint(3))
  };

  // Get the collision vector, but they not necessarily collide
  const auto lineDir = getUnitVector(line[1] - line[0]);
  const auto insideUnitDir = getUnitVector(sf::Vector2f { lineDir.y, -lineDir.x });

  for (int i = 0; i < 4; i++) {
    const auto pointDir = points[i] - line[0];
    const auto cross = crossProduct(lineDir, getUnitVector(pointDir));
    if (cross > EPS and getMagnitude(pointDir) * cross > getMagnitude(collisionVector)) {
      collisionVector = getMagnitude(pointDir) * cross * insideUnitDir;
    }
  }

  for (int i = 1; i <= 4; i++) {
    if (linesIntersect({ points[i-1], points[i%4] }, { line[0], line[1] }))
      collided = true;
  }

  if (collided)
    return collisionVector;
  return std::nullopt;
}

void resolveCollisions(Game& game) {
  for (size_t checks = 0; checks < COLLISION_CHECKS_MAX; checks++) {
    bool collided = false;
    sf::Vector2 minimumCollisionVector { 1e8f, 1e8f };

    for (auto wall : game.circuit.walls) {
      const auto collisionVectorOption = getCollisionVector(game.car, wall);
      if (collisionVectorOption) {
        collided = true;

        const auto collisionVector = collisionVectorOption.value();

        if (getMagnitude(minimumCollisionVector) >
            getMagnitude(collisionVector)) {

          minimumCollisionVector = collisionVector;
        }

      }
    }

    if (!collided) break;
    game.car.resolveCollision(minimumCollisionVector);
  }

  /*
  // Todo(naum): update when track is done
  const auto collOpt = getCollisionVector(game.car, game.line);
  if (collOpt) {
    const auto collVec = collOpt.value();
    game.car.resolveCollision(collVec);
  }
  */
}

}
