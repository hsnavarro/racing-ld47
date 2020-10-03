#pragma once

#include "sfml.hpp"
#include "types.hpp"

const f32 EPS = 1e-9f;

const s32 SIMULATION_FPS = 120;
const f32 SIMULATION_DELTA_TIME = 1.0f / static_cast<f32>(SIMULATION_FPS);

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float CAR_ENGINE_ACCELERATION = 80;
const float CAR_BRAKE_ACCELERATION = -150;
const float CAR_REVERSE_ACCELERATION = -20;

const float CAR_ANGULAR_VELOCITY = 2;
const int CAR_WIDTH = 10;
const int CAR_HEIGHT = 20;

const int DISPLAY_FPS = 60;

const int ANTI_ALIASING_LEVEL = 8;

// For testing
const sf::Vector2f INITIAL_CAR_POSITION = { SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5 };
const sf::Vector2f INITIAL_CAR_DIRECTION = { 0, 1 };
