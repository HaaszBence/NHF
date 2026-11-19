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
// -------------------------------------------------
#include "raylib.h"
#include <stdbool.h>

// Function Declarations
// -------------------------------------------------

// Initializes the audio subsystem and applies initial master volume settings.
bool audio_init(float master_volume);

// Updates active music stream buffers (must be called once per frame).
void audio_update(void);

// Sets the global master volume (0.0f to 1.0f).
void audio_set_master_volume(float volume);

// Plays a loaded sound effect once.
void audio_play_sfx(Sound sfx);

// Starts or switches to the specified background music stream.
void audio_play_music(Music *music);

// Closes the audio device and cleans up audio state.
void audio_shutdown(void);

#endif