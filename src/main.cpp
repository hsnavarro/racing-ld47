#include <cmath>
#include <iostream>

#include "sfml.hpp"
#include "game.hpp"

int main() {
  Game game;

  while (game.window.isOpen()) {
    game.handleEvents();
    game.update();
    game.render();
  }
}
