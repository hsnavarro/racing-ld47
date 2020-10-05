#include "car.hpp"

#include <cstdio>
#include <cmath>
#include <random>

#include "game.hpp"
#include "particle-system.hpp"

Car::Car(Game& game_) :
  rigidbody{ CAR_FORWARD_DRAG, CAR_LATERAL_DRAG, CAR_ANGULAR_DRAG },
  game{ game_ },
  smokeParticles{ ParticleType::SMOKE, game_ },
  leftTireTracks{ ParticleType::TIRE_TRACK , game_ },
  rightTireTracks{ ParticleType::TIRE_TRACK, game_ }
{

  texture.loadFromFile("assets/gfx/car.png");
  texture.setSmooth(true);

  ghostTexture.loadFromFile("assets/gfx/ghost.png");
  ghostTexture.setSmooth(true);

  shape.setTexture(&texture);
  shape.setSize({ CAR_WIDTH, CAR_HEIGHT });
  shape.setOrigin({ CAR_WIDTH * 0.5, CAR_HEIGHT * 0.5 });

  icon.setRadius(ICON_RADIUS);
  icon.setFillColor(ICON_COLOUR);
}

bool Car::isAccelerating() {
  if (goReverse) return false;
  return goForward;
}

bool Car::isReversing() {
  return goReverse;
}

void static applySound(Game& game) {
  auto& car = game.car;

  if (!car.isHardBraking()) {
    const float slideVolume = lerp(0.f, 30.f, getMagnitude(to_vector2f(car.rigidbody.linearVelocity) / CAR_MAX_VELOCITY));
    game.audioSystem.slideFX.setVolume(slideVolume);
    game.audioSystem.slideFX.stop();
  } else {
    game.audioSystem.slideFX.play();
  }

  if (car.collided) {
      
    const float volumeRatio = std::min(static_cast<float>(car.collisionVelocity) / (CAR_MAX_VELOCITY * 0.01f), 1.0f);
    const float collisionVolume = lerp(0.f, 30.f, volumeRatio);

    game.audioSystem.collisionFX.setVolume(collisionVolume);
    game.audioSystem.collisionFX.play();

    car.collided = false;
  }

  const float engineVolume = lerp(30.f, 80.f, getMagnitude(to_vector2f(car.rigidbody.linearVelocity) / CAR_MAX_VELOCITY));
  game.audioSystem.engineFX.setVolume(engineVolume);

  game.audioSystem.engineStartFX.stop();
  game.audioSystem.engineFX.play();

  const float acceleratorVolume = lerp(0.f, 100.f, getMagnitude(to_vector2f(car.rigidbody.linearVelocity) / CAR_MAX_VELOCITY));
  game.audioSystem.acceleratorFX.setVolume(acceleratorVolume);
  const float acceleratorPitchLevel = std::min(100.f, acceleratorVolume) / 50.f;
  game.audioSystem.acceleratorFX.setPitch(acceleratorPitchLevel);
  game.audioSystem.acceleratorFX.play();
}

void Car::update(float deltaTime) {
  float accelerationValue = 0.0;

  bool isGoingForward = rigidbody.isGoingForward();

  bool isMoving = getMagnitude(rigidbody.linearVelocity) > MOVEMENT_TOLERANCE;

  if (isReversing()) {
    if (isGoingForward) accelerationValue = brakeAcceleration;
    else accelerationValue = reverseAcceleration;
  }

  if (isAccelerating()) accelerationValue = engineAcceleration;

  if (isHandBrakeActive and isMoving) {
    if (isGoingForward) accelerationValue += brakeDriftAcceleration;
    else accelerationValue -= brakeDriftAcceleration;
  }

  float undoAngle = 0.0f;

  float deltaAngularVelocity = 0.0f;

  // create timer
  if (isHandBrakeActive) rigidbody.kLateralDrag = lateralDriftDrag;
  else rigidbody.kLateralDrag = lateralDrag;

  if (isMoving) {
    if (turnLeft) deltaAngularVelocity = -angularVelocity;
    if (turnRight) deltaAngularVelocity = angularVelocity;
  }

  if (!isGoingForward)
    deltaAngularVelocity *= -1;

  bool wasSliding = isSliding();
  updateDriftingStatus();
  if constexpr (IS_DRIFTING_BURST_ENABLED) {
    if (!isSliding() and wasSliding and driftTime.getElapsedTime().asSeconds() > DRIFT_BURST_TIME_THRESHOLD) {
      auto const unit = getUnitVector(rigidbody.direction);
      rigidbody.applyPointLinearVelocity(BURST_FORCE * unit);
    }
  }

  rigidbody.update(deltaTime, accelerationValue, deltaAngularVelocity);

  shape.setPosition(to_vector2f(rigidbody.position));
  icon.setPosition(to_vector2f(rigidbody.position));
  ::rotate(rigidbody.direction, -undoAngle);
  shape.setRotation(getRotation(to_vector2f(rigidbody.direction)));

  applySound(game);
}

void Car::updateParticles(float deltaTime) {
  smokeParticles.update(deltaTime);

  smokeEmission();
  tireTrackEmission();
}

void Car::render(const sf::View& view) {
  game.window.setView(view);
  game.window.draw(shape);
  smokeParticles.render();

  // debug
  /*
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
  */
}

void Car::renderIcon(const sf::View& view) {
  game.window.setView(view);
  game.window.draw(icon);
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

  //const float PARTICLES_EMISSION_RATIO = 1 / 2000.0f;
  //const float emissionRate = PARTICLES_EMISSION_RATIO * getMagnitude(to_vector2f(rigidbody.linearVelocity));

  const float emissionRate = lerp(0.06f, 0.4f, getMagnitude(to_vector2f(rigidbody.linearVelocity) / CAR_MAX_VELOCITY));

  smokeParticles.emissionFromLine(bottomLeftPoint, bottomRightPoint, -to_vector2f(rigidbody.direction), emissionRate);
}

void Car::updateDriftingStatus() {
  const bool wasSliding = isSliding();
  const auto angle = to_deg64(acos(dotProduct(getUnitVector(rigidbody.direction), getUnitVector(rigidbody.linearVelocity))));
  const bool isSliding = angle > LIM_TIRE_TRACK_ANGLE;

  if (isSliding and !wasSliding) driftTime.restart();
}

bool Car::isHardBraking() {
  if (getMagnitude(rigidbody.linearVelocity) < TIRE_TRACK_MIN_VELOCITY) return false;

  bool isCarGoingForward = rigidbody.isGoingForward();

  bool isHardBraking = false;

  if (isHandBrakeActive) isHardBraking = true;

  if (isCarGoingForward) {
    if (isReversing()) isHardBraking = true;
    if (isSliding()) isHardBraking = true;
  }

  if (!isCarGoingForward and isAccelerating()) isHardBraking = true;

  return isHardBraking;
}

void Car::tireTrackEmission() {
  if (!isHardBraking()) return;

  game.audioSystem.slideFX.play();

  const auto transform = shape.getTransform();

  const sf::Vector2f bottomLeftPoint = transform.transformPoint(shape.getPoint(3));
  const sf::Vector2f bottomRightPoint = transform.transformPoint(shape.getPoint(2));

  leftTireTracks.emitToTexture(bottomLeftPoint, game.roadTopRenderTexture);
  rightTireTracks.emitToTexture(bottomRightPoint, game.roadTopRenderTexture);
}

void Car::setPosition(sf::Vector2f pos) {
  rigidbody.reset();

  shape.setPosition(pos);
  shape.setRotation(getRotation(to_vector2f(rigidbody.direction)));
  rigidbody.position = to_vector2f64(pos);
}

void Car::move(sf::Vector2f deltaPos) {
  rigidbody.position += to_vector2f64(deltaPos);
  shape.setPosition(to_vector2f(rigidbody.position));
}

bool Car::isSliding() {
  const auto angle = to_deg64(acos(dotProduct(getUnitVector(rigidbody.direction), getUnitVector(rigidbody.linearVelocity))));
  return angle > LIM_TIRE_TRACK_ANGLE;
}
