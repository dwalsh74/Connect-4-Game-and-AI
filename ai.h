
#ifndef _AI_H_
#define _AI_H_

char* aiTurn(char* board[7][7]);
int evaluateMoves(char* board[7][7],char* piece, int thresh, int scores[7]);

#endif
