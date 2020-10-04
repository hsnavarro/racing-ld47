#include "rigidbody.hpp"

#include <cmath>
#include "algebra.hpp"
#include "consts.hpp"

#include <cstdio>

Rigidbody::Rigidbody() {}

Rigidbody::Rigidbody(sf::Vector2f _position,
                     sf::Vector2f _direction,
                     float forwardDrag,
                     float angularDrag) :
    position { _position },
    direction { _direction },
    kForwardDrag { forwardDrag },
    kAngularDrag { angularDrag }
{}

void Rigidbody::update(float deltaTime,
                       float linearAcceleration,
                       float deltaAngularVelocity) {
  // rotation
  angularVelocity -= kAngularDrag * angularVelocity * deltaTime;
  if (std::abs(angularVelocity) < std::abs(deltaAngularVelocity)) angularVelocity = 0.0f;

  float angle = (angularVelocity + deltaAngularVelocity) * deltaTime;

  ::rotate(direction, angle);

  // translation
  sf::Vector2f accelerationVector = linearAcceleration * direction;
  linearVelocity += accelerationVector * deltaTime;

  const float forwardSpeed =
    dotProduct(linearVelocity, getUnitVector(direction)) *
    (1.0f - kForwardDrag * deltaTime);

  /*
  const float lateralSpeed =
    crossProduct(linearVelocity, getUnitVector(direction)) *
    (1.0f - kLateralDrag * deltaTime);
    */

  linearVelocity =
    forwardSpeed * direction;
    //lateralSpeed * getPerpendicular(direction);

  position += linearVelocity * deltaTime;
}

void Rigidbody::applyPointAngularVelocity(float deltaAngularVelocity) {
  angularVelocity += deltaAngularVelocity;
}

void Rigidbody::applyPointLinearVelocity(sf::Vector2f deltaLinearVelocity) {
  linearVelocity += deltaLinearVelocity;
}

void Rigidbody::resolveCollision(sf::Vector2f collisionVector) {
  position += collisionVector;

  const auto alignedCollisionAmount =
    dotProduct(linearVelocity, getUnitVector(collisionVector));

  /*
  const auto perpendicularCollisionAmount =
    crossProduct(linearVelocity, getUnitVector(collisionVector));
  */

  float deltaAngularVelocity =
    -0.01f *
    alignedCollisionAmount *
    crossProduct(getUnitVector(linearVelocity), getUnitVector(collisionVector));

  applyPointAngularVelocity(deltaAngularVelocity);

  applyPointLinearVelocity(
    -1.4f *
    getUnitVector(collisionVector) *
    dotProduct(linearVelocity, getUnitVector(collisionVector))
  );
}

bool Rigidbody::isGoingForward() const {
  return dotProduct(linearVelocity, direction) >= 0;
}