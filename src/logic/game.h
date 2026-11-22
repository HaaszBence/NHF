#ifndef GAME_H // Include guard to prevent multiple inclusions of this header file
#define GAME_H

// Includes
// --------------------------------------------------------------------------------------------------
#include <stdbool.h> // Include standard boolean type for true/false values

// Structure definitions
// --------------------------------------------------------------------------------------------------

// A clickable target on the screen that the player can interact with
typedef struct {
    bool active; // True if the target is currently active and can be clicked
    float x, y; // Screen position of the target's center
    float radius; // Clickable area radius (in pixels) for detecting clicks
    float progress; // Current progress towards completion (0.0f to max_progress)
    float max_progress; // Maximum progress value at which the target is considered "completed"
    double reward; // Reward given to the player when the target is completed
} Clickable;

// Stores the current state of the game, including player stats and active targets
typedef struct {
    double credits; // Player's current credits (currency)
    double click_power; // Amount of progress gained per click on a target
    double passive_income; // Amount of credits gained passively per second
    
    // [DYNAMIC]
    Clickable *targets; // Dynamic array of active clickable targets on the screen
    int max_active_targets; // Current dynamic limit of simultaneously spawned targets
    int target_capacity; // Total allocated capacity of the targets array

    float spawn_timer; // Timer for spawning new targets
    float spawn_interval; // Interval (in seconds) between spawning new targets
    
    int screen_w; // Width of gameplay area
    int screen_h; // Height of gameplay area
} GameState;

// Function Declarations
// --------------------------------------------------------------------------------------------------

// Initializes the game state with default values and sets up the gameplay area dimensions
void game_init(GameState *state, int screen_w, int screen_h);

// Updates the game state based on elapsed time (dt) and handles passive income, target spawning, and progress updates
void game_update(GameState *state, float dt);

// Cleans up any resources used by the game state
void game_deinit(GameState *state);

// Dynamically adjusts the maximum number of simultaneously active targets (reallocates if needed)
bool game_set_max_targets(GameState *state, int new_max);

// Evaluates a click at (click_x, click_y); returns true if an active target was hit
bool game_handle_click(GameState *state, float click_x, float click_y);

#endif // GAME_H