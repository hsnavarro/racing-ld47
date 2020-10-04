#include "rigidbody.hpp"

#include <cmath>
#include "algebra.hpp"
#include "consts.hpp"

#include <cstdio>

Rigidbody::Rigidbody() {}

Rigidbody::Rigidbody(sf::Vector2<f64> _position,
                     sf::Vector2<f64> _direction,
                     f64 forwardDrag,
                     f64 lateralDrag,
                     f64 angularDrag) :
    position { _position },
    direction { _direction },
    kForwardDrag { forwardDrag },
    kLateralDrag { lateralDrag },
    kAngularDrag { angularDrag }
{}

void Rigidbody::update(f64 deltaTime,
                       f64 linearAcceleration,
                       f64 deltaAngularVelocity) {
  // rotation
  angularVelocity -= kAngularDrag * angularVelocity * deltaTime;
  if (std::abs(angularVelocity) < std::abs(deltaAngularVelocity)) angularVelocity = 0.0;

  f64 angle = (angularVelocity + deltaAngularVelocity) * deltaTime;

  ::rotate(direction, angle);

  // translation
  sf::Vector2<f64> accelerationVector = linearAcceleration * direction;

  const auto forwardUnit = getUnitVector(direction);
  const auto lateralUnit = getUnitVector(getPerpendicular(direction));

  const f64 forwardSpeed = dotProduct(linearVelocity, getUnitVector(direction));
  const f64 lateralSpeed = crossProduct(linearVelocity, getUnitVector(direction));

  const f64 forwardAccel = linearAcceleration - kForwardDrag * forwardSpeed;
  const f64 lateralAccel = - kLateralDrag * lateralSpeed;

  linearVelocity =
    (forwardSpeed + forwardAccel * deltaTime) * direction +
    (lateralSpeed + lateralAccel * deltaTime) * lateralUnit;

  position += linearVelocity * deltaTime;
}

void Rigidbody::applyPointAngularVelocity(f64 deltaAngularVelocity) {
  angularVelocity += deltaAngularVelocity;
}

void Rigidbody::applyPointLinearVelocity(sf::Vector2<f64> deltaLinearVelocity) {
  linearVelocity += deltaLinearVelocity;
}

void Rigidbody::resolveCollision(sf::Vector2<f64> collisionVector) {
  position += collisionVector;

  const auto alignedCollisionAmount =
    dotProduct(linearVelocity, getUnitVector(collisionVector));

  /*
  const auto perpendicularCollisionAmount =
    crossProduct(linearVelocity, getUnitVector(collisionVector));
  */

  f64 deltaAngularVelocity =
    -0.01 *
    alignedCollisionAmount *
    crossProduct(getUnitVector(linearVelocity), getUnitVector(collisionVector));

  applyPointAngularVelocity(deltaAngularVelocity);

  applyPointLinearVelocity(
    -1.4 *
    getUnitVector(collisionVector) *
    dotProduct(linearVelocity, getUnitVector(collisionVector))
  );
}

bool Rigidbody::isGoingForward() const {
  return dotProduct(linearVelocity, direction) >= 0;
}
