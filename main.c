#include <raylib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"

char *SimplifyFEN(char* FEN);

int main(void)
{
    int screenWidth = 1920;
    int screenHeight = 1080;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Heatchess Indev");
    
    SetTargetFPS(144);

    char FEN[90] = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";
    board b = FENtoBoard(FEN);

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

            // get position from clipboard
            if (IsKeyPressed(KEY_V))
            {
                char *clipboard = GetClipboardText();
                b = FENtoBoard(clipboard);
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
                    
                    // mouse logic
                    if (MouseX > sx && MouseX < sx + scale && MouseY > sy && MouseY < sy + scale)
                    {
                        if (IsMouseButtonDown(3))
                        {
                            DrawRectangle(sx, sy, scale, scale, BLUE);
                        }
                        else
                        {
                            DrawRectangle(sx, sy, scale, scale, GREEN);
                        }
                    }

                    // drawtext breaks without \0
                    char formatted[2];
                    formatted[0] = b.pieces[x][y];
                    formatted[1] = '\0';
                    DrawText(formatted, sx + scale / 4, sy, scale, WHITE);
                }
            }
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}

board FENtoBoard(char *fen)
{
    board converted;
    // simplify the FEN
    char simple[64];
    int i = 0;
    int j = 0;
    while (fen[i] != ' ')
    {
        if (isalpha(fen[i]))
        {
            simple[j] = fen[i];
            j++;
        }
        else if (isdigit(fen[i]))
        {
            int c = fen[i] - '0';
            for (int d = 0; d < c; d++)
            {
                simple[j] = ' ';
                j++;
            }
        }
        i++;
    }
    
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            converted.pieces[x][y] = simple[(x + 8 * y)];
        }
    }
    return converted;
}