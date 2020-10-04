#include "ui.hpp"

#include "game.hpp"

UI::UI(Game& game_) : game { game_ } {

  uiView.setSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  uiView.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

  minimapView.setSize(2*SCREEN_WIDTH,2*SCREEN_HEIGHT);
  minimapView.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
  minimapView.setViewport(sf::FloatRect(0.80f, 0.05f, 0.40f, 0.40f));
}

void UI::render() {
  // Minimap
  if constexpr (UI_MINIMAP_ENABLE) {
    if (game.currentCircuit)
      game.currentCircuit->render(minimapView);
    game.car.renderIcon(minimapView);
  }

  // Texts
  auto drawText = [this] (sf::Text& text, float x, float y, sf::RenderWindow& window) {
    const auto size = text.getLocalBounds().width;
    text.setPosition(x - size, y);
    window.draw(text);
  };

  char text[30];
  game.window.setView(uiView);

  float verticalOffset = 0.0f;

  const auto maximumSize = [this] {
    sf::Text tmp("000.00s", game.font, UI_DEFAULT_FONT_SIZE);
    return tmp.getLocalBounds().width;
  }();


  // lap time
  snprintf(text, 30, "%.2fs", game.lapTime.getElapsedTime().asSeconds());
  sf::Text lapTimeText(text, game.font, UI_DEFAULT_FONT_SIZE);
  drawText(lapTimeText, maximumSize, 10.0f, game.window);

  verticalOffset += UI_DEFAULT_MARGIN + lapTimeText.getLocalBounds().height;

  // last lap time
  if (game.lastLapTime > 0.0f) {
    snprintf(text, 30, "%.2fs", game.lastLapTime);
    sf::Text lastLapText(text, game.font, UI_SMALL_FONT_SIZE);
    drawText(lastLapText, maximumSize + maximumSize, 13.0f, game.window);
  }

  // lap time limit
  if (game.currentCircuit) {
    snprintf(text, 30, "%.2fs", game.currentCircuit->lapTimeLimit);
    sf::Text lapTimeLimitText(text, game.font, UI_DEFAULT_FONT_SIZE);
    drawText(lapTimeLimitText, maximumSize, 10.0f+verticalOffset, game.window);
  }

  // speed
  snprintf(text, 30, "%.2f px/s", getMagnitude(game.car.rigidbody.linearVelocity));
  sf::Text speedText(text, game.font, UI_DEFAULT_FONT_SIZE);
  drawText(speedText, SCREEN_WIDTH - 10.0f, SCREEN_HEIGHT - 40.0f, game.window);
}
