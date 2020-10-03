#include "rigidbody.hpp"
#include "algebra.hpp"
#include "consts.hpp"

RigidBody::RigidBody(sf::Vector2f initialPosition, sf::Vector2f initialDirection) {
  position = initialPosition;
  direction = initialDirection;
}

void RigidBody::rotate(float angle) {
  ::rotate(direction, angle);
  ::rotate(linearVelocity, angle);
}

void RigidBody::simulate(float deltaTime, float acceleration) {
  sf::Vector2f accelerationVector = acceleration * direction;

  accelerationVector -= 0.1f * linearVelocity;

  linearVelocity += accelerationVector * deltaTime;
  position += linearVelocity * deltaTime;
}