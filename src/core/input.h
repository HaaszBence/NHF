/*
input.h

Purpose: Hardware input polling and abstraction into game-agnostic action states.
High level overview:
    - Defines the InputState structure capturing cursor coordinates and abstracted button actions.
    - Decouples raw Raylib input queries from UI widgets and presentation scenes.
Last Updated: 2026-11-19
*/

#ifndef INPUT_H // Include guard to prevent multiple inclusions of this header file
#define INPUT_H

// Includes
// -------------------------------------------------
#include "raylib.h"
#include <stdbool.h>

// Structure definitions
// -------------------------------------------------
typedef struct {
    Vector2 cursor_pos; // Current screen position of the cursor
    bool primary_click; // True on the frame the primary button is pressed
    bool primary_held; // True while the primary button is held down
    bool secondary_click; // True on the frame the secondary button is pressed
    bool pause_toggle; // True on the frame the pause key (ESC) is pressed
} InputState;

// Function Declarations
// -------------------------------------------------

// Resets all fields of the InputState structure to safe default values.
void input_init(InputState *input);

// Polls hardware devices once per frame and populates the InputState structure.
void input_update(InputState *input);

#endif // INPUT_H