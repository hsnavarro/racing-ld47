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

const float DRIFT_ANGLE = 40.0f;
const float DRIFT_MIN_VELOCITY = 20.0f;

const bool IS_DRIFTING_BURST_ENABLED = false;
const float DRIFT_BURST_TIME_THRESHOLD = 0.4f;
const auto BURST_FORCE = 100.0;

const float ICON_RADIUS = 30.0f;
const sf::Color ICON_COLOUR{255,0,0,255};

// For testing
const sf::Vector2f INITIAL_CAR_POSITION = { SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5 };
const sf::Vector2f INITIAL_CAR_DIRECTION = { 0, -1 };
