#include "circuit.hpp"

#include "game.hpp"

Circuit::Circuit(Game& game) : game { game }
{}

void Circuit::setWalls(std::vector<sf::Line> walls_) {
  walls = walls_;
}

void Circuit::setCheckpoints(std::vector<sf::Line> checkpoints_) {
  checkpoints = checkpoints_;
}

void Circuit::setTexture(const std::string& path) {
  if(texture.loadFromFile(path)) {
    texture.setSmooth(true);
    sprite.setTexture(texture);
  }
}

void Circuit::update(float) {
  if (currentCheckpoint >= checkpoints.size())
    return;

  if (carIntersectsLine(game.car, checkpoints[currentCheckpoint])) {
    currentCheckpoint++;
    if (currentCheckpoint == checkpoints.size()) {
      // completed track
    }
  }
}

void Circuit::render() {
  game.window.draw(sprite);

  for (auto wall : walls) {
    sf::Vertex verts[] = { wall[0], wall[1] };
    game.window.draw(verts, 2, sf::Lines);
  }
}
