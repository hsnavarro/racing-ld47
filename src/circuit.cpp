#include "circuit.hpp"

#include "game.hpp"

#include <cstdio>

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
      game.completeLap();
    }
  }
}

void Circuit::render() const {
  game.window.draw(sprite);

  for (auto wall : walls) {
    sf::Vertex verts[] = { wall[0], wall[1] };
    game.window.draw(verts, 2, sf::Lines);
  }

  for (size_t i = 0; i < checkpoints.size(); i++) {
    const auto& checkpoint = checkpoints[i];

    const sf::Color color = (currentCheckpoint > i) ? sf::Color::Green : sf::Color::Blue;

    sf::Vertex verts[] = {
      { checkpoint[0], color },
      { checkpoint[1], color }
    };

    game.window.draw(verts, 2, sf::Lines);
  }
}

void Circuit::resetCheckpoints() {
  currentCheckpoint = 0;
}
