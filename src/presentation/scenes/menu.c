#include "presentation/scenes/menu.h" // Own header with declarations and types
#include "presentation/render.h" // Needed for SpriteView and sprite_view_draw
#include "presentation/ui.h" // Needed for Button struct, ui_button_update, and ui_button_draw
#include "core/assets.h" // Needed for assets_tex() and TextureId enum values
#include "raylib.h" // Needed for MeasureText, DrawText, and WHITE

#define MENU_BG_LAYER_COUNT 5

static SpriteView sprite_bg[MENU_BG_LAYER_COUNT]; // Background sprite
static int screen_w;
static int screen_h;

static Button btn_play;
static Button btn_exit;

void menu_init(const Config *cfg) {

    // Use configuration values for dinamic positioning
    if (cfg) {
        screen_w = cfg->screen_width;
        screen_h = cfg->screen_height;
    }

    // Create a full-screen rectangle for background rendering
    Rectangle full_screen = { 0.0f, 0.0f, (float)screen_w, (float)screen_h };

    // Order background layers from back (sky) to front (ground)
    TextureId layer_ids[MENU_BG_LAYER_COUNT] = {
        TEX_BG_SKY,
        TEX_BG_CLOUDS_FAR,
        TEX_BG_CLOUDS_CLOSE,
        TEX_BG_ROCKS,
        TEX_BG_GROUND
    };

    // Create SpriteView objects for each background layer
    for (int i = 0; i < MENU_BG_LAYER_COUNT; i++) {
        sprite_bg[i] = sprite_view_create(assets_tex(layer_ids[i]), full_screen);
    }

    float play_btn_w = 240.0f;
    float play_btn_h = 60.0f;
    float play_btn_x = (screen_w - play_btn_w) * 0.5f; // Center buttons horizontally
    float play_btn_y = screen_h * 0.45f; // Place first button slightly above vertical center

    Rectangle play_btn_bounds = { play_btn_x, play_btn_y, play_btn_w, play_btn_h }; // Play button bounds

    // Initialize the Play button with default styling (falls back to solid shape if tex.id == 0)
    btn_play = (Button){
        .bounds = play_btn_bounds,
        .type = BTN_TYPE_DEFAULT,
        .tex = { 0 },
        .text = "PLAY",
        .enabled = true,
        .hovered = false,
        .pressed = false
    };

    float exit_btn_w = 240.0f;
    float exit_btn_h = 60.0f;
    float exit_btn_x = (screen_w - exit_btn_w) * 0.5f; // Center buttons horizontally
    float exit_btn_y = screen_h * 0.45f; // Place first button slightly above vertical center

    Rectangle exit_btn_bounds = { exit_btn_x, exit_btn_y + 80.0f, exit_btn_w, exit_btn_h }; // Exit button bounds

    // Initialize the Exit button below the Play button
    btn_exit = (Button){
        .bounds = exit_btn_bounds,
        .type = BTN_TYPE_DEFAULT,
        .tex = { 0 },
        .text = "EXIT",
        .enabled = true,
        .hovered = false,
        .pressed = false
    };
}

SceneID menu_update(const InputState *input) {
    if (!input) return SCENE_MENU; // Guard against null pointer

    // Switch to the main clicker scene if the Play button was clicked
    if (ui_button_update(&btn_play, input)) {
        return SCENE_CLICK;
    }

    // Signal the main loop to close the application if the Exit button was clicked
    if (ui_button_update(&btn_exit, input)) {
        return SCENE_EXIT;
    }

    return SCENE_MENU; // Stay on the menu screen if no transition occurred
}

void menu_draw(void) {
    // Render all background layers back-to-front via render.c
    for (int i = 0; i < MENU_BG_LAYER_COUNT; i++) {
        if (sprite_bg[i].tex.id != 0) {
            sprite_view_draw(&sprite_bg[i]);
        }
    }

    // Measure text dimensions using the custom font and spacing to center horizontally
    const char *title = "Nature's Harmony Forest";
    Font title_font = assets_font(FONT_BOLD);
    float title_size = 32.0f;
    float title_spacing = 2.0f;
    Vector2 text_dim = MeasureTextEx(title_font, title, title_size, title_spacing);
    Vector2 title_pos = { ((float)screen_w - text_dim.x) * 0.5f, (float)screen_h * 0.22f };
    DrawTextEx(title_font, title, title_pos, title_size, title_spacing, WHITE);

    // Render interactive UI buttons
    ui_button_draw(&btn_play);
    ui_button_draw(&btn_exit);
}