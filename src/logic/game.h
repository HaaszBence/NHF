/*
game.h

Purpose: Pure game state and simulation rules for Nature's Harmony Forest.
High level overview:
    - Defines the GameState structure without any rendering dependencies.
    - Provides core lifecycle functions (init, update) to be expanded with gameplay mechanics.
Last Updated: 2026-11-19
*/

#ifndef GAME_H // Include guard to prevent multiple inclusions of this header file
#define GAME_H

// Structure definitions
// -------------------------------------------------
typedef struct {
    int placeholder; // Placeholder field until gameplay mechanics are defined
} GameState;

// Function Declarations
// -------------------------------------------------

// Initializes a fresh GameState with default starting values.
void game_init(GameState *state);

// Advances the pure game simulation by delta time (dt in seconds).
void game_update(GameState *state, float dt);

#endif // GAME_H