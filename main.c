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

    char FEN[90] = "8/8/8/4r3/8/8/8/8 w - - 0 1";
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
                        DrawRectangle(sx, sy, scale, scale, WHITE);
                        flip = false;
                    }
                    else
                    {
                        DrawRectangle(sx, sy, scale, scale, WHITE);
                        flip = true;
                    }                    

                    // heatmap
                    float blue = (float) (b.heat[x+8][y+8]) / (float) b.maxheat * 255;
                    DrawRectangle(sx, sy, scale, scale, (Color){ 0, 0, 255, blue });
                    float red = (float) b.heat2[x+8][y+8] / (float) b.maxheat * 255;
                    DrawRectangle(sx, sy, scale, scale, (Color){ 255, 0, 0, red });

                    // drawtext breaks without \0
                    if (IsKeyDown(KEY_F1))
                    {
                        char formatted[3];
                        sprintf(formatted, "%.3f", blue);
                        DrawText(formatted, sx + scale / 4, sy, scale / 2, RED);
                    }
                    else if (IsKeyDown(KEY_F2))
                    {
                        char formatted[3];
                        sprintf(formatted, "%f", red);
                        DrawText(formatted, sx + scale / 4, sy, scale / 2, RED);
                    }
                    else if (IsKeyDown(KEY_F3))
                    {
                        char formatted[3];
                        sprintf(formatted, "%i", b.heat2[x+8][y+8] - b.heat[x+8][y+8]);
                        DrawText(formatted, sx + scale / 4, sy, scale / 2, RED);
                    }
                    else if (IsKeyDown(KEY_F4))
                    {
                        char formatted[3];
                        sprintf(formatted, "%i", b.maxheat);
                        DrawText(formatted, sx + scale / 4, sy, scale / 2, RED);
                    }
                    else if (b.pieces[x][y] != ' ')
                    {
                        if (IsKeyDown(KEY_SPACE))
                        {
                            char formatted[2];
                            formatted[0] = b.pieces[x][y];
                            formatted[1] = '\0';
                            DrawText(formatted, sx + scale / 4, sy, scale, RED);
                        }
                        else
                        {
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

    // init heatmap to 0
    for (int x = 0; x < 32; x++)
    {
        for (int y = 0; y < 32; y++)
        {
            converted.heat[x][y] = 0;
            converted.heat2[x][y] = 0;
        }
    }
    

    // generate heatmap
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
                // offset
                int xo = x + 8;
                int yo = y + 8;

                if (isupper(converted.pieces[x][y]))
                {
                    switch (tolower(converted.pieces[x][y]))
                    {
                        case 'b':
                            // down/left
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x + i;
                                int yi = y + i;
                                int xoi = xo + i;
                                int yoi = yo + i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat2[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xoi][yoi]++;
                                }
                            }
                            // down/right
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x + i;
                                int yi = y - i;
                                int xoi = xo + i;
                                int yoi = yo - i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat2[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xoi][yoi]++;
                                }
                            }
                            // up/left
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x - i;
                                int yi = y + i;
                                int xoi = xo - i;
                                int yoi = yo + i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat2[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xoi][yoi]++;
                                }
                            }
                            // up/right
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x - i;
                                int yi = y - i;
                                int xoi = xo - i;
                                int yoi = yo - i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat2[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xoi][yoi]++;
                                }
                            }
                            break;
                        case 'k':
                            // up/down
                            converted.heat2[xo][yo+1]++;
                            converted.heat2[xo][yo-1]++;
                            // left/right
                            converted.heat2[xo+1][yo]++;
                            converted.heat2[xo-1][yo]++;
                            // corners
                            converted.heat2[xo+1][yo+1]++;
                            converted.heat2[xo-1][yo-1]++;
                            converted.heat2[xo+1][yo-1]++;
                            converted.heat2[xo-1][yo+1]++;
                            break;
                        case 'n':
                            converted.heat2[xo+1][yo+2]++;
                            converted.heat2[xo-1][yo+2]++;
                            converted.heat2[xo+2][yo+1]++;
                            converted.heat2[xo-2][yo+1]++;
                            converted.heat2[xo+1][yo-2]++;
                            converted.heat2[xo-1][yo-2]++;
                            converted.heat2[xo+2][yo-1]++;
                            converted.heat2[xo-2][yo-1]++;
                            break;
                        case 'p':
                            converted.heat2[xo+1][yo-1]++;
                            converted.heat2[xo-1][yo-1]++;
                            break;
                        case 'q':
                            // down/left
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x + i;
                                int yi = y + i;
                                int xoi = xo + i;
                                int yoi = yo + i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat2[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xoi][yoi]++;
                                }
                            }
                            // down/right
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x + i;
                                int yi = y - i;
                                int xoi = xo + i;
                                int yoi = yo - i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat2[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xoi][yoi]++;
                                }
                            }
                            // up/left
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x - i;
                                int yi = y + i;
                                int xoi = xo - i;
                                int yoi = yo + i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat2[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xoi][yoi]++;
                                }
                            }
                            // up/right
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x - i;
                                int yi = y - i;
                                int xoi = xo - i;
                                int yoi = yo - i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat2[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xoi][yoi]++;
                                }
                            }
                            // north
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x][y+i] != ' ')
                                {
                                    converted.heat2[xo][yo+i]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xo][yo+i]++;
                                }
                            }
                            // south
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x][y-i] != ' ')
                                {
                                    converted.heat2[xo][yo-i]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xo][yo-i]++;
                                }
                            }
                            // east
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x+i][y] != ' ')
                                {
                                    converted.heat2[xo+i][yo]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xo+i][yo]++;
                                }
                            }
                            // west
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x-i][y] != ' ')
                                {
                                    converted.heat2[xo-i][yo]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xo-i][yo]++;
                                }
                            }
                            break;  
                        case 'r':
                            // north
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x][y+i] != ' ')
                                {
                                    converted.heat2[xo][yo+i]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xo][yo+i]++;
                                }
                            }
                            // south
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x][y-i] != ' ')
                                {
                                    converted.heat2[xo][yo-i]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xo][yo-i]++;
                                }
                            }
                            // east
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x+i][y] != ' ')
                                {
                                    converted.heat2[xo+i][yo]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xo+i][yo]++;
                                }
                            }
                            // west
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x-i][y] != ' ')
                                {
                                    converted.heat2[xo-i][yo]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat2[xo-i][yo]++;
                                }
                            }
                            break;    
                        default:
                            break;
                    }
                }
                else if (islower(converted.pieces[x][y]))
                {
                    switch (converted.pieces[x][y])
                    {
                        case 'b':
                            // down/left
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x + i;
                                int yi = y + i;
                                int xoi = xo + i;
                                int yoi = yo + i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xoi][yoi]++;
                                }
                            }
                            // down/right
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x + i;
                                int yi = y - i;
                                int xoi = xo + i;
                                int yoi = yo - i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xoi][yoi]++;
                                }
                            }
                            // up/left
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x - i;
                                int yi = y + i;
                                int xoi = xo - i;
                                int yoi = yo + i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xoi][yoi]++;
                                }
                            }
                            // up/right
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x - i;
                                int yi = y - i;
                                int xoi = xo - i;
                                int yoi = yo - i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xoi][yoi]++;
                                }
                            }
                            break;
                        case 'k':
                            // up/down
                            converted.heat[xo][yo+1]++;
                            converted.heat[xo][yo-1]++;
                            // left/right
                            converted.heat[xo+1][yo]++;
                            converted.heat[xo-1][yo]++;
                            // corners
                            converted.heat[xo+1][yo+1]++;
                            converted.heat[xo-1][yo-1]++;
                            converted.heat[xo+1][yo-1]++;
                            converted.heat[xo-1][yo+1]++;
                            break;
                        case 'n':
                            converted.heat[xo+1][yo+2]++;
                            converted.heat[xo-1][yo+2]++;
                            converted.heat[xo+2][yo+1]++;
                            converted.heat[xo-2][yo+1]++;
                            converted.heat[xo+1][yo-2]++;
                            converted.heat[xo-1][yo-2]++;
                            converted.heat[xo+2][yo-1]++;
                            converted.heat[xo-2][yo-1]++;
                            break;
                        case 'p':
                            converted.heat[xo+1][yo+1]++;
                            converted.heat[xo-1][yo+1]++;
                            break;
                        case 'q':
                            // down/left
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x + i;
                                int yi = y + i;
                                int xoi = xo + i;
                                int yoi = yo + i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xoi][yoi]++;
                                }
                            }
                            // down/right
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x + i;
                                int yi = y - i;
                                int xoi = xo + i;
                                int yoi = yo - i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xoi][yoi]++;
                                }
                            }
                            // up/left
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x - i;
                                int yi = y + i;
                                int xoi = xo - i;
                                int yoi = yo + i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xoi][yoi]++;
                                }
                            }
                            // up/right
                            for (int i = 1; i < 8; i++)
                            {
                                int xi = x - i;
                                int yi = y - i;
                                int xoi = xo - i;
                                int yoi = yo - i;
                                if (converted.pieces[xi][yi] != ' ')
                                {
                                    converted.heat[xoi][yoi]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xoi][yoi]++;
                                }
                            }
                            // north
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x][y+i] != ' ')
                                {
                                    converted.heat[xo][yo+i]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xo][yo+i]++;
                                }
                            }
                            // south
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x][y-i] != ' ')
                                {
                                    converted.heat[xo][yo-i]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xo][yo-i]++;
                                }
                            }
                            // east
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x+i][y] != ' ')
                                {
                                    converted.heat[xo+i][yo]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xo+i][yo]++;
                                }
                            }
                            // west
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x-i][y] != ' ')
                                {
                                    converted.heat[xo-i][yo]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xo-i][yo]++;
                                }
                            }
                            break;  
                        case 'r':
                            // north
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x][y+i] != ' ')
                                {
                                    converted.heat[xo][yo+i]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xo][yo+i]++;
                                }
                            }
                            // south
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x][y-i] != ' ')
                                {
                                    converted.heat[xo][yo-i]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xo][yo-i]++;
                                }
                            }
                            // east
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x+i][y] != ' ')
                                {
                                    converted.heat[xo+i][yo]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xo+i][yo]++;
                                }
                            }
                            // west
                            for (int i = 1; i < 8; i++)
                            {
                                if (converted.pieces[x-i][y] != ' ')
                                {
                                    converted.heat[xo-i][yo]++;
                                    break;
                                }
                                else
                                {
                                    converted.heat[xo-i][yo]++;
                                }
                            }
                            break;    
                        default:
                            break;
                    }
                }
        }
    }

    // determine the max and min heat
    int max = 0;
    int min = 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
                // offset
                int xo = x + 8;
                int yo = y + 8;
                
                int test = converted.heat[xo][yo] + converted.heat2[xo][yo];

                if (test > max)
                {
                    max = test;
                }
                else if (converted.heat[xo][yo] < min)
                {
                    min = test;
                }
        }
    }
    converted.maxheat = max;
    converted.minheat = min;
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