/*
audio.h

Purpose: Audio playback and volume management for background music and sound effects.
High level overview:
    - Manages audio device initialization, per-frame music stream updates, and volume scaling.
    - Provides a clean interface to trigger SFX and switch background tracks from presentation scenes.
Last Updated: 2026-11-19
*/

#ifndef AUDIO_H // Include guard to prevent multiple inclusions of this header file
#define AUDIO_H

// Includes
// --------------------------------------------------------------------------------------------------
#include "raylib.h"
#include <stdbool.h>
#include "core/assets.h" // Needed for MusicCategory and SfxId enum values

// Function Declarations
// --------------------------------------------------------------------------------------------------

// Lifecycle Functions
// -------------------------------------------------

// Initializes the audio subsystem and applies initial volume settings.
void audio_init(void);

// Updates active music stream buffers (must be called once per frame).
void audio_update(void);

// Closes the audio device and cleans up audio state.
void audio_deinit(void);

// Music & Ambient Control
// -------------------------------------------------

// Starts or switches to the specified background music stream.
void audio_play_music(MusicCategory category);

// Starts playing the ambient sound effect in a loop.
void audio_start_ambient(void);

// Stops the currently playing ambient sound.
void audio_stop_ambient(void);

// Volume setters [0.0f .. 1.0f]
// -------------------------------------------------
void audio_set_music_volume(float volume);
void audio_set_sfx_volume(float volume);
void audio_set_ambient_volume(float volume);
void audio_set_master_volume(float volume);

// Volume Getters
// -------------------------------------------------
float audio_get_music_volume(void);
float audio_get_sfx_volume(void);
float audio_get_ambient_volume(void);
float audio_get_master_volume(void);

// Sound Effect Playback
// -------------------------------------------------

// Plays a loaded sound effect once.
void audio_play_sfx(SfxId id);

#endif // AUDIO_H