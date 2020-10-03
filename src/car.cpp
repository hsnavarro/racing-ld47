#include "car.hpp"

Car::Car(sf::Vector2f position, sf::Vector2f direction) :
    rigidbody{ position, direction, CAR_FORWARD_DRAG, CAR_LATERAL_DRAG, CAR_ANGULAR_DRAG } {

  shape.setSize({ CAR_WIDTH, CAR_HEIGHT });
  shape.setOrigin({ CAR_WIDTH * 0.5, CAR_HEIGHT * 0.5 });
  shape.setPosition(position);
}

void Car::update(float deltaTime) {
  float accelerationValue = 0.0;

  bool isGoingForward = rigidbody.isGoingForward();

  if (goReverse) {
    if (isGoingForward) accelerationValue = brakeAcceleration;
    else accelerationValue = reverseAcceleration;
  } else if (goForward) accelerationValue = engineAcceleration;

  float deltaAngularVelocity = 0.0f;
  if (turnLeft)  deltaAngularVelocity = -angularVelocity;
  if (turnRight) deltaAngularVelocity = angularVelocity;

  rigidbody.update(deltaTime, accelerationValue, deltaAngularVelocity);

  shape.setPosition(rigidbody.position);
  shape.setRotation(rigidbody.getRotation());
}

void Car::resolveCollision(sf::Vector2f collisionVector) {
  rigidbody.resolveCollision(collisionVector);

  shape.setPosition(rigidbody.position);
  shape.setRotation(rigidbody.getRotation());
}
