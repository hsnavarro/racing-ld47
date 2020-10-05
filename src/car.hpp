#pragma once

#include "sfml.hpp"
#include "rigidbody.hpp"
#include "types.hpp"
#include "consts.hpp"
#include "particle-system.hpp"

const float CAR_ENGINE_ACCELERATION  = 80;
const float CAR_DRIFT_BRAKE_ACCELERATION = -120;
const float CAR_BRAKE_ACCELERATION   = -150;
const float CAR_REVERSE_ACCELERATION = -60;

const float CAR_MAX_VELOCITY = 220.f;

const float MOVEMENT_TOLERANCE = 0.01f;

const float CAR_ANGULAR_VELOCITY = 2;

const f64 CAR_FORWARD_DRAG = 0.3;
const f64 CAR_LATERAL_DRAG = 5;
const f64 CAR_LATERAL_DRIFT_DRAG = 2;
const f64 CAR_ANGULAR_DRAG = 1;

const int CAR_WIDTH  = 10;
const int CAR_HEIGHT = 20;

class Game;

class Car {
public:
  Car(Game&);

  void update(float);
  void updateParticles(float);
  void updateDriftingStatus();
  void render(const sf::View& view);
  void renderIcon(const sf::View& view);

  void resolveCollision(sf::Vector2<f64> collisionVector);

  bool isHardBraking();
  bool isAccelerating();
  bool isReversing();

  void smokeEmission();
  void tireTrackEmission();

  void setPosition(sf::Vector2f);
  void move(sf::Vector2f);

  bool isSliding();

public:
  Rigidbody rigidbody;
  sf::RectangleShape shape;
  sf::Texture texture;
  sf::Texture ghostTexture;
  sf::CircleShape icon;
  Game& game;

  ParticleSystem smokeParticles, leftTireTracks, rightTireTracks;

  f64 collisionVelocity;

  bool turnRight = false;
  bool turnLeft = false;
  bool goForward = false;
  bool goReverse = false;
  bool isHandBrakeActive = false;
  bool collided = false;
  sf::Clock driftTime;

  // These are constants!
  float lateralDrag = CAR_LATERAL_DRAG;
  float lateralDriftDrag = CAR_LATERAL_DRIFT_DRAG;
  float engineAcceleration = CAR_ENGINE_ACCELERATION;
  float brakeDriftAcceleration = CAR_DRIFT_BRAKE_ACCELERATION;
  float brakeAcceleration = CAR_BRAKE_ACCELERATION;
  float reverseAcceleration = CAR_REVERSE_ACCELERATION;
  float angularVelocity = CAR_ANGULAR_VELOCITY;
};
