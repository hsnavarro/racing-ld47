#pragma once

#include "sfml.hpp"

const bool UI_MINIMAP_ENABLE = false;

const float UI_DEFAULT_MARGIN = 10.0f;

const int UI_SMALL_FONT_SIZE = 12;
const int UI_DEFAULT_FONT_SIZE = 15;
const int UI_LARGE_FONT_SIZE = 30;

class Game;

class UI {
public:
  UI(Game&);

  void setup();
  void update();
  void render();

public:
  sf::Font font;
  sf::View uiView;
  sf::View minimapView;

  sf::Text text;

  float lastT;
  bool finishedCountdown;
  sf::Clock clock;

  Game& game;
};
