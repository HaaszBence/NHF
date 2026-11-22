/*
game.c

Purpose: Implementation of the game logic.
Last Updated: 2026-11-21
*/

// Includes
// --------------------------------------------------------------------------------------------------
#include "logic/game.h" // Own header containing GameState, Clickable, and function declarations
#include "core/config.h" // Needed for default configuration values (DEF_SCREEN_WIDTH, DEF_SCREEN_HEIGHT)
#include <stdlib.h> // Needed for malloc, realloc, free, and rand
#include "core/debugmalloc.h"

// Constants
// --------------------------------------------------------------------------------------------------
#define INITIAL_MAX_TARGETS 3

// Function Implementations
// --------------------------------------------------------------------------------------------------

// Helpers
// -------------------------------------------------

// Returns a random float in the range [min, max]
static float rand_range(float min, float max) {
    float scale = (float)rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}

// Counts how many target slots are currently active
static int count_active_targets(const GameState *state) {
    int count = 0;
    for (int i = 0; i < state->target_capacity; i++) {
        if (state->targets[i].active) {
            count++;
        }
    }
    return count;
}

// Finds the first inactive slot and spawns a new target inside safe screen margins
static void spawn_single_target(GameState *state) {
    if (!state || !state->targets) return; // Guard against null pointers
    if (count_active_targets(state) >= state->max_active_targets) return; // Don't spawn if at max active targets

    // Iterate through the targets array to find an inactive slot
    for (int i = 0; i < state->target_capacity; i++) {
        if (!state->targets[i].active) {

            // Define safe margins to prevent targets from spawning too close to the edges of the screen
            float radius = 40.0f;
            float margin_x = radius + 60.0f;
            float margin_top = radius + 120.0f;
            float margin_bottom = radius + 80.0f;

            // Calculate the valid range for spawning targets within the screen dimensions
            float min_x = margin_x;
            float max_x = (float)state->screen_w - margin_x;
            float min_y = margin_top;
            float max_y = (float)state->screen_h - margin_bottom;

            // Guard against tiny window dimensions
            if (max_x < min_x) max_x = min_x;
            if (max_y < min_y) max_y = min_y;

            // Initialize the target in the found inactive slot with random position and default properties
            state->targets[i] = (Clickable){
                .active = true,
                .x = rand_range(min_x, max_x),
                .y = rand_range(min_y, max_y),
                .radius = radius,
                .progress = 0.0f,
                .max_progress = 100.0f,
                .reward = 10.0
            };
            return;
        }
    }
}

// Public Functions
// -------------------------------------------------
void game_init(GameState *state, int screen_w, int screen_h) {
    if (!state) return; // Guard against null pointer

    // Initialize player stats and game state
    state->credits = 0.0;
    state->click_power = 35.0;
    state->passive_income = 0.0;

    // Set gameplay area dimensions, falling back to default values if invalid
    state->screen_w = (screen_w > 0) ? screen_w : DEF_SCREEN_WIDTH;
    state->screen_h = (screen_h > 0) ? screen_h : DEF_SCREEN_HEIGHT;

    state->spawn_timer = 0.0f; // Reset spawn timer to start counting for the first target spawn
    state->spawn_interval = 1.5f; // Spawn a new target every 1.5 seconds by default

    state->max_active_targets = INITIAL_MAX_TARGETS;
    state->target_capacity = INITIAL_MAX_TARGETS;
    state->targets = (Clickable *)malloc(sizeof(Clickable) * (size_t)state->target_capacity);

    // Initialize all target slots as inactive
    if (state->targets) { // Guard against malloc failure
        for (int i = 0; i < state->target_capacity; i++) {
            state->targets[i].active = false;
        }
        // Spawn one target immediately so the player does not wait on an empty screen
        spawn_single_target(state);
    } else {
        state->target_capacity = 0;
        state->max_active_targets = 0;
    }
}

void game_deinit(GameState *state) {
    if (!state) return; // Guard against null pointer

    // Free the dynamically allocated targets array and reset pointers and counters
    free(state->targets);
    state->targets = NULL;
    state->target_capacity = 0;
    state->max_active_targets = 0;
}

// Dynamically adjusts the maximum number of simultaneously active targets
bool game_set_max_targets(GameState *state, int new_max) {
    if (!state || new_max < 1) return false; // Guard against null pointer and invalid new_max values

    // Expand underlying heap array if the requested limit exceeds current capacity
    if (new_max > state->target_capacity) {
        Clickable *resized = (Clickable *)realloc(
            state->targets,
            sizeof(Clickable) * (size_t)new_max
        );
        if (!resized) return false; // Preserve existing pointer on allocation failure

        // Initialize newly added slots as inactive
        for (int i = state->target_capacity; i < new_max; i++) {
            resized[i].active = false;
        }

        state->targets = resized;
        state->target_capacity = new_max;
    }

    state->max_active_targets = new_max;
    return true;
}

// Evaluates a click at (click_x, click_y); returns true if an active target was hit
void game_update(GameState *state, float dt) {
    if (!state || dt <= 0.0f) return; // Guard against null pointer and non-positive delta time

    // Accumulate passive credits over time
    if (state->passive_income > 0.0) {
        state->credits += state->passive_income * (double)dt;
    }

    // Advance spawn timer and spawn a new target if below max_active_targets
    if (count_active_targets(state) < state->max_active_targets) {
        state->spawn_timer += dt;
        if (state->spawn_timer >= state->spawn_interval) {
            state->spawn_timer = 0.0f;
            spawn_single_target(state);
        }
    } else {
        // Keep timer reset while at capacity so spawns don't trigger instantaneously on clear
        state->spawn_timer = 0.0f;
    }
}

bool game_handle_click(GameState *state, float click_x, float click_y) {
    if (!state || !state->targets) return false; // Guard against null pointers

    // Iterate backwards so topmost rendered targets get priority on overlap
    for (int i = state->target_capacity - 1; i >= 0; i--) {
        Clickable *t = &state->targets[i];
        if (!t->active) continue;

        float dx = click_x - t->x;
        float dy = click_y - t->y;

        // Check circular hitbox using squared distance
        if ((dx * dx + dy * dy) <= (t->radius * t->radius)) {
            t->progress += (float)state->click_power;

            // Award credits and deactivate target once progress bar is filled
            if (t->progress >= t->max_progress) {
                state->credits += t->reward;
                t->active = false;
                t->progress = 0.0f;
            }
            return true;
        }
    }

    return false;
}