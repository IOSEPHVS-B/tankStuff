#include <stdio.h>
#include <raylib.h>
#include "modes.h"

int main(void)
{
    scale = 1.0f;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(GAME_WIDTH * scale, GAME_HEIGHT * scale, "Tank Toilette");
    SetWindowMinSize(GAME_WIDTH, GAME_HEIGHT);

    SetExitKey(KEY_ESCAPE);
    InitAudioDevice();
    HideCursor();

    RenderTexture2D renderTarget = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    SetTextureFilter(renderTarget.texture, FILTER_POINT);

    SetTargetFPS(60);

    modeFunctions* modeFuncs[MODE_NUM];
    currentMode = modePlay;

    modeFuncs[modePlay] = initGame();

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F11)) 
        {
            ToggleFullscreen();
        }

        scale = min((float)GetScreenWidth()/GAME_WIDTH, (float)GetScreenHeight()/GAME_HEIGHT);

        float dt = GetFrameTime();
        modeFuncs[currentMode] -> update(dt);

        // DRAWING
        BeginDrawing();

            ClearBackground(BLACK);

            BeginTextureMode(renderTarget);

                // Call the current mode's draw function
                modeFuncs[currentMode] -> draw();

            EndTextureMode();

            // Now draw the actual canvas to the screen
            DrawTexturePro(
                renderTarget.texture, 
                (Rectangle){ 0.0f, 0.0f, (float)renderTarget.texture.width, (float)-renderTarget.texture.height },
                (Rectangle){ (GetScreenWidth() - ((float)GAME_WIDTH*scale))*0.5, (GetScreenHeight() - ((float)GAME_HEIGHT*scale))*0.5, (float)GAME_WIDTH*scale, (float)GAME_HEIGHT*scale }, 
                (Vector2){ 0, 0 }, 
                0.0f, 
                WHITE);


        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
