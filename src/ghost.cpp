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

static sf::Vector2<f64> interpolateVectors(sf::Vector2<f64> currentFrameVector, sf::Vector2<f64> nextFrameVector, f64 interpolationRatio) {
  return interpolationRatio * nextFrameVector + (1 - interpolationRatio) * currentFrameVector;
}

void applyTransformation(sf::RectangleShape& shape, sf::Vector2<f64>& position, sf::Vector2<f64>& direction) {
  shape.setPosition(to_vector2f(position));
  shape.setRotation(getRotation(to_vector2f(direction)));
}

CarState Ghost::interpolateStates(CarState& currentFrame, CarState& nextFrame, f64 interpolationTime) {

  f64 interpolationRatio = (interpolationTime - currentFrame.time) / (nextFrame.time - currentFrame.time);

  sf::Vector2<f64> interPosition = interpolateVectors(to_vector2f64(currentFrame.shape.getPosition()), to_vector2f64(nextFrame.shape.getPosition()), interpolationRatio);
  sf::Vector2<f64> interDirection = interpolateVectors(currentFrame.rigidbody.direction, nextFrame.rigidbody.direction, interpolationRatio);
  sf::Vector2<f64> interLinearVelocity = interpolateVectors(currentFrame.rigidbody.linearVelocity, nextFrame.rigidbody.linearVelocity, interpolationRatio);

  interDirection = getUnitVector(interDirection);

  float interTime = static_cast<float>(interpolationTime);

  Rigidbody interRigidbody;
  interRigidbody.position = interPosition;
  interRigidbody.direction = interDirection;
  interRigidbody.linearVelocity = interLinearVelocity;

  sf::RectangleShape interShape = currentFrame.shape;
  applyTransformation(interShape, interPosition, interDirection);

  return { interRigidbody, interShape, interTime };
}

enum Direction { FUTURE, PAST };

CarState predictState(CarState& currentState, f64 deltaTime, Direction direction) {

  f64 multiplier = direction == FUTURE ? 1.0 : -1.0;

  auto& currentStateDirection = currentState.rigidbody.direction;
  auto& currentStatePosition = currentState.rigidbody.position;
  auto& currentStateLinearVelocity = currentState.rigidbody.direction;

  CarState statePredicted;

  statePredicted.rigidbody.linearVelocity = currentStateLinearVelocity + multiplier * deltaTime * CAR_ENGINE_ACCELERATION * currentStateDirection;
  statePredicted.rigidbody.direction = currentStateDirection;
  statePredicted.rigidbody.position = currentStatePosition + multiplier * deltaTime * currentStateLinearVelocity;

  statePredicted.shape = currentState.shape;
  applyTransformation(statePredicted.shape, statePredicted.rigidbody.position, statePredicted.rigidbody.direction);

  statePredicted.time = static_cast<float>(currentState.time + multiplier * deltaTime);

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
