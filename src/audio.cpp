#include <cstdio>

#include "audio.hpp"

Audio::Audio(const std::string& fileName) {
  if (audioBuffer.loadFromFile(fileName)) {
    audio.setBuffer(audioBuffer);
  } else {
    printf("Could not load file\n");
  }
}

void Audio::play() {
  if(!isPlaying()) audio.play();  
}

void Audio::stop() {
  audio.pause();
  audio.stop();
}

void Audio::setVolume(const float volume) { 
  audio.setVolume(volume); 
  }

void Audio::setPitch(const float pitch) { 
  audio.setPitch(pitch); 
}

bool Audio::isPlaying() const { 
  return audio.getStatus() == sf::SoundSource::Playing; 
}