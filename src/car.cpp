#include "car.hpp"

#include <cmath>

#include "game.hpp"
#include "particle-system.hpp"

Car::Car(sf::Vector2f position, sf::Vector2f direction, Game& _game) :
  rigidbody{ to_vector2f64(position), to_vector2f64(direction), CAR_FORWARD_DRAG, CAR_LATERAL_DRAG, CAR_ANGULAR_DRAG },
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

  shape.setPosition(to_vector2f(rigidbody.position));
  ::rotate(rigidbody.direction, -undoAngle);
  shape.setRotation(getRotation(to_vector2f(rigidbody.direction)));
}

void Car::updateParticles(float deltaTime) {
  smokeParticles.update(deltaTime);
  leftTireTracks.update(deltaTime);
  rightTireTracks.update(deltaTime);

  smokeEmission();
  tireTrackEmission();
}

void Car::render() {
  leftTireTracks.render();
  rightTireTracks.render();
  game.window.draw(shape);
  smokeParticles.render();

  // debug
  drawLine(
    { to_vector2f(rigidbody.position), to_vector2f(rigidbody.position + getUnitVector(rigidbody.direction) * 20.0) },
    sf::Color::Blue,
    game.window
  );

  drawLine(
    { to_vector2f(rigidbody.position), to_vector2f(rigidbody.position + getUnitVector(rigidbody.linearVelocity) * 20.0) },
    sf::Color::Green,
    game.window
  );
}

void Car::resolveCollision(sf::Vector2<f64> collisionVector) {
  rigidbody.resolveCollision(collisionVector);

  shape.setPosition(to_vector2f(rigidbody.position));
  shape.setRotation(getRotation(to_vector2f(rigidbody.direction)));
}

void Car::smokeEmission() {
  const auto transform = shape.getTransform();

  const sf::Vector2f bottomLeftPoint = transform.transformPoint(shape.getPoint(3));
  const sf::Vector2f bottomRightPoint = transform.transformPoint(shape.getPoint(2));
  const float PARTICLES_EMISSION_RATIO = 1 / 2000.0f;
  //const float emissionRate = PARTICLES_EMISSION_RATIO * getMagnitude(to_vector2f(rigidbody.linearVelocity));

  const float emissionRate = lerp(0.06f, 0.4f, getMagnitude(to_vector2f(rigidbody.linearVelocity) / 200.0f));

  smokeParticles.emissionFromLine(bottomLeftPoint, bottomRightPoint, -to_vector2f(rigidbody.direction), emissionRate);
}

void Car::tireTrackEmission() {
  const auto angle = to_deg64(acos(dotProduct(getUnitVector(rigidbody.direction), getUnitVector(rigidbody.linearVelocity))));

  if (!goReverse and (angle < 60.0 or getMagnitude(rigidbody.linearVelocity) < 40.0)) return;

  const auto transform = shape.getTransform();

  const sf::Vector2f bottomLeftPoint = transform.transformPoint(shape.getPoint(3));
  const sf::Vector2f bottomRightPoint = transform.transformPoint(shape.getPoint(2));

  leftTireTracks.emissionFromPoint(bottomLeftPoint, -to_vector2f(rigidbody.direction));
  rightTireTracks.emissionFromPoint(bottomRightPoint, -to_vector2f(rigidbody.direction));
}

void Car::setPosition(sf::Vector2f pos) {
  shape.setPosition(pos);
  rigidbody.position = to_vector2f64(pos);
}

void Car::move(sf::Vector2f deltaPos) {
  rigidbody.position += to_vector2f64(deltaPos);
  shape.setPosition(to_vector2f(rigidbody.position));
}
