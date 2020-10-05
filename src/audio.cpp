#include <cstdio>

#include "audio.hpp"

Audio::Audio(std::string fileName) {
  if (audioBuffer.loadFromFile(fileName)) {
    audio.setBuffer(audioBuffer);
  } else {
    printf("could not load file\n");
  }
}

void Audio::play() {
  if(!isPlaying()) audio.play(); 
}

void Audio::stop() {
  audio.pause();
  audio.stop();
}

void Audio::setVolume(float volume) {
  audio.setVolume(volume);
}

bool Audio::isPlaying() {
  return audio.getStatus() == sf::SoundSource::Playing;
}