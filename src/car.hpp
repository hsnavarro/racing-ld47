#pragma once

#include "sfml.hpp"
#include "rigidbody.hpp"
#include "types.hpp"
#include "consts.hpp"
#include "particle-system.hpp"

const float CAR_ENGINE_ACCELERATION  = 80;
const float CAR_ENGINE_DRIFT_ACCELERATION = 50;
const float CAR_BRAKE_ACCELERATION   = -200;
const float CAR_REVERSE_ACCELERATION = -20;

const float CAR_ANGULAR_VELOCITY = 2;
const float CAR_DRIFT_ANGULAR_VELOCITY = 3;

const float CAR_FORWARD_DRAG = 0.4f;
//const float CAR_LATERAL_DRAG = 0.9999f;
const float CAR_ANGULAR_DRAG = 0.99f;

const int CAR_WIDTH  = 10;
const int CAR_HEIGHT = 20;

class Game;

class Car {
public:
  Car(sf::Vector2f, sf::Vector2f, Game&);

  void update(float);
  void updateParticles(float);
  void render();

  void resolveCollision(sf::Vector2f collisionVector);

  void smokeEmission();
  void tireTrackEmission();

  void setPosition(sf::Vector2f);

public:
  Rigidbody rigidbody;
  sf::RectangleShape shape;
  Game& game;

  ParticleSystem smokeParticles, leftTireTracks, rightTireTracks;

  bool turnRight = false;
  bool turnLeft = false;
  bool goForward = false;
  bool goReverse = false;
  bool isDriftActive = false;

  // These are constants!
  float engineAcceleration = CAR_ENGINE_ACCELERATION;
  float engineDriftAcceleration = CAR_ENGINE_DRIFT_ACCELERATION;
  float brakeAcceleration = CAR_BRAKE_ACCELERATION;
  float reverseAcceleration = CAR_REVERSE_ACCELERATION;
  float angularVelocity = CAR_ANGULAR_VELOCITY;
  float angularDriftVelocity = CAR_DRIFT_ANGULAR_VELOCITY;
};
