#include "ghost.hpp"
#include "game.hpp"
#include "car.hpp"

#include <iostream>

Ghost::Ghost() {
  timeActive.restart();
};

void Ghost::addState(const Game& game) {
  const Car& car = game.car;
  float currentLapTime = game.lapTime.getElapsedTime().asSeconds();

  auto shape = car.shape;
  shape.setTexture(&car.ghostTexture);
  lastStates.push_back({ car.rigidbody, shape, currentLapTime });
}

static sf::Vector2<f64> interpolateVectors(const sf::Vector2<f64>& currentFrameVector,const sf::Vector2<f64>& nextFrameVector, const f64 interpolationRatio) {
  return interpolationRatio * nextFrameVector + (1 - interpolationRatio) * currentFrameVector;
}

static void applyTransformation(sf::RectangleShape& shape, const sf::Vector2<f64>& position, const sf::Vector2<f64>& direction) {
  shape.setPosition(to_vector2f(position));
  shape.setRotation(getRotation(to_vector2f(direction)));
}

CarState Ghost::interpolateStates(const CarState& currentFrame, const CarState& nextFrame, const f64 interpolationTime) const {

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

static CarState predictState(const CarState& currentState, const f64 deltaTime, const Direction direction) {

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

CarState Ghost::getState(const float lapTime) const {

  int futureFrameIndex = (int)lastStates.size();

  for (int i = 0; i < (int)lastStates.size(); i++) {
    const CarState& state = lastStates[i];

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
}

void Ghost::clear() {
  timeActive.restart();
  lastStates.clear();
}

void Ghost::completeLap(const float timeTaken) {
  timeToCompleteLap = timeTaken;
}

void Ghost::activateRendering() {
  isRendered = true;
  timeActive.restart();
}

void Ghost::render(sf::RenderWindow& window) const {
  if (!isRendered) return;

  CarState state = getCurrentState();

  window.draw(state.shape);
}

CarState Ghost::getCurrentState() const {
  float totalTime = timeActive.getElapsedTime().asSeconds();
  float lapTime = 0.0f;
  if (timeToCompleteLap > EPS) lapTime = totalTime - floor(totalTime / timeToCompleteLap) * timeToCompleteLap;

  return getState(lapTime);
}
