#include "rigidbody.hpp"
#include "algebra.hpp"

RigidBody::RigidBody(sf::Vector2f initialPosition, sf::Vector2f initialDirection) {
  position = initialPosition;
  direction = initialDirection;
}

void RigidBody::rotate(float angle) {
  ::rotate(direction, angle);
  linearVelocity = module(linearVelocity) * direction;
}

void RigidBody::simulate(float time, int orientation) {
  float accelerationValue = 0;

  if (orientation == FORWARD) accelerationValue = acceleration;
  else if (orientation == REVERSE) accelerationValue = deceleration;

  sf::Vector2f accelerationVector = accelerationValue * direction;

  if (orientation == REVERSE and module(linearVelocity) < module(accelerationVector)) {
    linearVelocity = { 0.0f, 0.0f };
  } else linearVelocity += accelerationVector * time;

  position += linearVelocity * time;
}