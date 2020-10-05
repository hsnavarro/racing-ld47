#pragma once

#include "sfml.hpp"

const std::string BACKGROUND_MUSIC_FILE = "./assets/music/background-music.wav";

class Audio {
  public:
    Audio(std::string);
    void play();
    void setVolume(float);

  private:
    sf::SoundBuffer audioBuffer;
    sf::Sound audio;
};