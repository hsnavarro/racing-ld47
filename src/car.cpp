#include "car.hpp"
#include "game.hpp"
#include "particle-system.hpp"

Car::Car(sf::Vector2f position, sf::Vector2f direction, Game& _game) :
  rigidbody{ position, direction, CAR_FORWARD_DRAG, CAR_ANGULAR_DRAG },
  game{ _game },
  smokeParticles{ ParticleType::SMOKE, _game },
  leftTireTracks{ ParticleType::TIRE_TRACK , _game },
  rightTireTracks{ ParticleType::TIRE_TRACK, _game } {
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
  } else if (goForward) {
    if (isDriftActive) accelerationValue = engineDriftAcceleration;
    else accelerationValue = engineAcceleration;
  }

  float undoAngle = 0.0f;

  float deltaAngularVelocity = 0.0f;
  if (turnLeft) {
    if (isDriftActive) {
      deltaAngularVelocity = -angularDriftVelocity;
      undoAngle = PI32 / 6.0f;
      ::rotate(rigidbody.direction, undoAngle);
    } else deltaAngularVelocity = -angularVelocity;
  }
  if (turnRight) {
    if (isDriftActive) {
      deltaAngularVelocity = angularDriftVelocity;
      undoAngle = -PI32 / 6.0f;
      ::rotate(rigidbody.direction, undoAngle);
    } else deltaAngularVelocity = angularVelocity;
  }

  rigidbody.update(deltaTime, accelerationValue, deltaAngularVelocity);

  shape.setPosition(rigidbody.position);
  ::rotate(rigidbody.direction, -undoAngle);
  shape.setRotation(getRotation(rigidbody.direction));
}

void Car::updateParticles(float deltaTime) {
  smokeParticles.update(deltaTime);
  leftTireTracks.update(deltaTime);
  rightTireTracks.update(deltaTime);

  smokeEmission();
  tireTrackEmission();
}

void Car::render() {
  game.window.draw(shape);
  smokeParticles.render();
  leftTireTracks.render();
  rightTireTracks.render();
}

void Car::resolveCollision(sf::Vector2f collisionVector) {
  rigidbody.resolveCollision(collisionVector);

  shape.setPosition(rigidbody.position);
  shape.setRotation(getRotation(rigidbody.direction));
}

void Car::smokeEmission() {
  const auto transform = shape.getTransform();

  const sf::Vector2f bottomLeftPoint = transform.transformPoint(shape.getPoint(3));
  const sf::Vector2f bottomRightPoint = transform.transformPoint(shape.getPoint(2));
  const float PARTICLES_EMISSION_RATIO = 1 / 2000.0f;
  const float emissionRate = PARTICLES_EMISSION_RATIO * getMagnitude(rigidbody.linearVelocity);

  smokeParticles.emissionFromLine(bottomLeftPoint, bottomRightPoint, -rigidbody.direction, emissionRate);
}

void Car::tireTrackEmission() {
  if (!goReverse) return;

  const auto transform = shape.getTransform();

  const sf::Vector2f bottomLeftPoint = transform.transformPoint(shape.getPoint(3));
  const sf::Vector2f bottomRightPoint = transform.transformPoint(shape.getPoint(2));

  leftTireTracks.emissionFromPoint(bottomLeftPoint, -rigidbody.direction);
  rightTireTracks.emissionFromPoint(bottomRightPoint, -rigidbody.direction);
}

void Car::setPosition(sf::Vector2f pos) {
  shape.setPosition(pos);
  rigidbody.position = pos;
}
