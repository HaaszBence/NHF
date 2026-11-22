/*
save.h

Purpose: Persistent save and load serialization for game progress.
High level overview:
    - Defines the default save file path.
    - Provides functions to write GameState to disk, restore saved progress, and check save existence.
Last Updated: 2026-11-19
*/

#ifndef SAVE_H // Include guard to prevent multiple inclusions of this header file
#define SAVE_H

// Includes
// -------------------------------------------------
#include "logic/game.h"
#include <stdbool.h>

// Constants and Macros
// -------------------------------------------------
#define SAVE_FILE_PATH "../../data/savegame.dat" // Path to the binary save file

// Function Declarations
// -------------------------------------------------

// Returns true if a valid save file exists on disk.
bool save_exists(void);

// Serializes the current GameState to SAVE_FILE_PATH; returns true on success.
bool save_game(const GameState *state);

// Loads saved progress into GameState from SAVE_FILE_PATH; returns true on success.
bool save_load(GameState *state);

// Deletes the existing save file from disk.
bool save_delete(void);

#endif // SAVE_H