#include "raylib.h"
#include "core/config.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    ChangeDirectory(GetApplicationDirectory()); // Change working directory to the directory of the running application

    const Config *cfg = config_init();

    if (cfg->fullscreen) {
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
    }

    InitWindow(cfg->screen_width, cfg->screen_height, cfg->title);
    SetExitKey(KEY_NULL); // Unset default exit key (ESC)

    SetTargetFPS(cfg->target_fps); // Set our game to run at default frames-per-second
    //--------------------------------------------------------------------------------------

    Texture2D bgTexture = LoadTexture(ASSETS_PATH "images/bg/game_background_4.png");

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawTexturePro(bgTexture, (Rectangle){0, 0, (float)bgTexture.width, (float)bgTexture.height}, (Rectangle){0, 0, (float)cfg->screen_width, (float)cfg->screen_height}, (Vector2){0, 0}, 0.0f, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    UnloadTexture(bgTexture); // Unload texture from GPU memory (VRAM)
    //--------------------------------------------------------------------------------------

    return 0;
}