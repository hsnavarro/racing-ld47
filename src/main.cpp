#include <cmath>
#include <iostream>

#include "sfml.hpp"
#include "game.hpp"

int main() {
  Game game;

  game.setup();
  while (game.window.isOpen()) {
    game.handleEvents();
    game.update();
    game.render();
  }
}
