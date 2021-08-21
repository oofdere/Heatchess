#include <raylib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

char *SimplifyFEN(char* FEN);

int main(void)
{
    int screenWidth = 1920;
    int screenHeight = 1080;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Heatchess Indev");
    
    SetTargetFPS(144);

    char FEN[90] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    char simple[90] = "rnbqkbnrpppppppp                                PPPPPPPPRNBQKBNRwKQkq-01";
    printf("Simple FEN: %s\n", simple);
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

            // keep mouse position up to date
            int MouseX = GetMouseX();
            int MouseY = GetMouseY();

            // show fps
            DrawFPS(0, 0);

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
                    
                    // mouse logic
                    if (MouseX > sx && MouseX < sx + scale && MouseY > sy && MouseY < sy + scale)
                    {
                        DrawRectangle(sx, sy, scale, scale, GREEN);
                    }

                    // drawtext breaks without \0
                    char formatted[2];
                    formatted[0] = simple[x + 8 * y];
                    formatted[1] = '\0';
                    DrawText(formatted, sx + scale / 4, sy, scale, WHITE);
                }
            }
        EndDrawing();
    }
    
    free(simple);
    CloseWindow();

    return 0;
}

char *SimplifyFEN(char* fen)
{
    int i = 0; // to iterate through FEN
    char *simple = malloc(90);
    int j = 0; // to iterate through simple
    while (i != 0)
    {
        /*if (i == '/')
        {
            // do nothing
        }
        else if (isdigit(fen[i]))
        {
            // add that amount of spaces to simple
        }
        else
        {
            // copy over
            simple[j] = fen[i];
            j++;
        }*/
        //simple[i] = fen[i];
        i++;
    }
    simple[0] = 'H';
    simple[1] = 'I';
    simple[2] = '!';
    simple[3] = '\0';
    return simple;
}