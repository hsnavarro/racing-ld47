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
  audio.play();
}

void Audio::setVolume(float volume) {
  audio.setVolume(volume);
}