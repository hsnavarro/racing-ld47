#include "game.hpp"
#include "audio-system.hpp"

AudioSystem::AudioSystem(Game& _game) : 
    game{ _game }, 
    backgroundMusic { BACKGROUND_MUSIC_FILE }, 
    handbrakeFX { HANDBRAKE_FX_FILE }, 
    slideFX { SLIDE_FX_FILE }, 
    collisionFX { COLLISION_FX_FILE }, 
    engineFX { ENGINE_FX_FILE }, 
    engineStartFx { ENGINE_START_FX_FILE }, 
    acceleratorFX { ACCELERATOR_FX_FILE }, 
    countdownfx { COUNTDOWN_FX_FILE }, 
    goSoundFx { GO_SOUND_FX_FILE } {

    //collisionfx  = { { COLLISION_FX_FILE }, { COLLISION_FX2_FILE } };
}