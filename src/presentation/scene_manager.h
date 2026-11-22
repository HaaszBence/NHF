#ifndef SCENE_MANAGER_H // Prevent multiple inclusions of this header
#define SCENE_MANAGER_H

#include <stdbool.h> // Needed for bool return type
#include "core/config.h" // Needed for Config pointer parameter
#include "core/assets.h" // Needed for Assets pointer parameter
#include "core/input.h" // Needed for InputState pointer parameter

// Initializes all scenes and sets the initial active scene
void scene_manager_init(const Config *cfg);

// Updates the active scene and handles scene transitions
void scene_manager_update(const InputState *input, float dt);

// Frees dynamic resources owned by the game state and scenes
void scene_manager_deinit(void);

// Renders the currently active scene
void scene_manager_draw(void);

// Returns true if the user requested to exit the application
bool scene_manager_should_exit(void);

#endif // SCENE_MANAGER_H