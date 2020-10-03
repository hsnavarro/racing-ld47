#pragma once

#include <string_view>

#include "sfml.hpp"
#include "algebra.hpp"
#include "types.hpp"

class Game;

class Circuit {
public:
  Circuit(Game&);

  void setWalls(std::vector<sf::Line>);
  void setCheckpoints(std::vector<sf::Line>);
  void setTexture(const std::string&);

  void update(float);
  void render() const;

  void resetCheckpoints();

  std::vector<sf::Line> walls;
  std::vector<sf::Line> checkpoints;

  sf::Texture texture;
  sf::Sprite sprite;

  size_t currentCheckpoint = 0;

  Game& game;
};
