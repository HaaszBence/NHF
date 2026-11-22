#include "presentation/render.h" // Own header containing SpriteView and function declarations

SpriteView sprite_view_create(Texture2D tex, Rectangle dest) {
    return (SpriteView){
        .tex = tex,
        .dest = dest,
        .offset = (Vector2){ 0.0f, 0.0f },
        .scale = 1.0f,
        .rotation = 0.0f,
        .tint = WHITE,
        .wrap_x = false
    };
}

void sprite_view_draw(const SpriteView *v) {
    if (!v || v->tex.id == 0) return; // Skip drawing if the pointer is NULL or the texture is invalid

    // Source rectangle covering the entire texture
    Rectangle src = {
        0.0f,
        0.0f,
        (float)v->tex.width,
        (float)v->tex.height
    };

    // Apply positional offset and scale multiplier to base destination
    float scaled_w = v->dest.width * v->scale;
    float scaled_h = v->dest.height * v->scale;

    Rectangle dst = {
        v->dest.x + v->offset.x + (v->dest.width - scaled_w) * 0.5f,
        v->dest.y + v->offset.y + (v->dest.height - scaled_h) * 0.5f,
        scaled_w,
        scaled_h
    };

    if (v->wrap_x && dst.width > 0.0f) {
        // Normalize horizontal offset into [-dst.width, 0] for seamless looping
        float start_x = dst.x;
        while (start_x > 0.0f) start_x -= dst.width;
        while (start_x <= -dst.width) start_x += dst.width;

        // Draw enough tiles side-by-side to cover the screen width
        for (float x = start_x; x < (float)GetScreenWidth(); x += dst.width) {
            Rectangle tile_dst = { x, dst.y, dst.width, dst.height };
            DrawTexturePro(v->tex, src, tile_dst, (Vector2){ 0.0f, 0.0f }, v->rotation, v->tint);
        }
    } else {
        DrawTexturePro(v->tex, src, dst, (Vector2){ 0.0f, 0.0f }, v->rotation, v->tint);
    }
}