/*
anim.h

Purpose: Stateless, composable visual animation modifiers for SpriteView elements.
High level overview:
    - Defines directional enums for linear movement.
    - Provides pure transform and frame-swapping modifiers (drift, bobbing, pulse, fade, frames) without drawing.
Last Updated: 2026-11-19
*/

#ifndef ANIM_H // Include guard to prevent multiple inclusions of this header file
#define ANIM_H

// Includes
// -------------------------------------------------
#include "presentation/render.h"
#include <stdbool.h>

// Enum definitions
// -------------------------------------------------
typedef enum {
    DRIFT_LEFT,
    DRIFT_RIGHT,
    DRIFT_UP,
    DRIFT_DOWN
} DriftDir;

// Function Declarations
// -------------------------------------------------

// Transform Modifiers
// -------------------------------------------------

// Applies continuous linear movement along the given direction, optionally looping seamlessly.
void anim_drift(SpriteView *v, float speed, DriftDir dir, bool loop);

// Applies a vertical sinusoidal floating motion to the view's offset.
void anim_bobbing(SpriteView *v, float amplitude, float freq);

// Applies a periodic scaling oscillation around the view's center.
void anim_pulse(SpriteView *v, float amount, float freq);

// Modulates the view's alpha transparency (0.0f = transparent, 1.0f = opaque).
void anim_fade(SpriteView *v, float alpha);

// Frame-Based Texture Modifiers
// -------------------------------------------------

// Continuously cycles through an array of separate textures at the given frames per second (FPS).
void anim_frames(SpriteView *v, const Texture2D *frames, int frame_count, float fps);

// Plays a frame sequence once starting from start_time; returns true while still playing, false when finished.
bool anim_frames_once(SpriteView *v, const Texture2D *frames, int frame_count, float fps, double start_time);

#endif