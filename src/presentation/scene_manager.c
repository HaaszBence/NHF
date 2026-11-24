#include "presentation/scene_manager.h"
#include "core/scene.h" // Needed for SceneID enum values
#include "core/audio.h" // Needed for audio playback control during scene transitions
#include "presentation/scenes/menu.h" // Needed to initialize, update, and draw the menu scene
#include "logic/game.h" // Needed for GameState, game_init, and game_deinit
#include "presentation/scenes/click.h" // Needed for click gameplay scene functions
#include "raylib.h" // Needed for temporary placeholder DrawText

// Encapsulated state tracking the currently active screen
static SceneID current_scene = SCENE_MENU;
static GameState game_state;

// Function Implementations
// --------------------------------------------------------------------------------------------------

// Helpers
// -------------------------------------------------
static void apply_scene_audio(SceneID scene) {
    switch (scene) {
        case SCENE_MENU:
            audio_play_music(MUSIC_CAT_CHILL);
            audio_start_ambient();
            break;
        case SCENE_CLICK:
            audio_play_music(MUSIC_CAT_CHILL);
            audio_start_ambient();
            break;
        case SCENE_SHOP:
            // audio_play_music(MUSIC_CAT_GOOFY);
            audio_stop_ambient();
            break;
        case SCENE_UPTREE:
            // audio_play_music(MUSIC_CAT_GOOFY);
            audio_stop_ambient();
            break;
        default:
            break;
    }
}

// Public Functions
// -------------------------------------------------

// Initializes all scenes and sets the initial active scene
void scene_manager_init(const Config *cfg) {
    // Initialize the game state with default values and gameplay area dimensions
    game_init(&game_state, cfg->screen_width, cfg->screen_height);

    // Initialize all scenes once at startup
    menu_init(cfg);
    click_init(cfg);

    // Start the initial scene's music and ambient sound
    apply_scene_audio(current_scene);
}

void scene_manager_update(const InputState *input, float dt) {
    (void)dt; // Suppress unused parameter warning until gameplay scenes use delta time

    SceneID next_scene = current_scene; // Defaults to no scene change unless a scene requests a transition

    // Delegate per-frame update logic to the currently active scene
    switch (current_scene) {
        case SCENE_MENU:
            next_scene = menu_update(input);
            break;
        case SCENE_CLICK:
            next_scene = click_update(&game_state, input, dt);
            break;
        case SCENE_UPTREE:
        case SCENE_SHOP:
            // Temporary placeholder until uptree.c and shop.c are implemented
            if (input && input->pause_toggle) {
                next_scene = SCENE_CLICK;
            }
            break;
        default:
            break;
    }

    // Handle transition side effects (e.g., switching background music tracks) when scene changes
    if (next_scene != current_scene) {
        current_scene = next_scene;
        apply_scene_audio(current_scene);
    }
}

void scene_manager_deinit(void) {
    game_deinit(&game_state);
}

void scene_manager_draw(void) {
    // Delegate rendering to the currently active scene
    switch (current_scene) {
        case SCENE_MENU:
            menu_draw();
            break;
        case SCENE_CLICK:
            click_draw(&game_state);
            break;
        case SCENE_UPTREE:
            DrawText("COMING SOON (Press ESC to return)", 40, 40, 28, WHITE);
            break;
        case SCENE_SHOP:
            DrawText("COMING SOON (Press ESC to return)", 40, 40, 28, WHITE);
            break;
        default:
            break;
    }
}

bool scene_manager_should_exit(void) {
    return current_scene == SCENE_EXIT;
}