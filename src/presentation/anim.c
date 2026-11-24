/*
anim.c

Purpose: Stateless, composable visual animation modifiers for SpriteView elements.
Last Updated: 2026-11-22
*/

#include "presentation/anim.h"
#include "raylib.h"
#include <math.h>

void anim_drift(SpriteView *v, float speed, DriftDir dir, bool loop) {
    if (!v) return;

    float shift = (float)(GetTime() * (double)speed);

    if (loop) {
        if ((dir == DRIFT_LEFT || dir == DRIFT_RIGHT) && v->dest.width > 0.0f) {
            shift = fmodf(shift, v->dest.width);
        } else if ((dir == DRIFT_UP || dir == DRIFT_DOWN) && v->dest.height > 0.0f) {
            shift = fmodf(shift, v->dest.height);
        }
    }

    switch (dir) {
        case DRIFT_LEFT:  v->offset.x -= shift; break;
        case DRIFT_RIGHT: v->offset.x += shift; break;
        case DRIFT_UP:    v->offset.y -= shift; break;
        case DRIFT_DOWN:  v->offset.y += shift; break;
    }
}

void anim_bobbing(SpriteView *v, float amplitude, float freq) {
    if (!v) return;
    v->offset.y += sinf((float)GetTime() * freq * 2.0f * PI) * amplitude;
}

void anim_pulse(SpriteView *v, float amount, float freq) {
    if (!v) return;
    v->scale += sinf((float)GetTime() * freq * 2.0f * PI) * amount;
}

void anim_fade(SpriteView *v, float alpha) {
    if (!v) return;
    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;
    v->tint.a = (unsigned char)(alpha * 255.0f);
}

void anim_frames(SpriteView *v, const Texture2D *frames, int frame_count, float fps) {
    if (!v || !frames || frame_count <= 0 || fps <= 0.0f) return;
    int idx = (int)(GetTime() * (double)fps) % frame_count;
    v->tex = frames[idx];
}

bool anim_frames_once(SpriteView *v, const Texture2D *frames, int frame_count, float fps, double start_time) {
    if (!v || !frames || frame_count <= 0 || fps <= 0.0f) return false;
    double elapsed = GetTime() - start_time;
    if (elapsed < 0.0) elapsed = 0.0;

    int idx = (int)(elapsed * (double)fps);
    if (idx >= frame_count) {
        v->tex = frames[frame_count - 1];
        return false;
    }

    v->tex = frames[idx];
    return true;
}