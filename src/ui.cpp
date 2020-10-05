#include "ui.hpp"

#include "game.hpp"

void drawTextRight(sf::Text& text, float x, float y, sf::RenderWindow& window) {
  const auto size = text.getLocalBounds().width;
  text.setPosition(x - size, y);
  window.draw(text);
};

void drawTextCenter(sf::Text& text, float x, float y, sf::RenderWindow& window) {
  const auto w = text.getLocalBounds().width;
  const auto h = text.getLocalBounds().height;
  text.setPosition(x - w / 2, y - h / 2);
  window.draw(text);
};

UI::UI(Game& game_) : game { game_ } {

  if (!font.loadFromFile("assets/fonts/Monocons.ttf")) {
    printf("fail to load font!\n");
  }

  if constexpr (UI_MINIMAP_ENABLE) {
    minimapView.setSize(2*SCREEN_WIDTH,2*SCREEN_HEIGHT);
    minimapView.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
    minimapView.setViewport(sf::FloatRect(0.80f, 0.05f, 0.40f, 0.40f));
  }

  uiView.setSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  uiView.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

  text = sf::Text { "", font, UI_LARGE_FONT_SIZE };
  text.setOutlineColor(sf::Color::Black);
  text.setOutlineThickness(5);
}

void UI::setup() {
  clock.restart();
  readyCountdown = 4;
  text.setString("GET READY");
  text.setScale({ 1.0f, 1.0f });
}

void UI::update() {
  const auto t = clock.getElapsedTime().asSeconds();

  if (readyCountdown == 0) {
    game.onCountdown = false;
    text.setString("GO!");
    if (clock.getElapsedTime().asSeconds() >= 1.0f) {
      readyCountdown--;
    }
  } else if (readyCountdown == 4) {
    // GET READY / controls
    /*
    const auto scale = lerp(1.0f, 2.0f, t / 3.0f);
    text.setScale({ scale, scale });
    */

    if (clock.getElapsedTime().asSeconds() >= 2.0f) {
      readyCountdown--;
      clock.restart();
    }
  } else {
    char str[10];
    snprintf(str, 10, "%d", readyCountdown);

    text.setString(str);

    const auto scale = lerp(1.0f, 2.0f, t);
    text.setScale({ scale, scale });

    if (t >= 1.0f) {
      readyCountdown--;
      clock.restart();
    }
  }
}

void UI::render() {
  // Minimap
  if constexpr (UI_MINIMAP_ENABLE) {
    if (game.currentCircuit)
      game.currentCircuit->render(minimapView);
    game.car.renderIcon(minimapView);
  }

  // Texts
  char str[30];
  game.window.setView(uiView);

  if (game.state == Game::State::END_GAME and game.hasEscaped) {
    text.setString("Thank you for playing!");
    text.setScale({ 1.0f, 1.0f });
    drawTextCenter(text, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, game.window);
    return;
  };

  float verticalOffset = 0.0f;

  const auto maximumSize = [this] {
    sf::Text tmp("000.00s", font, UI_DEFAULT_FONT_SIZE);
    return tmp.getLocalBounds().width;
  }();


  // lap time
  snprintf(str, 30, "%.2fs", game.lapTime.getElapsedTime().asSeconds());
  sf::Text lapTimeText(str, font, UI_DEFAULT_FONT_SIZE);
  drawTextRight(lapTimeText, maximumSize, 10.0f, game.window);

  verticalOffset += UI_DEFAULT_MARGIN + lapTimeText.getLocalBounds().height;

  // last lap time
  if (game.lastLapTime > 0.0f) {
    snprintf(str, 30, "%.2fs", game.lastLapTime);
    sf::Text lastLapText(str, font, UI_SMALL_FONT_SIZE);
    drawTextRight(lastLapText, maximumSize + maximumSize, 13.0f, game.window);
  }

  // lap time limit
  if (game.currentCircuit) {
    snprintf(str, 30, "%.2fs", game.currentCircuit->lapTimeLimit);
    sf::Text lapTimeLimitText(str, font, UI_DEFAULT_FONT_SIZE);
    drawTextRight(lapTimeLimitText, maximumSize, 10.0f+verticalOffset, game.window);
  }

  // speed
  snprintf(str, 30, "%.2f px/s", getMagnitude(game.car.rigidbody.linearVelocity));
  sf::Text speedText(str, font, UI_DEFAULT_FONT_SIZE);
  drawTextRight(speedText, SCREEN_WIDTH - 10.0f, SCREEN_HEIGHT - 40.0f, game.window);

  //
  if (readyCountdown >= 0)
    drawTextCenter(text, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, game.window);
}
