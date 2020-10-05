#pragma once

#include "audio.hpp"

const std::string BACKGROUND_MUSIC_FILE = "./assets/music/background-music.wav";
const std::string SLIDE_FX_FILE = "./assets/sfx/slide-fx2.wav";
const std::string ACCELERATOR_FX_FILE = "./assets/sfx/accelerator-fx.wav";
const std::string HANDBRAKE_FX_FILE = "./assets/sfx/handbrake-fx.wav";
const std::string COLLISION_FX_FILE = "./assets/sfx/collision-fx2.wav";
const std::string ENGINE_FX_FILE = "./assets/sfx/motor-engine-on-fx.wav";
const std::string ENGINE_START_FX_FILE = "./assets/sfx/motor-engine-start-fx.wav";
const std::string COUNTDOWN_FX_FILE = "./assets/sfx/countdown-fx.wav";
const std::string GO_SOUND_FX_FILE = "./assets/sfx/go-sound-fx.wav";

class Game;

class AudioSystem {
public:
  AudioSystem(Game&);

public:
  Game& game;
  Audio backgroundMusic;
  Audio handbrakeFX;
  Audio slideFX;
  Audio collisionFX;
  Audio engineFX;
  Audio engineStartFx;
  Audio acceleratorFX;
  Audio countdownfx;
  Audio goSoundFx;
  //std::vector<Audio> collisionfx;
};