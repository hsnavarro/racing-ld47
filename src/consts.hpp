#pragma once

#include "sfml.hpp"
#include "types.hpp"

const f32 EPS = 1e-9f;

const s32 SIMULATION_FPS = 120;
const f32 SIMULATION_DELTA_TIME = 1.0f / static_cast<f32>(SIMULATION_FPS);

const int SCREEN_WIDTH        = 800;
const int SCREEN_HEIGHT       = 600;
const sf::Vector2f SCREEN_SIZE { SCREEN_WIDTH, SCREEN_HEIGHT };
const int DISPLAY_FPS         = 60;
const int ANTI_ALIASING_LEVEL = 8;

const float UI_DEFAULT_MARGIN = 10.0f;
const int UI_DEFAULT_FONT_SIZE = 15;

// For testing
const sf::Vector2f INITIAL_CAR_POSITION = { SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5 };
const sf::Vector2f INITIAL_CAR_DIRECTION = { 0, -1 };
