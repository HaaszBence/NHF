#include "core/audio.h"
#include "core/config.h" // ASSETS_PATH
#include "raylib.h" // Needed for AudioStream, Music, and Sound types
#include <stddef.h> // Needed for NULL

static MusicCategory current_cat = MUSIC_CAT_COUNT;
static Music *current_music = NULL;
static int current_track_idx = 0;

static Music ambient_music = { 0 };
static bool ambient_playing = false;

static float master_volume  = 1.0f;
static float music_volume   = 0.6f;
static float sfx_volume     = 0.8f;
static float ambient_volume = 0.15f;

static bool initialized = false;

static float clamp01(float v) {
    if (v < 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    return v;
}

void audio_init(void) {
    if (initialized) return;
    InitAudioDevice();
    ambient_music = LoadMusicStream(ASSETS_PATH "sounds/sfx/nature.mp3");
    ambient_music.looping = true;
    initialized = true;
    audio_set_master_volume(master_volume);
    audio_set_sfx_volume(sfx_volume);
    audio_set_ambient_volume(ambient_volume);
}

void audio_update(void) {
    if (!initialized) return;

    if (current_music && current_music->ctxData != NULL) {
        UpdateMusicStream(*current_music);

        // Play next track if the current one has finished
        if (GetMusicTimePlayed(*current_music) >= GetMusicTimeLength(*current_music) - 0.1f) {
            int count = 0;
            Music *list = assets_music_list(current_cat, &count);
            if (list && count > 0) {
                StopMusicStream(*current_music);
                current_track_idx = (current_track_idx + 1) % count;
                current_music = &list[current_track_idx];
                current_music->looping = false;
                SetMusicVolume(*current_music, music_volume);
                PlayMusicStream(*current_music);
            }
        }
    }

    if (ambient_playing && ambient_music.ctxData != NULL) {
        UpdateMusicStream(ambient_music);
    }
}

void audio_deinit(void) {
    if (!initialized) return;
    if (ambient_music.ctxData != NULL) UnloadMusicStream(ambient_music);
    current_music = NULL;
    CloseAudioDevice();
    initialized = false;
}

void audio_play_music(MusicCategory category) {
    if (!initialized || category < 0 || category >= MUSIC_CAT_COUNT) return;
    if (current_cat == category && current_music != NULL) return;

    if (current_music && current_music->ctxData != NULL) {
        StopMusicStream(*current_music);
    }

    int count = 0;
    Music *list = assets_music_list(category, &count);
    current_cat = category;
    current_track_idx = 0;

    if (list && count > 0) {
        current_music = &list[0];
        current_music->looping = false;
        SetMusicVolume(*current_music, music_volume);
        PlayMusicStream(*current_music);
    } else {
        current_music = NULL;
    }
}

void audio_start_ambient(void) {
    if (!initialized || ambient_music.ctxData == NULL) return;
    if (!ambient_playing) {
        SetMusicVolume(ambient_music, ambient_volume);
        PlayMusicStream(ambient_music);
        ambient_playing = true;
    }
}

void audio_stop_ambient(void) {
    if (!initialized || ambient_music.ctxData == NULL) return;
    if (ambient_playing) {
        StopMusicStream(ambient_music);
        ambient_playing = false;
    }
}

void audio_set_music_volume(float volume) {
    music_volume = clamp01(volume);
    if (initialized && current_music && current_music->ctxData != NULL) {
        SetMusicVolume(*current_music, music_volume);
    }
}

void audio_set_sfx_volume(float volume) {
    sfx_volume = clamp01(volume);
    if (!initialized) return;
    for (int i = 0; i < SFX_COUNT; i++) {
        if (i == SFX_NATURE) continue;
        Sound s = assets_sfx((SfxId)i);
        if (s.frameCount > 0) SetSoundVolume(s, sfx_volume);
    }
}

void audio_set_ambient_volume(float volume) {
    ambient_volume = clamp01(volume);
    if (initialized && ambient_music.ctxData != NULL) {
        SetMusicVolume(ambient_music, ambient_volume);
    }
}

void audio_set_master_volume(float volume) {
    master_volume = clamp01(volume);
    if (initialized) SetMasterVolume(master_volume);
}

float audio_get_music_volume(void)   { return music_volume; }
float audio_get_sfx_volume(void)     { return sfx_volume; }
float audio_get_ambient_volume(void) { return ambient_volume; }
float audio_get_master_volume(void)  { return master_volume; }

void audio_play_sfx(SfxId id) {
    if (!initialized || id < 0 || id >= SFX_COUNT) return;
    Sound s = assets_sfx(id);
    if (s.frameCount > 0) PlaySound(s);
}