#include "ghost.hpp"
#include "game.hpp"
#include "car.hpp"

#include <iostream>

Ghost::Ghost(Game& _game) : game{ _game } {
  timeActive.restart();
};

void Ghost::addState() {
  Car& car = game.car;
  float currentLapTime = game.lapTime.getElapsedTime().asSeconds();

  lastStates.push_back({ car.rigidbody, car.shape, currentLapTime });
}

sf::Vector2f interpolateVectors(sf::Vector2f currentFrameVector, sf::Vector2f nextFrameVector, float interpolationRatio) {
  return interpolationRatio * nextFrameVector + (1 - interpolationRatio) * currentFrameVector;
}

void applyTransformation(sf::RectangleShape& shape, sf::Vector2f& position, sf::Vector2f& direction) {
  shape.setPosition(position);
  shape.setRotation(getRotation(direction));
}

CarState Ghost::interpolateStates(CarState& currentFrame, CarState& nextFrame, float interpolationTime) {

  float interpolationRatio = (interpolationTime - currentFrame.time) / (nextFrame.time - currentFrame.time);

  sf::Vector2f interPosition = interpolateVectors(currentFrame.shape.getPosition(), nextFrame.shape.getPosition(), interpolationRatio);
  sf::Vector2f interDirection = interpolateVectors(currentFrame.rigidbody.direction, nextFrame.rigidbody.direction, interpolationRatio);
  sf::Vector2f interLinearVelocity = interpolateVectors(currentFrame.rigidbody.linearVelocity, nextFrame.rigidbody.linearVelocity, interpolationRatio);

  interDirection = getUnitVector(interDirection);

  float interTime = interpolationTime;

  Rigidbody interRigidbody;
  interRigidbody.position = interPosition;
  interRigidbody.direction = interDirection;
  interRigidbody.linearVelocity = interLinearVelocity;

  sf::RectangleShape interShape = currentFrame.shape;
  applyTransformation(interShape, interPosition, interDirection);

  return { interRigidbody, interShape, interTime };
}

enum Direction { FUTURE, PAST };

CarState predictState(CarState& currentState, float deltaTime, Direction direction) {

  float multiplier = direction == FUTURE ? 1.0f : -1.0f;

  sf::Vector2f& currentStateDirection = currentState.rigidbody.direction;
  sf::Vector2f& currentStatePosition = currentState.rigidbody.position;
  sf::Vector2f& currentStateLinearVelocity = currentState.rigidbody.direction;

  CarState statePredicted;

  statePredicted.rigidbody.linearVelocity = currentStateLinearVelocity + multiplier * deltaTime * CAR_ENGINE_ACCELERATION * currentStateDirection;
  statePredicted.rigidbody.direction = currentStateDirection;
  statePredicted.rigidbody.position = currentStatePosition + multiplier * deltaTime * currentStateLinearVelocity;

  statePredicted.shape = currentState.shape;
  applyTransformation(statePredicted.shape, statePredicted.rigidbody.position, statePredicted.rigidbody.direction);

  statePredicted.time = currentState.time + multiplier * deltaTime;

  return statePredicted;
}

CarState Ghost::getState(float lapTime) {

  int futureFrameIndex = (int)lastStates.size();

  for (int i = 0; i < (int)lastStates.size(); i++) {
    CarState& state = lastStates[i];

    if (state.time > lapTime) {
      futureFrameIndex = i;
      break;
    }
  }

  CarState previousState, nextState;

  if (futureFrameIndex == 0) {

    previousState = predictState(lastStates[futureFrameIndex], lapTime, PAST);
    nextState = lastStates[futureFrameIndex];

  } else if (futureFrameIndex == (int)lastStates.size()) {

    previousState = lastStates[futureFrameIndex - 1];
    nextState = predictState(lastStates[futureFrameIndex - 1], timeToCompleteLap - lapTime, FUTURE);

  } else {

    previousState = lastStates[futureFrameIndex - 1];
    nextState = lastStates[futureFrameIndex];

  }

  return interpolateStates(previousState, nextState, lapTime);
}

void Ghost::reset() {
  timeActive.restart();
  lastStates.clear();
}

void Ghost::completeLap(float timeTaken) {
  timeToCompleteLap = timeTaken;
  game.ghosts.push_back(Ghost(game));
}

void Ghost::activateRendering() {
  isRendered = true;
  timeActive.restart();
}

void Ghost::render() {
  if (!isRendered) return;

  float totalTime = timeActive.getElapsedTime().asSeconds();
  float lapTime = totalTime - floor(totalTime / timeToCompleteLap) * timeToCompleteLap;

  CarState state = getState(lapTime);

  game.window.draw(state.shape);
}
