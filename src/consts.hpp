#pragma once

#include "sfml.hpp"

enum Orientation { FORWARD, REVERSE, LEFT, RIGHT, HOLD };

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;
const float CAR_ACCELERATION = 30;
const float CAR_DECELERATION = -60;
const float CAR_ANGULAR_VELOCITY = 0.5;
const int CAR_WIDTH = 20;
const int CAR_HEIGHT = 100;
const int EPS = 1e-9;

// For testing
const sf::Vector2f INITIAL_CAR_POSITION = { SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5 };
const sf::Vector2f INITIAL_CAR_DIRECTION = { 0, 1 };