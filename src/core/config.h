/*
config.h

Purpose: Config management for the game engine.
High level overview:
    - Defines default configuration values and the Config structure.
    - Provides functions to initialize, retrieve defaults, and save configuration.
Last Updated: 2026-11-18
*/

#ifndef CONFIG_H // Include guard to prevent multiple inclusions of this header file
#define CONFIG_H

// Includes
// -------------------------------------------------
#include <stdbool.h>

// Constants and Macros
// -------------------------------------------------
#define CONFIG_PATH "../config.cfg" // Path to the configuration file
#define ASSETS_PATH "../assets/" // Path to the assets directory

// Default configuration values
// -------------------------------------------------
#define DEF_SCREEN_WIDTH 1280 // Default width of the game window
#define DEF_SCREEN_HEIGHT 720 // Default height of the game window

#define DEF_FPS 60 // Target frames per second for the game loop

#define DEF_TITLE "Nature's Harmony Forest" // Default title of the game window
#define DEF_FULLSCREEN false // Default fullscreen mode
#define DEF_MASTER_VOLUME 1.0f // Default master volume

// Structure definition
// -------------------------------------------------
typedef struct {
    int screen_width;
    int screen_height;
    int target_fps;
    char title[64];
    bool fullscreen;
    float master_volume;
} Config;

// Function Declarations
// -------------------------------------------------

// Tries to load the configuration from a file.
const Config *config_init(void);

// Returns a default configuration.
const Config *config_get_defaults(void);

// Saves the configuration to the configuration file.
bool config_save(const Config *c);

#endif