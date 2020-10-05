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

// This line divides the plane into two semiplanes
// The "left" plane (CCW) is the region we don't want to be
// The "right" plane (CW) is the region we want to be
// The collision vector will be pointing from bad to good planes, if it exists
static std::optional<sf::Vector2f> getCollisionVector(const Car& car, sf::Line line) {
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

static std::optional<sf::Vector2f> carIntersectsGhost(Car& car, const Ghost& ghost) {
  // Generate points since SFML doesn't know how to do it...
  const auto& shape = ghost.getCurrentState().shape;
  const auto transform = shape.getTransform();
  const sf::Vector2f points[4] = {
    transform.transformPoint(shape.getPoint(0)),
    transform.transformPoint(shape.getPoint(1)),
    transform.transformPoint(shape.getPoint(2)),
    transform.transformPoint(shape.getPoint(3))
  };

  bool collided = false;
  sf::Vector2f collisionVector { 1e8f, 1e8f };

  for (int i = 1; i <= 4; i++) {
    auto opt = getCollisionVector(car, { points[i-1], points[i%4] });
    if (opt and getMagnitude(opt.value()) < getMagnitude(collisionVector)) {
      collided = true;
      collisionVector = opt.value();
    }
  }

  if (collided) {
    car.collided = true;
    return collisionVector;
  }
  return std::nullopt;
}

namespace physics {

void resolveCollisions(Game& game) {
  for (size_t checks = 0; checks < COLLISION_CHECKS_MAX; checks++) {
    bool collided = false;
    sf::Vector2 minimumCollisionVector { 1e8f, 1e8f };

    // Walls
    if (game.currentCircuit) {
      for (auto wall : game.currentCircuit->walls) {
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
    }

    if (!collided) break;
    game.car.collided = true;
    game.car.resolveCollision(to_vector2f64(minimumCollisionVector));
  }

  // Ghosts
  size_t curSize = game.ghosts.size();
  for (int i = static_cast<int>(game.ghosts.size() - 1); i >= 0; i--) {
    auto& ghost = game.ghosts[i];
    const auto collisionVectorOption = carIntersectsGhost(game.car, ghost);
    if (collisionVectorOption) {
      const auto collisionVector = collisionVectorOption.value();

      game.car.move(collisionVector);

      const auto ghostVel = ghost.getCurrentState().rigidbody.linearVelocity;
      const auto deltaVel = ghostVel - game.car.rigidbody.linearVelocity;
      game.car.rigidbody.applyPointLinearVelocity(1.5 * deltaVel);

      game.newGhosts.push_back(std::move(ghost));

      curSize--;
      game.ghosts[i] = std::move(game.ghosts[curSize]);
    }
  }

  game.ghosts.resize(curSize);
}

}
