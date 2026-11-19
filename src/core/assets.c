/*
assets.c

Purpose: Implementation of centralized asset management.
Last Updated: 2026-11-19
*/

// Includes
// -------------------------------------------------
#include "core/assets.h"
#include "core/config.h"
#include <stdlib.h>

// Asset Path Tables
// -------------------------------------------------
static const char *texture_paths[TEX_COUNT] = {
    [TEX_BG_SKY]    = ASSETS_PATH "images/bg/sky.png",
    [TEX_BG_CLOUDS] = ASSETS_PATH "images/bg/clouds.png",
    [TEX_BG_TREES]  = ASSETS_PATH "images/bg/trees.png",
    [TEX_MAIN_TREE] = ASSETS_PATH "images/tree.png"
};

static const char *font_paths[FONT_COUNT] = {
    [FONT_MAIN]  = ASSETS_PATH "fonts/main.ttf",
    [FONT_TITLE] = ASSETS_PATH "fonts/title.ttf"
};

static const char *sfx_paths[SFX_COUNT] = {
    [SFX_CLICK]   = ASSETS_PATH "sounds/sfx/click.wav",
    [SFX_UPGRADE] = ASSETS_PATH "sounds/sfx/upgrade.wav"
};

static const char *music_dirs[MUSIC_CAT_COUNT] = {
    [MUSIC_CAT_CHILL] = ASSETS_PATH "sounds/music/chill",
    [MUSIC_CAT_GOOFY] = ASSETS_PATH "sounds/music/goofy"
};

// Internal Storage
// -------------------------------------------------
static Texture2D textures[TEX_COUNT];
static Font      fonts[FONT_COUNT];
static Sound     sfx[SFX_COUNT];

static Music    *music_tracks[MUSIC_CAT_COUNT];
static int       music_counts[MUSIC_CAT_COUNT];

// Public Functions
// -------------------------------------------------
bool assets_load(void) {
    for (int i = 0; i < TEX_COUNT; i++) {
        textures[i] = LoadTexture(texture_paths[i]);
        if (textures[i].id == 0) return false;
    }

    for (int i = 0; i < FONT_COUNT; i++) {
        fonts[i] = LoadFont(font_paths[i]);
        if (fonts[i].texture.id == 0) return false;
    }

    for (int i = 0; i < SFX_COUNT; i++) {
        sfx[i] = LoadSound(sfx_paths[i]);
        if (sfx[i].frameCount == 0) return false;
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
    for (int i = 0; i < TEX_COUNT; i++) UnloadTexture(textures[i]);
    for (int i = 0; i < FONT_COUNT; i++) UnloadFont(fonts[i]);
    for (int i = 0; i < SFX_COUNT; i++) UnloadSound(sfx[i]);

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