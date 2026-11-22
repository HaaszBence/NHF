#include "raylib.h" // Core windowing and main loop functions
#include "core/config.h" // Window resolution, FPS, and title configuration
#include "core/assets.h" // Central asset loading and unloading
#include "core/input.h" // Hardware input polling
#include "core/audio.h" // Audio device initialization and volume control
#include "presentation/scene_manager.h" // Central scene lifecycle, transitions, and rendering

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    // Ensure relative asset paths resolve properly regardless of launch directory
    ChangeDirectory(GetApplicationDirectory());

    const Config *cfg = config_init();
    if (cfg->fullscreen) {
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
    }

    InitWindow(cfg->screen_width, cfg->screen_height, cfg->title);
    SetExitKey(KEY_NULL); // Unset default exit key (ESC)
    SetTargetFPS(cfg->target_fps); // Set our game to run at coniguration's frames-per-second

    // Initialize the audio subsystem and apply initial volume settings
    audio_init();

    // Load all registered textures, fonts, sound effects, and music streams
    assets_load();

    // Initialize all scenes
    scene_manager_init(cfg);

    // Initialize input state before the main loop
    InputState input;
    input_init(&input);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose() && !scene_manager_should_exit()) // Detect window close button or scene exit request
    {
        // Update
        //----------------------------------------------------------------------------------

        float dt = GetFrameTime();
        input_update(&input); // Poll hardware devices and populate the InputState structure
        audio_update(); // Update active music stream buffers (must be called once per frame)
        scene_manager_update(&input, dt); // Update active scene and process transitions

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK);
            scene_manager_draw();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    scene_manager_deinit(); // Free dynamic resources owned by the game state and scenes
    assets_unload(); // Unload all loaded textures, fonts, sound effects, and music streams
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}