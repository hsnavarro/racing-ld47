#include "game.hpp"
#include "audio-system.hpp"

AudioSystem::AudioSystem() :
    backgroundMusic{ BACKGROUND_MUSIC_FILE },
    handbrakeFX{ HANDBRAKE_FX_FILE },
    slideFX{ SLIDE_FX_FILE },
    collisionFX{ COLLISION_FX_FILE },
    engineFX{ ENGINE_FX_FILE },
    engineStartFX{ ENGINE_START_FX_FILE },
    acceleratorFX{ ACCELERATOR_FX_FILE },
    countdownFX{ COUNTDOWN_FX_FILE },
    goSoundFX{ GO_SOUND_FX_FILE } {}

void AudioSystem::toggleMute() {
 backgroundMusic.toggleMute();
 handbrakeFX.toggleMute();
 slideFX.toggleMute();
 collisionFX.toggleMute();
 engineFX.toggleMute();
 engineStartFX.toggleMute();
 acceleratorFX.toggleMute();
 countdownFX.toggleMute();
 goSoundFX.toggleMute();
}