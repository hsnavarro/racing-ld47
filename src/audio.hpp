#pragma once

#include "sfml.hpp"

class Audio {
  public:
    Audio(std::string);
    void play();
    void setVolume(float);
    bool isPlaying();

  private:
    sf::SoundBuffer audioBuffer;
    sf::Sound audio;
};