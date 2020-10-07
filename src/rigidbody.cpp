#include "rigidbody.hpp"

#include <cmath>
#include "algebra.hpp"
#include "consts.hpp"

#include <cstdio>

Rigidbody::Rigidbody(const f64 forwardDrag,
  const f64 lateralDrag,
  const f64 angularDrag) :
  forwardDrag{ forwardDrag },
  lateralDrag{ lateralDrag },
  kAngularDrag{ angularDrag }
{}

void Rigidbody::update(const f64 deltaTime,
  const f64 linearAcceleration,
  const f64 deltaAngularVelocity) {


  // rotation
  angularVelocity -= kAngularDrag * angularVelocity * deltaTime;
  if (std::abs(angularVelocity) < std::abs(deltaAngularVelocity)) angularVelocity = 0.0;

  f64 angle = (angularVelocity + deltaAngularVelocity) * deltaTime;

  ::rotate(direction, angle);

  // translation
  //sf::Vector2<f64> accelerationVector = linearAcceleration * direction;

  //const auto forwardUnit = getUnitVector(direction);
  const auto lateralUnit = getUnitVector(getPerpendicular(direction));

  const f64 forwardSpeed = dotProduct(linearVelocity, getUnitVector(direction));
  const f64 lateralSpeed = crossProduct(linearVelocity, getUnitVector(direction));

  const f64 forwardAccel = linearAcceleration - forwardDrag * forwardSpeed;
  const f64 lateralAccel = -lateralDrag * lateralSpeed;

  linearVelocity =
    (forwardSpeed + forwardAccel * deltaTime) * direction +
    (lateralSpeed + lateralAccel * deltaTime) * lateralUnit;

  position += linearVelocity * deltaTime;
}

void Rigidbody::applyPointAngularVelocity(const f64 deltaAngularVelocity) {
  angularVelocity += deltaAngularVelocity;
}

void Rigidbody::applyPointLinearVelocity(const sf::Vector2<f64>& deltaLinearVelocity) {
  linearVelocity += deltaLinearVelocity;
}

void Rigidbody::resolveCollision(const sf::Vector2<f64>& collisionVector) {
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

void Rigidbody::reset() {
  position = { 0.0, 0.0 };
  direction = { 0.0, -1.0 };
  linearVelocity = { 0.0, 0.0 };
  angularVelocity = 0.0;
}

void Rigidbody::setForwardDrag(const f64 drag) {
  forwardDrag = drag;
}
