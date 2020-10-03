#include "rigidbody.hpp"
#include "algebra.hpp"
#include "consts.hpp"

RigidBody::RigidBody(sf::Vector2f initialPosition, sf::Vector2f initialDirection) {
  position = initialPosition;
  direction = initialDirection;
}

void RigidBody::rotate(float angle) {
  ::rotate(direction, angle);
  linearVelocity = module(linearVelocity) * direction;
}

void RigidBody::simulate(float deltaTime, Orientation orientation) {
  float accelerationValue;

  if (orientation == FORWARD) accelerationValue = acceleration;
  else if (orientation == REVERSE) accelerationValue = deceleration;
  else if (orientation == HOLD) accelerationValue = 0.0;

  accelerationValue -= 0.1 * module(linearVelocity);

  sf::Vector2f accelerationVector = accelerationValue * direction;

  linearVelocity += accelerationVector * deltaTime;

  if(dotProduct(linearVelocity, direction) < 0) linearVelocity = { 0.0f, 0.0f };

  position += linearVelocity * deltaTime;
}