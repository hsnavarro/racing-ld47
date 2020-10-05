#include "game.hpp"
#include "audio-system.hpp"

AudioSystem::AudioSystem(Game& _game) : 
    game{ _game }, 
    backgroundMusic { BACKGROUND_MUSIC_FILE }, 
    handbrakefx { HANDBRAKE_FX_FILE }, 
    slidefx { SLIDE_FX_FILE }{}