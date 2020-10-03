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

void RigidBody::simulate(float deltaTime, Orientation orientation) {
  float accelerationValue;

  bool isGoingForward = dotProduct(linearVelocity, direction) >= 0;

  if (orientation == FORWARD) accelerationValue = acceleration;
  else if (orientation == REVERSE) {
    if (isGoingForward) accelerationValue = brakeAcceleration;
    else accelerationValue = reverseAcceleration;
  } else if (orientation == HOLD) accelerationValue = 0.0;

  sf::Vector2f accelerationVector = accelerationValue * direction;

  accelerationVector -= 0.1f * linearVelocity;

  linearVelocity += accelerationVector * deltaTime;
  position += linearVelocity * deltaTime;
}