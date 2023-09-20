#include <stdio.h>
#include <stdlib.h>

char* game(char* board[7][7]);
char* playerTurn(char* board[7][7], char* piece);
char* aiTurn(char* board[7][7]);
int evaluateMoves(char* board[7][7],char* piece, int thresh, int scores[7]);
int checkWin(char* board[7][7],char* piece);
void showBoard(char* board[7][7]);

