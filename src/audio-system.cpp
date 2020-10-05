#include "game.hpp"
#include "audio-system.hpp"

AudioSystem::AudioSystem(Game& _game) : 
    game{ _game }, 
    backgroundMusic { BACKGROUND_MUSIC_FILE }, 
    handbrakefx { HANDBRAKE_FX_FILE }, 
    slidefx { SLIDE_FX_FILE }, 
    collisionfx1 { COLLISION_FX_FILE }, 
    collisionfx2 { COLLISION_FX2_FILE } {

    //collisionfx  = { { COLLISION_FX_FILE }, { COLLISION_FX2_FILE } };
}