#pragma once

#include "sfml.hpp"

enum Orientation { FORWARD, REVERSE, LEFT, RIGHT, HOLD };

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const float CAR_ACCELERATION = 80;
const float CAR_BRAKE_ACCELERATION = -150;
const float CAR_REVERSE_ACCELERATION = -20;

const float CAR_ANGULAR_VELOCITY = 2;
const int CAR_WIDTH = 10;
const int CAR_HEIGHT = 20;

const int SIMULATION_FPS = 120;
const int DISPLAY_FPS = 60;

const int ANTI_ALIASING_LEVEL = 8;

// For testing
const sf::Vector2f INITIAL_CAR_POSITION = { SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5 };
const sf::Vector2f INITIAL_CAR_DIRECTION = { 0, 1 };