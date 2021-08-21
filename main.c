#include <raylib.h>

int main(void)
{
    int screenWidth = 1920;
    int screenHeight = 1080;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Heatchess Indev");
    
    SetTargetFPS(144);

    // gameplay loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            // keep window size up to date
            if (IsWindowResized)
            {
                screenWidth = GetScreenWidth(); 
                screenHeight = GetScreenHeight();
            }

            // draw the chess board
            bool flip = true; // square color
            float scale = screenHeight / 8; // size of squares
            int base = screenWidth / 2 - screenHeight / 2; // place to start drawing from
            for (int y = 0; y < 8; y++)
            {
                int sy = scale * y;
                flip = !flip;
                for (int x = 0; x < 8; x++)
                {
                    int sx = scale * x + base;
                    if (flip)
                    {
                        DrawRectangle(sx, sy, scale, scale, RED);
                        flip = false;
                    }
                    else
                    {
                        DrawRectangle(sx, sy, scale, scale, BLACK);
                        flip = true;
                    }
                }
            }
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}