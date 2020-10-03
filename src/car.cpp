#include "car.hpp"

Car::Car(sf::Vector2f initialPosition, sf::Vector2f initialDirection) : properties{ initialPosition, initialDirection } {
  shape.setSize({ CAR_WIDTH, CAR_HEIGHT });
  shape.setOrigin({ CAR_WIDTH * 0.5, CAR_HEIGHT * 0.5 });
  shape.setPosition(initialPosition);
}

void Car::rotate(float deltaTime) {
  float angle = properties.angularVelocity * deltaTime;

  if (turnLeft) shape.rotate(to_deg(-angle)), properties.rotate(-angle);
  if (turnRight) shape.rotate(to_deg(angle)), properties.rotate(angle);
}

void Car::simulate(float deltaTime) {
  if (goReverse) properties.simulate(deltaTime, REVERSE);
  else if (goForward) properties.simulate(deltaTime, FORWARD);
  else properties.simulate(deltaTime, HOLD);

  shape.setPosition(properties.position);
}

void Car::update(float deltaTime) {
  rotate(deltaTime);
  simulate(deltaTime);
}