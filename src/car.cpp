#include "car.hpp"

Car::Car(sf::Vector2f initialPosition, sf::Vector2f initialDirection) : properties{ initialPosition, initialDirection } {
  shape.setSize({ CAR_WIDTH, CAR_HEIGHT });
  shape.setOrigin({ CAR_WIDTH * 0.5, CAR_HEIGHT * 0.5 });
  shape.setPosition(initialPosition);
}

void Car::rotate(float deltaTime) {
  float angle = angularVelocity * deltaTime;

  if (turnLeft) shape.rotate(to_deg(-angle)), properties.rotate(-angle);
  if (turnRight) shape.rotate(to_deg(angle)), properties.rotate(angle);
}

void Car::simulate(float deltaTime) {
  float accelerationValue = 0.0;

  bool isGoingForward = dotProduct(properties.linearVelocity, properties.direction) >= 0;

  if (goReverse) {
    if (isGoingForward) accelerationValue = brakeAcceleration;
    else accelerationValue = reverseAcceleration;
  } else if (goForward) accelerationValue = engineAcceleration;

  properties.simulate(deltaTime, accelerationValue);

  shape.setPosition(properties.position);
}

void Car::update(float deltaTime) {
  rotate(deltaTime);
  simulate(deltaTime);
}