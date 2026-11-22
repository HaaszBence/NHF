/*
scene.h

Purpose: Defines the different scenes in the game.
High level overview:
    - Enumerates the possible scenes that can be active in the game.
    - Provides a type alias for scene identifiers.
Last Updated: 2026-11-21
*/


#ifndef SCENE_H // Include guard to prevent multiple inclusions of this header file
#define SCENE_H

// Central list of all game screens/states so main.c and scenes can communicate cleanly
typedef enum {
    SCENE_MENU, // Main title screen
    SCENE_CLICK, // Core clicker gameplay screen
    SCENE_UPTREE, // Upgrade tree screen
    SCENE_SHOP, // Shop screen
    SCENE_PAUSE, // Pause overlay/screen
    SCENE_EXIT // Signals the main loop in main.c to terminate the application
} SceneID;

#endif