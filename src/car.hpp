#pragma once

#include "sfml.hpp"
#include "rigidbody.hpp"
#include "types.hpp"
#include "consts.hpp"
#include "particle-system.hpp"

const float CAR_ENGINE_ACCELERATION = 80;
const float CAR_DRIFT_BRAKE_ACCELERATION = -120;
const float CAR_BRAKE_ACCELERATION = -150;
const float CAR_REVERSE_ACCELERATION = -60;

const float CAR_MAX_VELOCITY = 220.f;

const float MOVEMENT_TOLERANCE = 0.01f;

const float CAR_ANGULAR_VELOCITY = 2;

const f64 CAR_FORWARD_DRAG = 0.3;
const f64 CAR_LATERAL_DRAG = 5;
const f64 CAR_LATERAL_DRIFT_DRAG = 2;
const f64 CAR_ANGULAR_DRAG = 1;

const int CAR_WIDTH = 10;
const int CAR_HEIGHT = 20;

class Game;

class Car {
public:
  Car(Game&);

  void update(const float);
  void updateParticles(const float);
  void render(const sf::View&);
  void renderIcon(const sf::View&) const;

  void resolveCollision(const sf::Vector2<f64>&);

  bool isHardBraking() const;
  bool isAccelerating() const;
  bool isReversing() const;
  bool isSliding() const;

  void smokeEmission();
  void tireTrackEmission();

  void setPosition(const sf::Vector2f&);
  void move(const sf::Vector2f&);

  void applyTurnRight(const float);
  void applyTurnLeft(const float);
  void applyGoForward(const float);
  void applyGoReverse(const float);

private:
  void applySound();

public:
  Rigidbody rigidbody;
  sf::RectangleShape shape;
  sf::Texture texture;
  sf::Texture ghostTexture;
  sf::CircleShape icon;
  Game& game;

  ParticleSystem leftSmokeParticles, rightSmokeParticles, leftTireTracks, rightTireTracks;

  f64 collisionVelocity;

  float turnRightRatio = 0.f;
  float turnLeftRatio = 0.f;
  float goForwardRatio = 0.f;
  float goReverseRatio = 0.f;

  bool turnRight = false;
  bool turnLeft = false;
  bool goForward = false;
  bool goReverse = false;
  bool isHandBrakeActive = false;
  bool collided = false;
  sf::Clock driftTime;

  const float lateralDrag = CAR_LATERAL_DRAG;
  const float lateralDriftDrag = CAR_LATERAL_DRIFT_DRAG;
  const float engineAcceleration = CAR_ENGINE_ACCELERATION;
  const float brakeDriftAcceleration = CAR_DRIFT_BRAKE_ACCELERATION;
  const float brakeAcceleration = CAR_BRAKE_ACCELERATION;
  const float reverseAcceleration = CAR_REVERSE_ACCELERATION;
  const float angularVelocity = CAR_ANGULAR_VELOCITY;
};
