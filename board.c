#include "board.h"

board FENtoBoard(char *fen)
{
    board converted;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            converted.pieces[x][y] = 'a';
        }
    }
    return converted;
}