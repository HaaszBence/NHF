/*
ui.c

Purpose: Implementation of UI widgets and static HUD display components.
Last Updated: 2026-11-19
*/

// Includes
// --------------------------------------------------------------------------------------------------
#include "presentation/ui.h"
#include "core/assets.h" // Needed for assets_font() and FONT_* enum values

// Functions
// --------------------------------------------------------------------------------------------------

// Helpers
// -------------------------------------------------
static float clamp01(float v) {
    if (v < 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    return v;
}

// Draws text centered both horizontally and vertically inside a given Rectangle
static void draw_text_in_rect(Rectangle bounds, Font font, const char *text, float font_size, float spacing, Color color) {
    if (!text) return;

    Vector2 dim = MeasureTextEx(font, text, font_size, spacing);
    Vector2 pos = {
        bounds.x + (bounds.width - dim.x) * 0.5f,
        bounds.y + (bounds.height - dim.y) * 0.5f
    };
    DrawTextEx(font, text, pos, font_size, spacing, color);
}

// Public Functions
// -------------------------------------------------

bool ui_button_update(Button *btn, const InputState *input) {
    if (!btn || !input || !btn->enabled) {
        if (btn) {
            btn->hovered = false;
            btn->pressed = false;
        }
        return false;
    }

    btn->hovered = CheckCollisionPointRec(input->cursor_pos, btn->bounds);
    btn->pressed = btn->hovered && input->primary_click;
    return btn->pressed;
}

void ui_button_draw(const Button *btn) {
    if (!btn) return;

    switch (btn->type) {
        case BTN_TYPE_ROUNDED: {
            Color tint = !btn->enabled ? GRAY : (btn->hovered ? (Color){ 255, 255, 210, 255 } : WHITE);
            if (btn->tex.id != 0) {
                Rectangle src = { 0.0f, 0.0f, (float)btn->tex.width, (float)btn->tex.height };
                DrawTexturePro(btn->tex, src, btn->bounds, (Vector2){ 0.0f, 0.0f }, 0.0f, tint);
            } else {
                DrawRectangleRec(btn->bounds, !btn->enabled ? DARKGRAY : BROWN);
            }
            draw_text_in_rect(btn->bounds, assets_font(FONT_REGULAR), btn->text, 20.0f, 2.0f, WHITE);
            break;
        }
        case BTN_TYPE_DEFAULT:
        default: {
            Color bg = !btn->enabled ? DARKGRAY : (btn->hovered ? SKYBLUE : BLUE);
            DrawRectangleRec(btn->bounds, bg);
            DrawRectangleLinesEx(btn->bounds, 2.0f, WHITE);
            draw_text_in_rect(btn->bounds, assets_font(FONT_REGULAR), btn->text, 20.0f, 2.0f, WHITE);
            break;
        }
    }
}

bool ui_slider_update(Slider *slider, const InputState *input) {
    if (!slider || !input) return false;

    bool hovered = CheckCollisionPointRec(input->cursor_pos, slider->bounds);

    if (hovered && input->primary_click) {
        slider->dragging = true;
    }
    if (!input->primary_held) {
        slider->dragging = false;
    }

    if (slider->dragging && slider->bounds.width > 0.0f) {
        float prev = slider->value;
        float ratio = (input->cursor_pos.x - slider->bounds.x) / slider->bounds.width;
        slider->value = clamp01(ratio);
        return slider->value != prev;
    }

    return false;
}

void ui_slider_draw(const Slider *slider) {
    if (!slider) return;

    float val = clamp01(slider->value);
    float knob_x = slider->bounds.x + slider->bounds.width * val;

    if (slider->label) {
        Vector2 label_pos = { slider->bounds.x, slider->bounds.y - 24.0f };
        DrawTextEx(assets_font(FONT_REGULAR), slider->label, label_pos, 18.0f, 2.0f, WHITE);
    }

    switch (slider->type) {
        case SLIDER_TYPE_ROUNDED: {
            if (slider->tex.id != 0) {
                Rectangle src = { 0.0f, 0.0f, (float)slider->tex.width, (float)slider->tex.height };
                DrawTexturePro(slider->tex, src, slider->bounds, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
            } else {
                DrawRectangleRec(slider->bounds, DARKBROWN);
            }
            DrawRectangle((int)slider->bounds.x, (int)slider->bounds.y,
                          (int)(slider->bounds.width * val), (int)slider->bounds.height, GOLD);
            DrawRectangle((int)knob_x - 8, (int)slider->bounds.y - 4, 16, (int)slider->bounds.height + 8, BEIGE);
            break;
        }

        case SLIDER_TYPE_DEFAULT:
        default: {
            DrawRectangleRec(slider->bounds, DARKGRAY);
            DrawRectangle((int)slider->bounds.x, (int)slider->bounds.y,
                          (int)(slider->bounds.width * val), (int)slider->bounds.height, SKYBLUE);
            DrawRectangleLinesEx(slider->bounds, 2.0f, WHITE);
            DrawRectangle((int)knob_x - 6, (int)slider->bounds.y - 4, 12, (int)slider->bounds.height + 8, WHITE);
            break;
        }
    }
}

void ui_panel_draw(Rectangle bounds, PanelType type, Texture2D tex, Color bg_color, Color border_color) {
    (void)tex; // Currently unused; reserved for future texture-based panel rendering
    switch (type) {
        case PANEL_TYPE_ROUNDED: {
            DrawRectangleRounded(bounds, 0.5f, 8, bg_color);
            DrawRectangleRoundedLinesEx(bounds, 0.5f, 8, 2.0f, border_color);
            break;
        }

        case PANEL_TYPE_DEFAULT:
        default: {
            DrawRectangleRec(bounds, bg_color);
            DrawRectangleLinesEx(bounds, 2.0f, border_color);
            break;
        }
    }
}

void ui_progress_bar_draw(Rectangle bounds, ProgressBarType type, float progress, Color fill_color, Color bg_color) {
    float p = clamp01(progress);
    Rectangle fill_rect = { bounds.x, bounds.y, bounds.width * p, bounds.height };

    switch (type) {
        case BAR_TYPE_ROUNDED: {
            DrawRectangleRounded(bounds, 0.5f, 8, bg_color);
            if (p > 0.0f) {
                DrawRectangleRounded(fill_rect, 0.5f, 8, fill_color);
            }
            DrawRectangleRoundedLinesEx(bounds, 0.5f, 8, 2.0f, WHITE);
            break;
        }

        case BAR_TYPE_DEFAULT:
        default: {
            DrawRectangleRec(bounds, bg_color);
            DrawRectangleRec(fill_rect, fill_color);
            DrawRectangleLinesEx(bounds, 2.0f, WHITE);
            break;
        }
    }
}

void ui_draw_text_centered(Font font, const char *text, float y, float font_size, float spacing, Color color, int screen_w) {
    if (!text) return;

    Vector2 dim = MeasureTextEx(font, text, font_size, spacing);
    Vector2 pos = { ((float)screen_w - dim.x) * 0.5f, y };
    DrawTextEx(font, text, pos, font_size, spacing, color);
}