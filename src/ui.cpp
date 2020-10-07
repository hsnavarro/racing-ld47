#include "ui.hpp"

#include <cmath>
#include "game.hpp"

void static drawTextRight(sf::Text& text, const float x, const float y, sf::RenderWindow& window) {
  const auto size = text.getLocalBounds().width;
  text.setPosition(x - size, y);
  window.draw(text);
};

void static drawTextCenter(sf::Text& text, const float x, const float y, sf::RenderWindow& window) {
  const auto w = text.getLocalBounds().width;
  const auto h = text.getLocalBounds().height;
  text.setPosition(x - w / 2, y - h / 2);
  window.draw(text);
};

UI::UI(Game& game_) : game{ game_ } {

  if (!font.loadFromFile("assets/fonts/Monocons.ttf")) {
    printf("fail to load font!\n");
  }

  if constexpr (UI_MINIMAP_ENABLE) {
    minimapView.setSize(2 * SCREEN_WIDTH, 2 * SCREEN_HEIGHT);
    minimapView.setCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    minimapView.setViewport(sf::FloatRect(0.80f, 0.05f, 0.40f, 0.40f));
  }

  uiView.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  uiView.setCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

  text = sf::Text{ "", font, UI_LARGE_FONT_SIZE };
  text.setOutlineColor(sf::Color::Black);
  text.setOutlineThickness(5);
}

void UI::setup() {
  clock.restart();
  finishedCountdown = false;
  lastT = 0.0f;
  text.setString("GET READY");
  text.setScale({ 1.0f, 1.0f });
}

void UI::update() {
  const auto t = clock.getElapsedTime().asSeconds();

  if (t > 7.0f or game.hasEscaped) return;

  if (lastT <= 6.0f and t > 6.0f) {
    finishedCountdown = true;
  } if (lastT <= 5.0f and t > 5.0f) {
    game.onCountdown = false;
    text.setString("GO!");
    game.audioSystem.goSoundFX.play();
  } else if (t <= 5.0f) {
    for (int i = 3; i >= 1; i--) {
      float exhibitionTime = 5.0f - float(i);

      if (lastT <= exhibitionTime and t > exhibitionTime) {
        char str[10];
        snprintf(str, 10, "%d", i);

        text.setString(str);
        game.audioSystem.countdownFX.play();
      }

      if (t > exhibitionTime) {
        const auto scale = lerp(1.0f, 2.0f, t - exhibitionTime);
        text.setScale({ scale, scale });
      }
    }
  }

  lastT = t;
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

    float timeUntilEscape = clock.getElapsedTime().asSeconds();
    if (timeUntilEscape > 2.0f) {
      text.setString("Developers:\n\nHenrique Navarro\n\nLuciano Barreira\n\nNaum Azeredo\n\nGithub:\n\n/hsnavarro/racing-ld47\n");
    }

    text.setScale({ 1.0f, 1.0f });
    drawTextCenter(text, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, game.window);

    return;
  };

  if (game.state == Game::State::MAIN_MENU) {
    const auto t = clock.getElapsedTime().asSeconds();

    auto getPos = [](float f, float d, float w) {
      return f - static_cast<int>(f / (d + w)) * (d + w) - w;
    };

    sf::Text title1Text("CIRCUIT", font, 60);
    auto width = title1Text.getLocalBounds().width;
    title1Text.setPosition(getPos(100.0f * cos(1.0f + t * 2.0f) + t * 330.0f, SCREEN_WIDTH, width), 200.0f);
    title1Text.setOutlineColor(sf::Color::Black);
    title1Text.setOutlineThickness(3);
    game.window.draw(title1Text);

    sf::Text title2Text("ESCAPE", font, 55);
    width = title2Text.getLocalBounds().width;
    title2Text.setPosition(getPos(200.0f * sin(2.1f + t * 1.2f) + t * 415.0f, SCREEN_WIDTH, width), 300.0f);
    title2Text.setOutlineColor(sf::Color::Black);
    title2Text.setOutlineThickness(3);
    game.window.draw(title2Text);

    sf::Text inst0("W/I/N - accelerate", font, 20);
    inst0.setOutlineColor(sf::Color::Black);
    inst0.setOutlineThickness(2);
    drawTextCenter(inst0, SCREEN_WIDTH / 2 + 10.0f * cos(t), 360.0f + 340.0f + 2.1f * cos(0.6f + t * 0.4f), game.window);

    sf::Text inst1("S/K/M - break/reverse", font, 20);
    inst1.setOutlineColor(sf::Color::Black);
    inst1.setOutlineThickness(2);
    drawTextCenter(inst1, SCREEN_WIDTH / 2 + 10.0f * cos(0.1f + t * 0.2f), 360.0f + 370.0f + 2.7f * cos(0.2f + t * 0.1f), game.window);

    sf::Text inst2("A/J/X - turn left", font, 20);
    inst2.setOutlineColor(sf::Color::Black);
    inst2.setOutlineThickness(2);
    drawTextCenter(inst2, SCREEN_WIDTH / 2 + 10.2f * cos(1.1f + t * 1.7f), 360.0f + 400.0f + 2.1f * cos(1.7f + t * 3.1f), game.window);

    sf::Text inst3("D/L/C - turn right", font, 20);
    inst3.setOutlineColor(sf::Color::Black);
    inst3.setOutlineThickness(2);
    drawTextCenter(inst3, SCREEN_WIDTH / 2 + 13.0f * cos(0.8f + t * 2.2f), 360.0f + 430.0f + 2.3f * cos(8.8f + t * 1.1f), game.window);

    sf::Text inst4("space - handbrake", font, 20);
    inst4.setOutlineColor(sf::Color::Black);
    inst4.setOutlineThickness(2);
    drawTextCenter(inst4, SCREEN_WIDTH / 2 + 12.0f * cos(0.7f + t * 3.9f), 360.0f + 460.0f + 2.9f * cos(2.0f + t * 0.2f), game.window);

    sf::Text inst5("press 'space' to start", font, 20);
    inst5.setOutlineColor(sf::Color::Black);
    inst5.setOutlineThickness(2);
    drawTextCenter(inst5, SCREEN_WIDTH / 2 + 10.7f * cos(2.2f + t * 1.2f), 360.0f + 530.0f + 2.3f * cos(3.1f + t * 3.8f), game.window);

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
  lapTimeText.setOutlineColor(sf::Color::Black);
  lapTimeText.setOutlineThickness(1);
  drawTextRight(lapTimeText, maximumSize, 10.0f, game.window);

  verticalOffset += UI_DEFAULT_MARGIN + lapTimeText.getLocalBounds().height;

  // last lap time
  if (game.lastLapTime > 0.0f) {
    snprintf(str, 30, "%.2fs", game.lastLapTime);
    sf::Text lastLapText(str, font, UI_SMALL_FONT_SIZE);
    lastLapText.setOutlineColor(sf::Color::Black);
    lastLapText.setOutlineThickness(1);
    drawTextRight(lastLapText, maximumSize + maximumSize, 13.0f, game.window);
  }

  // lap time limit
  if (game.currentCircuit) {
    snprintf(str, 30, "%.2fs", game.currentCircuit->lapTimeLimit);
    sf::Text lapTimeLimitText(str, font, UI_DEFAULT_FONT_SIZE);
    lapTimeLimitText.setOutlineColor(sf::Color::Black);
    lapTimeLimitText.setOutlineThickness(1);
    drawTextRight(lapTimeLimitText, maximumSize, 10.0f + verticalOffset, game.window);
  }

  // speed
  snprintf(str, 30, "%.2f px/s", getMagnitude(game.car.rigidbody.linearVelocity));
  sf::Text speedText(str, font, UI_DEFAULT_FONT_SIZE);
  speedText.setOutlineColor(sf::Color::Black);
  speedText.setOutlineThickness(1);
  drawTextRight(speedText, SCREEN_WIDTH - 10.0f, SCREEN_HEIGHT - 40.0f, game.window);

  //
  if (!finishedCountdown) drawTextCenter(text, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, game.window);
}
