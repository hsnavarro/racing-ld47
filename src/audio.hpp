#pragma once

#include "sfml.hpp"

class Audio {
  public:
    Audio(const std::string&);
    void play();
    void stop();
    void setVolume(const float);
    void setPitch(const float);
    bool isPlaying() const;

  private:
    sf::SoundBuffer audioBuffer;
    sf::Sound audio;
};