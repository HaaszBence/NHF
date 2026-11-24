/*
assets.c

Purpose: Implementation of centralized asset management.
Last Updated: 2026-11-22
*/

// Includes
// -------------------------------------------------
#include "core/assets.h"
#include "core/config.h"
#include <stdlib.h>

// Target Animation Frame Counts
// -------------------------------------------------
#define WATER_BALL_FRAME_COUNT 12
#define FIREBALL_FRAME_COUNT   8

// Asset Path Tables
// -------------------------------------------------
static const char *texture_paths[TEX_BG_COUNT] = {
    [TEX_BG_SKY] = ASSETS_PATH "images/bg/layers/sky.png",
    [TEX_BG_CLOUDS_CLOSE] = ASSETS_PATH "images/bg/layers/clouds_close.png",
    [TEX_BG_CLOUDS_FAR] = ASSETS_PATH "images/bg/layers/clouds_far.png",
    [TEX_BG_ROCKS] = ASSETS_PATH "images/bg/layers/rocks.png",
    [TEX_BG_GROUND] = ASSETS_PATH "images/bg/layers/ground.png"
};

static const char *font_paths[FONT_COUNT] = {
    [FONT_REGULAR] = ASSETS_PATH "fonts/pixel.ttf",
    [FONT_BOLD] = ASSETS_PATH "fonts/pixel_bold.ttf"
};

static const char *sfx_paths[SFX_COUNT] = {
    [SFX_NATURE] = ASSETS_PATH "sounds/sfx/nature.mp3",
    [SFX_CLICK] = ASSETS_PATH "sounds/sfx/click.wav", // Not included yet
    [SFX_UPGRADE] = ASSETS_PATH "sounds/sfx/upgrade.wav" // Not included yet
};

static const char *music_dirs[MUSIC_CAT_COUNT] = {
    [MUSIC_CAT_CHILL] = ASSETS_PATH "sounds/music/chill",
    [MUSIC_CAT_GOOFY] = ASSETS_PATH "sounds/music/goofy"
};

// Internal Storage
// -------------------------------------------------
static Texture2D textures[TEX_BG_COUNT];
static Texture2D water_ball_frames[WATER_BALL_FRAME_COUNT];
static Texture2D fireball_frames[FIREBALL_FRAME_COUNT];

static Font fonts[FONT_COUNT];
static Sound sfx[SFX_COUNT];

static Music *music_tracks[MUSIC_CAT_COUNT];
static int music_counts[MUSIC_CAT_COUNT];

// Public Functions
// -------------------------------------------------
bool assets_load(void) {
    for (int i = 0; i < TEX_BG_COUNT; i++) {
        textures[i] = LoadTexture(texture_paths[i]);
        if (textures[i].id == 0) return false;
    }

    for (int i = 0; i < WATER_BALL_FRAME_COUNT; i++) {
        const char *path = TextFormat(ASSETS_PATH "images/targets/water_ball/frame%02d.png", i + 1);
        water_ball_frames[i] = LoadTexture(path);
        if (water_ball_frames[i].id == 0) return false;
    }

    for (int i = 0; i < FIREBALL_FRAME_COUNT; i++) {
        const char *path = TextFormat(ASSETS_PATH "images/targets/fireball/frame%02d.png", i + 1);
        fireball_frames[i] = LoadTexture(path);
        if (fireball_frames[i].id == 0) return false;
    }

    for (int i = 0; i < FONT_COUNT; i++) {
        fonts[i] = LoadFont(font_paths[i]);
        if (fonts[i].texture.id == 0) return false;
    }

    for (int i = 0; i < SFX_COUNT; i++) {
        sfx[i] = LoadSound(sfx_paths[i]);
        // Only require SFX_NATURE strictly until click.wav and upgrade.wav are added
        if (i == SFX_NATURE && sfx[i].frameCount == 0) return false;
    }

    for (int c = 0; c < MUSIC_CAT_COUNT; c++) {
        FilePathList files = LoadDirectoryFiles(music_dirs[c]);
        music_counts[c] = (int)files.count;
        music_tracks[c] = (files.count > 0) ? malloc(sizeof(Music) * files.count) : NULL;

        for (unsigned int i = 0; i < files.count; i++) {
            music_tracks[c][i] = LoadMusicStream(files.paths[i]);
        }
        UnloadDirectoryFiles(files);
    }

    return true;
}

void assets_unload(void) {
    for (int i = 0; i < TEX_BG_COUNT; i++) UnloadTexture(textures[i]);
    for (int i = 0; i < WATER_BALL_FRAME_COUNT; i++) UnloadTexture(water_ball_frames[i]);
    for (int i = 0; i < FIREBALL_FRAME_COUNT; i++) UnloadTexture(fireball_frames[i]);

    for (int i = 0; i < FONT_COUNT; i++) UnloadFont(fonts[i]);

    for (int i = 0; i < SFX_COUNT; i++) {
        if (sfx[i].frameCount > 0) UnloadSound(sfx[i]);
    }

    for (int c = 0; c < MUSIC_CAT_COUNT; c++) {
        for (int i = 0; i < music_counts[c]; i++) {
            UnloadMusicStream(music_tracks[c][i]);
        }
        free(music_tracks[c]);
        music_tracks[c] = NULL;
        music_counts[c] = 0;
    }
}

Texture2D assets_tex(TextureId id) {
    return textures[id];
}

Font assets_font(FontId id) {
    return fonts[id];
}

Sound assets_sfx(SfxId id) {
    return sfx[id];
}

Music *assets_music_list(MusicCategory cat, int *out_count) {
    if (out_count) *out_count = music_counts[cat];
    return music_tracks[cat];
}

const Texture2D *assets_target_frames(TargetAnimId id, int *out_count) {
    switch (id) {
        case TARGET_ANIM_WATER_BALL:
            if (out_count) *out_count = WATER_BALL_FRAME_COUNT;
            return water_ball_frames;
        case TARGET_ANIM_FIREBALL:
            if (out_count) *out_count = FIREBALL_FRAME_COUNT;
            return fireball_frames;
        default:
            if (out_count) *out_count = 0;
            return NULL;
    }
}