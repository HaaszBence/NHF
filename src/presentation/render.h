/*
render.h

Purpose: Core visual representation and pure rendering primitives for the presentation layer.
High level overview:
    - Defines the SpriteView structure holding texture, transform, tint, and wrapping properties.
    - Provides functions to initialize default views and execute pure rendering without side effects.
Last Updated: 2026-11-19
*/

#ifndef RENDER_H // Include guard to prevent multiple inclusions of this header file
#define RENDER_H

// Includes
// -------------------------------------------------
#include "raylib.h"
#include <stdbool.h>

// Structure definitions
// -------------------------------------------------
typedef struct {
    Texture2D tex;      // Active texture to render
    Rectangle dest;     // Base destination position and dimensions
    Vector2 offset;     // Positional offset applied by animation modifiers
    float scale;        // Scale multiplier (default: 1.0f)
    float rotation;     // Rotation angle in degrees (default: 0.0f)
    Color tint;         // Color modulation and alpha (default: WHITE)
    bool wrap_x;        // Enables seamless horizontal tiling (e.g., scrolling clouds)
} SpriteView;

// Function Declarations
// -------------------------------------------------

// Creates a default, static SpriteView from a texture and destination rectangle.
SpriteView sprite_view_create(Texture2D tex, Rectangle dest);

// Pure render: draws the SpriteView using its current properties without modifying state.
void sprite_view_draw(const SpriteView *v);

#endif