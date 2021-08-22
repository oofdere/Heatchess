#ifndef HEATCHESS_BOARD_H
#define HEATCHESS_BOARD_H

typedef struct board
{
    char pieces[8][8];
}board;

board FENtoBoard(char *fen);
void LoadTextures(void);
Texture2D DrawPiece(char p);
#endif