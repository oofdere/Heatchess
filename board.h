#ifndef HEATCHESS_BOARD_H
#define HEATCHESS_BOARD_H

typedef struct board
{
    char pieces[8][8];
    int heat[32][32];
    int heat2[32][32];
    int maxheat;
    int minheat;
}board;

board FENtoBoard(char *fen);
void LoadTextures(void);
Texture2D DrawPiece(char p);
#endif