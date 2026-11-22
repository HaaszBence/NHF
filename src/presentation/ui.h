/*
ui.h

Purpose: Reusable User Interface (UI) widgets and HUD display components.
High level overview:
    - Defines interactive widget structures (Button, Slider) and visual type enums for all UI elements.
    - Consumes abstracted InputState for pure update functions and renders via pure draw functions.
Last Updated: 2026-11-19
*/

#ifndef UI_H // Include guard to prevent multiple inclusions of this header file
#define UI_H

// Includes
// ----------------------------------------------------------------------------------------------
#include "core/input.h"
#include "raylib.h"
#include <stdbool.h>

// Enum definitions
// ----------------------------------------------------------------------------------------------
typedef enum {
    BTN_TYPE_DEFAULT = 0, // Standard rectangular text button
    BTN_TYPE_ROUNDED, // Standard rounded button
} ButtonType;

typedef enum {
    SLIDER_TYPE_DEFAULT = 0, // Standard rectangular track and knob slider
    SLIDER_TYPE_ROUNDED // Standard rounded track and knob slider
} SliderType;

typedef enum {
    PANEL_TYPE_DEFAULT = 0, // Solid colored rectangle with border
    PANEL_TYPE_ROUNDED // Standard rounded rectangle with border
} PanelType;

typedef enum {
    BAR_TYPE_DEFAULT = 0, // Standard flat progress bar
    BAR_TYPE_ROUNDED // Rounded-corner progress bar
} ProgressBarType;

// Structure definitions
// ----------------------------------------------------------------------------------------------
typedef struct {
    Rectangle bounds;       // Screen position and size of the button
    ButtonType type;        // Visual variant (defaults to BTN_TYPE_DEFAULT)
    const char *text;       // Label displayed inside the button
    Texture2D tex;          // Optional texture or icon for non-default types
    bool enabled;           // Whether the button accepts user interaction
    bool hovered;           // Updated by ui_button_update: true if cursor is over bounds
    bool pressed;           // Updated by ui_button_update: true if clicked in the current frame
} Button;

typedef struct {
    Rectangle bounds;       // Screen position and size of the slider track
    SliderType type;        // Visual variant (defaults to SLIDER_TYPE_DEFAULT)
    const char *label;      // Descriptive label above the slider
    Texture2D tex;          // Optional track/knob texture for non-default types
    float value;            // Normalized value between 0.0f and 1.0f
    bool dragging;          // Updated by ui_slider_update: true while held down by the user
} Slider;

// Function Declarations
// ----------------------------------------------------------------------------------------------

// Interactive Widgets (Update / Draw)
// -------------------------------------------------

// Evaluates InputState against button bounds and updates state; returns true on primary click.
bool ui_button_update(Button *btn, const InputState *input);

// Pure render: draws the button based on its type and current state flags.
void ui_button_draw(const Button *btn);

// Evaluates InputState against slider track and updates value; returns true if the value changed.
bool ui_slider_update(Slider *slider, const InputState *input);

// Pure render: draws the slider based on its type and current value.
void ui_slider_draw(const Slider *slider);

// Static Display Elements
// -------------------------------------------------

// Draws a background panel container according to the specified PanelType.
void ui_panel_draw(Rectangle bounds, PanelType type, Texture2D tex, Color bg_color, Color border_color);

// Draws a horizontal progress bar for a normalized ratio (0.0f to 1.0f) according to ProgressBarType.
void ui_progress_bar_draw(Rectangle bounds, ProgressBarType type, float progress, Color fill_color, Color bg_color);

// Draws text centered both horizontally and vertically inside a given Rectangle.
void ui_draw_text_in_rect(Rectangle bounds, Font font, const char *text, float font_size, float spacing, Color color);

#endif // UI_H