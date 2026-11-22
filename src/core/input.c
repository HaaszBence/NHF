/*
input.c

Purpose: Implementation of hardware input polling and action mapping.
Last Updated: 2026-11-19
*/

// Includes
// -------------------------------------------------
#include "core/input.h"

// Public Functions
// -------------------------------------------------
void input_init(InputState *input) {
    if (!input) return;

    input->cursor_pos = (Vector2){ 0.0f, 0.0f };
    input->primary_click = false;
    input->primary_held = false;
    input->secondary_click = false;
    input->pause_toggle = false;
}

void input_update(InputState *input) {
    if (!input) return;

    input->cursor_pos = GetMousePosition();
    input->primary_click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    input->primary_held = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    input->secondary_click = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
    input->pause_toggle = IsKeyPressed(KEY_ESCAPE);
}