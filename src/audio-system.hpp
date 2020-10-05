#pragma once

#include "audio.hpp"

const std::string BACKGROUND_MUSIC_FILE = "./assets/music/background-music.wav";
const std::string SLIDE_FX_FILE = "./assets/sfx/slide-fx2.wav";
const std::string ACCELERATOR_FX_FILE = "./assets/sfx/accelerator-fx.wav";
const std::string HANDBRAKE_FX_FILE = "./assets/sfx/handbrake-fx.wav";
const std::string COLLISION_FX_FILE = "./assets/sfx/collision-fx.wav";

class Game;

class AudioSystem {
public:
  AudioSystem(Game&);

public:
  Game& game;
  Audio backgroundMusic;
  Audio handbrakefx;
  Audio slidefx;
  Audio collisionfx;
};