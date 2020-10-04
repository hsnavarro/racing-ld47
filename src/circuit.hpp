#pragma once

#include <string>
#include <vector>

#include "sfml.hpp"
#include "algebra.hpp"
#include "types.hpp"

const int CIRCUIT_TILE_SIZE = 108;

class Game;

class Circuit {
private:

  struct TileType {
    enum Direction {
      DIR_NONE  = 0x0,
      DIR_UP    = 0x1,
      DIR_DOWN  = 0x2,
      DIR_RIGHT = 0x4,
      DIR_LEFT  = 0x8,
    };

    sf::Sprite sprite;
    sf::Line checkpoint;
    std::vector<sf::Line> walls;
    bool hasCheckpoint;
    u8 exits[16];

    inline void setTilePosition(size_t pos_y, size_t pos_x) {
      sprite.setTexture(textureAtlas);
      sprite.setTextureRect(
        {
          (CIRCUIT_TILE_SIZE + 2) * static_cast<int>(pos_x) + 1,
          (CIRCUIT_TILE_SIZE + 2) * static_cast<int>(pos_y) + 1,
          CIRCUIT_TILE_SIZE,
          CIRCUIT_TILE_SIZE
        }
      );
    }
  };

  struct Tile {
    size_t type;
  };

public:
  Circuit(Game&);

  bool loadFromFile(const std::string&);
  void setLapTimeLimit(float);

  void update(float);
  void render(const sf::View&) const;

  void resetCheckpoints();
  void startRace();

  static void loadAtlas();

public:
  std::vector<sf::Line> walls;
  std::vector<sf::Line> checkpoints;

  std::vector<std::vector<Tile>> tiles;
  size_t initialPosY = 0, initialPosX = 0;

  float lapTimeLimit;
  size_t currentCheckpoint = 0;

  static sf::Texture textureAtlas;
  static std::vector<TileType> tileTypes;

  Game& game;
};
