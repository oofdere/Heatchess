#include <raylib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"

Texture2D textures[2][6];

int main(void)
{
    int screenWidth = 1920;
    int screenHeight = 1080;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Heatchess Indev");
    
    SetTargetFPS(144);

    char FEN[90] = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";
    board b = FENtoBoard(FEN);

    // load images
    LoadTextures();

    // gameplay loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            
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
                    Rectangle rect;
                    rect.x = sx;
                    rect.y = sy;
                    rect.height = scale;
                    rect.width = scale;
                    
                    if (flip)
                    {
                        DrawRectangle(sx, sy, scale, scale, LIGHTGRAY);
                        flip = false;
                    }
                    else
                    {
                        DrawRectangle(sx, sy, scale, scale, WHITE);
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
                    if (b.pieces[x][y] != ' ')
                    {
                        char formatted[2];
                        formatted[0] = b.pieces[x][y];
                        formatted[1] = '\0';
                        DrawText(formatted, sx + scale / 4, sy, scale, WHITE);
                        Vector2 pos;
                        pos.x = 0;
                        pos.y = 0;
                        
                        Texture2D texture = DrawPiece(b.pieces[x][y]);
                        Rectangle texturebounds;
                        texturebounds.x = 0;
                        texturebounds.y = 0;
                        texturebounds.height = texture.height;
                        texturebounds.width = texture.width;
                        DrawTexturePro(texture, texturebounds, rect, pos, 0, WHITE);
                    } 
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

void LoadTextures(void)
{
    // bishop
    textures[0][0] = LoadTextureFromImage(LoadImage("assets/white_bishop.png"));
    textures[1][0] = LoadTextureFromImage(LoadImage("assets/black_bishop.png"));

    // king
    textures[0][1] = LoadTextureFromImage(LoadImage("assets/white_king.png"));
    textures[1][1] = LoadTextureFromImage(LoadImage("assets/black_king.png"));
    
    // knight
    textures[0][2] = LoadTextureFromImage(LoadImage("assets/white_knight.png"));
    textures[1][2] = LoadTextureFromImage(LoadImage("assets/black_knight.png"));

    // pawn
    textures[0][3] = LoadTextureFromImage(LoadImage("assets/white_pawn.png"));
    textures[1][3] = LoadTextureFromImage(LoadImage("assets/black_pawn.png"));

    // queen
    textures[0][4] = LoadTextureFromImage(LoadImage("assets/white_queen.png"));
    textures[1][4] = LoadTextureFromImage(LoadImage("assets/black_queen.png"));

    // rook
    textures[0][5] = LoadTextureFromImage(LoadImage("assets/white_rook.png"));
    textures[1][5] = LoadTextureFromImage(LoadImage("assets/black_rook.png"));

    // texture filtering
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            SetTextureFilter(textures[i][j], TEXTURE_FILTER_TRILINEAR);
        }
        
    }
    
}

Texture2D DrawPiece(char p)
{
    int i = 0;
    int j = 0;
    
    if (islower(p))
    {
        i = 1;
    }
    
    switch (tolower(p))
    {
        case 'b':
            j = 0;
            break;
        case 'k':
            j = 1;
            break;
        case 'n':
            j = 2;
            break;
        case 'p':
            j = 3;
            break;
        case 'q':
            j = 4;
            break;
        case 'r':
            j = 5;
            break;    
        default:
            break;
    }

    return textures[i][j];
}