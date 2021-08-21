#ifndef HEATCHESS_BOARD_H
#define HEATCHESS_BOARD_H

typedef struct board
{
    char pieces[8][8];
}board;

const int epic = 90;

board FENtoBoard(char *fen);
#endif