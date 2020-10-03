#include "car.hpp"

Car::Car(sf::Vector2f initialPosition, sf::Vector2f initialDirection) : properties{ initialPosition, initialDirection } {
  shape.setSize({ CAR_WIDTH, CAR_HEIGHT });
  shape.setOrigin({ CAR_WIDTH * 0.5, CAR_HEIGHT * 0.5 });
  shape.setPosition(initialPosition);
}

void Car::rotate(float time) {
  float angle = properties.angularVelocity * time;

  if (turnLeft) shape.rotate(to_deg(-angle)), properties.rotate(-angle);
  if (turnRight) shape.rotate(to_deg(angle)), properties.rotate(angle);
}

void Car::simulate(float time) {
  if (goReverse) properties.simulate(time, REVERSE);
  else if (goForward) properties.simulate(time, FORWARD);
  else properties.simulate(time, HOLD);

  shape.setPosition(properties.position);
}

void Car::update(float time) {
  rotate(time);
  simulate(time);
}