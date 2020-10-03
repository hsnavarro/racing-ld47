#include "sfml.hpp"
#include <iostream>
#include <cmath>

enum Orientation { CW, ACW };
enum Direction { FORWARD, REVERSE, LEFT, RIGHT };

const float CAR_ACCELERATION = 7;
const float CAR_DECELERATION = 10;
const float CAR_ANGULAR_VELOCITY = 4;
const int CAR_WIDTH = 4;
const int CAR_HEIGHT = 10;

class Movement {
public:
  float acceleration = CAR_ACCELERATION;
  float deceleration = CAR_DECELERATION;
  float angularVelocity = CAR_ANGULAR_VELOCITY;
  sf::Vector2f position;
  sf::Vector2f direction;
  sf::Vector2f linearVelocity = { 0.0f, 0.0f };

  Movement(sf::Vector2f initialPosition, sf::Vector2f initialDirection) {
    position = initialPosition;
    direction = initialDirection;
  }

  void rotate(float angle) {
    float newX = direction.x * cos(angle) - direction.y * sin(angle);
    float newY = direction.x * sin(angle) + direction.y * cos(angle);

    direction = { newX, newY };
  }

  void simulate(float time) {
    sf::Vector2f accelerationVector = acceleration * direction;
    linearVelocity += accelerationVector * time;

  }



};

class Car {
public:
  Movement movement;
  sf::RectangleShape shape;

  Car(sf::Vector2f initialPosition, sf::Vector2f initialDirection) {
    movement = { initialPosition, initialDirection };
    shape.setSize({ CAR_WIDTH, CAR_HEIGHT });
    shape.setPosition(initialPosition);
  }

  void rotate(float time, int orientation) {
    float angle = movement.angularVelocity * time;

    if (orientation == CW) movement.rotate(-angle);
    else movement.rotate(angle);
  }

}


};

class Game {
  Car car;

  bool turnRight = false;


  void update(float timeElapsed) {



  }
};

int main() {

  sf::RenderWindow window;
  sf::Clock clock;
  sf::Event event;

  float timeToUpdateSimulation = (1 / 200.0);

  while (window.isOpen()) {
    float frameTime = clock.restart().asSeconds();

    while (frameTime > 0.0) {
      float deltaTime = std::min(frameTime, timeToUpdateSimulation);
      game.update(deltaTime);
      frameTime -= deltaTime;
    }

    game.render();

    while (game.window.pollEvent(event)) {
      game.handleEvent(event);
    }
  }
}