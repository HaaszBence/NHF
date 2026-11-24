/*
assets.h

Purpose: Centralized asset loading, storage, and unloading for the game engine.
High level overview:
    - Defines enum IDs for textures, fonts, sound effects, and music categories.
    - Loads fixed assets via enum indexing and dynamically scans music directories.
Last Updated: 2026-11-19
*/

#ifndef ASSETS_H // Include guard to prevent multiple inclusions of this header file
#define ASSETS_H

// Includes
// -------------------------------------------------
#include "raylib.h"
#include <stdbool.h>

// Enum definitions
// -------------------------------------------------
typedef enum {
    TEX_BG_SKY,
    TEX_BG_CLOUDS_CLOSE,
    TEX_BG_CLOUDS_FAR,
    TEX_BG_ROCKS,
    TEX_BG_GROUND,
    TEX_BG_COUNT
} TextureId;

typedef enum {
    FONT_REGULAR,
    FONT_BOLD,
    FONT_COUNT
} FontId;

typedef enum {
    SFX_NATURE,
    SFX_CLICK,
    SFX_UPGRADE,
    SFX_COUNT
} SfxId;

typedef enum {
    MUSIC_CAT_CHILL,
    MUSIC_CAT_GOOFY,
    MUSIC_CAT_COUNT
} MusicCategory;

typedef enum {
    TARGET_ANIM_WATER_BALL,
    TARGET_ANIM_FIREBALL,
    TARGET_ANIM_COUNT
} TargetAnimId;

// Function Declarations
// -------------------------------------------------

// Loads all registered assets and scans music folders; returns false if critical files fail.
bool assets_load(void);

// Unloads all loaded textures, fonts, sound effects, and music streams.
void assets_unload(void);

// Resource Getters
// -------------------------------------------------
Texture2D assets_tex(TextureId id);
Font assets_font(FontId id);
Sound assets_sfx(SfxId id);

// Returns a pointer to the loaded Music array for a category and writes its length to out_count.
Music *assets_music_list(MusicCategory cat, int *out_count);

// Returns a pointer to the frame array for the given target animation and writes the frame count to out_count.
const Texture2D *assets_target_frames(TargetAnimId id, int *out_count);

#endif // ASSETS_H