/*
menu.h

Purpose: Menu scene for the game.
Last Updated: 2026-11-18
*/
#ifndef MENU_H // Include guard to prevent multiple inclusions of this header file
#define MENU_H

#include "core/scene.h" // Needed for the SceneID return type of menu_update
#include "core/input.h" // Needed for the InputState pointer parameter
#include "core/config.h" // Needed for the Config pointer parameter

// Sets up button positions and stores the background texture once at startup
void menu_init(const Config *cfg);

// Handles button hover/click logic per frame and returns the next active scene
SceneID menu_update(const InputState *input);

// Renders the background, title text, and menu buttons
void menu_draw(void);

#endif // MENU_H