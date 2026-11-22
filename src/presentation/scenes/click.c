#include "presentation/scenes/click.h" // Own header with declarations
#include "presentation/render.h" // Needed for SpriteView and sprite_view_draw
#include "presentation/ui.h" // Needed for Button, ui_button_update/draw, ui_panel_draw, ui_progress_bar_draw
#include "core/assets.h" // Needed for assets_tex() and TextureId enum values
#include "raylib.h" // Needed for DrawCircle, DrawText, TextFormat, and colors

#define CLICK_BG_LAYER_COUNT 5

// File-scoped (static) presentation state for the click scene
static SpriteView bg_layers[CLICK_BG_LAYER_COUNT];
static int screen_w;
static int screen_h;

static Button btn_uptree;
static Button btn_shop;
static Button btn_menu;

void click_init(const Config *cfg) {
    if (cfg) {
        screen_w = cfg->screen_width;
        screen_h = cfg->screen_height;
    }

    // Create a full-screen rectangle for background rendering
    Rectangle full_screen = { 0.0f, 0.0f, (float)screen_w, (float)screen_h };

    // Order background layers from back (sky) to front (ground)
    TextureId layer_ids[CLICK_BG_LAYER_COUNT] = {
        TEX_BG_SKY,
        TEX_BG_CLOUDS_FAR,
        TEX_BG_CLOUDS_CLOSE,
        TEX_BG_ROCKS,
        TEX_BG_GROUND
    };

    // Create SpriteView objects for each background layer
    for (int i = 0; i < CLICK_BG_LAYER_COUNT; i++) {
        bg_layers[i] = sprite_view_create(assets_tex(layer_ids[i]), full_screen);
    }

    // Position top-right navigation buttons inside the HUD bar
    float btn_w = 175.0f;
    float btn_h = 44.0f;
    float btn_y = 18.0f;
    float margin = 20.0f;

    btn_menu = (Button){
        .bounds = { (float)screen_w - margin - btn_w, btn_y, btn_w, btn_h },
        .type = BTN_TYPE_DEFAULT,
        .text = "MENU",
        .tex = { 0 },
        .enabled = true,
        .hovered = false,
        .pressed = false
    };

    btn_shop = (Button){
        .bounds = { btn_menu.bounds.x - 15.0f - btn_w, btn_y, btn_w, btn_h },
        .type = BTN_TYPE_DEFAULT,
        .text = "SHOP",
        .tex = { 0 },
        .enabled = true,
        .hovered = false,
        .pressed = false
    };

    btn_uptree = (Button){
        .bounds = { btn_shop.bounds.x - 15.0f - btn_w, btn_y, btn_w, btn_h },
        .type = BTN_TYPE_DEFAULT,
        .text = "UPGRADES",
        .tex = { 0 },
        .enabled = true,
        .hovered = false,
        .pressed = false
    };
}

SceneID click_update(GameState *state, const InputState *input, float dt) {
    if (!state || !input) return SCENE_CLICK; // Guard against null pointers

    // Advance passive income and target spawning
    game_update(state, dt);

    // Return to pause menu on ESC key
    if (input->pause_toggle) {
        return SCENE_PAUSE; // Not yet implemented, but reserved for future pause overlay
    }

    // Evaluate navigation buttons first so clicking UI does not click through to targets
    if (ui_button_update(&btn_menu, input)) {
        return SCENE_MENU;
    }
    if (ui_button_update(&btn_shop, input)) {
        return SCENE_SHOP;
    }
    if (ui_button_update(&btn_uptree, input)) {
        return SCENE_UPTREE;
    }

    // Check if primary click hit any active target
    if (input->primary_click && input->cursor_pos.y > 80.0f) { // Ignore clicks on the top HUD bar
        game_handle_click(state, input->cursor_pos.x, input->cursor_pos.y);
    }

    return SCENE_CLICK;
}

void click_draw(const GameState *state) {
    // Render background layers back-to-front
    for (int i = 0; i < CLICK_BG_LAYER_COUNT; i++) {
        if (bg_layers[i].tex.id != 0) {
            sprite_view_draw(&bg_layers[i]);
        }
    }

    // Draw all active clickable targets and their overhead progress bars
    if (state && state->targets) {
        for (int i = 0; i < state->target_capacity; i++) {
            const Clickable *t = &state->targets[i];
            if (!t->active) continue;

            // Draw target placeholder circle (can be replaced with SpriteView later)
            DrawCircle((int)t->x, (int)t->y, t->radius, DARKGREEN);
            DrawCircleLines((int)t->x, (int)t->y, t->radius, LIME);

            // Calculate normalized progress ratio [0.0f, 1.0f]
            float ratio = (t->max_progress > 0.0f) ? (t->progress / t->max_progress) : 0.0f;

            // Draw progress bar centered directly above the target
            float bar_w = t->radius * 2.2f;
            float bar_h = 12.0f;
            Rectangle bar_bounds = {
                t->x - bar_w * 0.5f,
                t->y - t->radius - 20.0f,
                bar_w,
                bar_h
            };

            ui_progress_bar_draw(bar_bounds, BAR_TYPE_DEFAULT, ratio, LIME, DARKGRAY);
        }
    }

    // Draw top HUD panel
    Rectangle hud_rect = { 0.0f, 0.0f, (float)screen_w, 80.0f };
    ui_panel_draw(hud_rect, PANEL_TYPE_DEFAULT, (Texture2D){ 0 }, Fade(BLACK, 0.65f), WHITE);

    // Draw current credits and passive income stats
    if (state) {
        DrawText(TextFormat("Credits: %.0f", state->credits), 24, 16, 26, GOLD);
        DrawText(TextFormat("Income: %.1f / sec", state->passive_income), 24, 48, 18, LIGHTGRAY);
    }

    // Draw top HUD navigation buttons
    ui_button_draw(&btn_uptree);
    ui_button_draw(&btn_shop);
    ui_button_draw(&btn_menu);
}