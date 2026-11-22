#ifndef CLICK_H // Include guard to prevent multiple inclusions of this header file
#define CLICK_H

#include "core/scene.h" // Needed for SceneID return type
#include "core/input.h" // Needed for InputState pointer parameter
#include "core/config.h" // Needed for Config pointer parameter
#include "logic/game.h" // Needed for GameState pointer parameter

// Initializes background layers and HUD button layout for the clicker scene
void click_init(const Config *cfg);

// Updates the game simulation, target clicks, and HUD navigation buttons
SceneID click_update(GameState *state, const InputState *input, float dt);

// Renders the background, active clickable targets with progress bars, and top HUD
void click_draw(const GameState *state);

#endif // CLICK_H