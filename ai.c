#include "c4.h"

//ai attempts to identify and make the best move
char* aiTurn(char* board[7][7])
{
	int a = 7;
	//see if the ai can win and make the move if so
	int scores[7] = {0,0,0,0,0,0,0};

	evaluateMoves(board,"|O",3,scores);
	for (int i=0; i<7; i++)
	{
		if (scores[i] !=0)
		{
			a = i;
			i = 7;
		}
	}
	
	//see if the opponent can win and block if so
	if (a == 7)
	{
		evaluateMoves(board,"|X",3,scores); 
		for (int i=0; i<7; i++)
		{
			if (scores[i] !=0)
			{
				a = i;
				i = 7;
			}
		}
	}

	if(a == 7) //if ai can't immediately win
	{
		int blacklist[7] = {7,7,7,7,7,7,7}; //stores index of column that would give the opponent the opportunity to win if a piece is placed
		char* testBoard[7][7];
		
		//copy the board to a test board
		for (int i=0; i<7; i++)
		{
			for (int j=0; j<6; j++)
			{
				testBoard[i][j] = board[i][j];
			}
		}
		
		//identify and mark moves that would give the opponent an opportunity to win 
		for (int i=0; i<7; i++)
		{
			for (int j=0; j<6; j++)
			{
				//test place an O then an X in each column
				if ("| " == board[6-j][i])
				{
					testBoard[6-j][i] = "|O";
					testBoard[5-j][i] = "|X";
					
					//see if this would result in a win by the player
					if (1 == checkWin(testBoard,"|X"))
					{
						blacklist[i] = i;
					}
					j = 6;
				}
			}
		}
		
		//get number of ways each move would benefit the player 
		int totalScores[7] = {0,0,0,0,0,0,0};
		evaluateMoves(board,"|O",2,totalScores); 
		//get number of ways each move would hinder the opponent
		evaluateMoves(board,"|X",2,scores);  
		int x = 0;
		int y = 0;
		
		//identify the highest scoring moves
		for (int i=0; i<7; i++)
		{
			//add the total positive effects of each move
			totalScores[i] = totalScores[i] + scores[i]; 
			//find the highest scoring defensive move
			if (scores[i] > x)
			{
				x = i;
			}
			//find the highest scoring overall move
			if (totalScores[i] > y)
			{
				y = i;
			}
		}
		//if higest scoring overall move is greater than highest defensive , choose to make that move
		if (totalScores[y] > scores[x])
		{
			a = y;
		}
		//otherwise if defensive move has at least one benefit, make that move 
		else if (scores[x] > 0)
		{
			a = x;
		}
		//default to column 3 otherwise
		else
		{
			a = 3;
		}
		  
		/*
		int i = 0;  
		while (i<7)
		{
			 
			if(a == 7 || a == blacklist[i])
			{
				a = getMove(board,"|X",3); //check if player can win and block if so
			}
			if(a == 7 || a == blacklist[i])
			{
				a = getMove(board,"|X",2); //if no one can win, try and stop player from making a line of 3	
			}
			if(a == 7 || a == blacklist[i])
			{
				a = getMove(board,"|O",2); //otherwise, try and make its own line of 3	
			}
			i = i+1;
			if (a != 7 && a != blacklist[i]) //exit loop once a valid move is found
			{
				i = 7;
			}
			i = i+1;
		} 
		*/
	} 
	/*
	if(a == 7)
	{
		a = 3;
	}
	*/
	//**make move based on own pieces
	int placed = 0;
	//attempt to lay a piece in the chpsen column, find another free space if column is full
	while (0 == placed)
	{
		for (int i=0;i<7;i++)
		{
			if ("| " == board[6-i][a])
			{
				board[6-i][a] = "|O";
				i = 7;
				placed = 1;
			}
		}
		if (a<7)
		{
			a = a+1;
		}
		else
		{
			a = a-1;
		}
	}
	return **board;
}

int evaluateMoves(char* board[7][7],char* piece, int thresh,int scores[7])
{
	int lineCount = 0;
	int openCount = 0;
	//fill array with zeros
	for (int i=0;i<7;i++)
	{
		scores[i] = 0;
	}
			
	//check vertical
	for (int j=0;j<(7-thresh);j++)
	{
		for (int i=0;i<7;i++)
		{
			int x = j;
			while (x<6)	
			{
				x = x+1;
				if (piece == board[x][i])
				{
					lineCount++;
					if (thresh <= lineCount && x-3 >=0 && board[x-thresh][i] == "| ")	//check if 3 or more, place a piece to block if so   
					{
						scores[i]++;
					}
				}
				else
				{
					x = 6;
				}
			}
		}
		lineCount = 0;	//reset lineCount
		openCount = 0;  //reset openCount
	}
	for (int j=0;j<4;j++)
	{
		for (int i=0;i<7;i++)
		{
			int x = j;
			//check horizontal
			for (x = j; x < j+4; x++)
			{
				if (piece == board[i][x])	//check if 3 or more, place a piece to block if so 
				{
					lineCount++;
				}
				else if ("| " == board[i][x])	//count open spaces in set of 4
				{
					openCount++;
				}
			}
			if (thresh <= lineCount && openCount == 4 - thresh)
			{
				x = x-1;
				while (x >= j)
				{
					if (((i+1) == 7 || board [i+1][x] != "| ") && board[i][x] == "| ")
					{
						scores[x]++;
					}
					x = x-1;
				}
			}
			lineCount = 0;	//reset lineCount
			openCount = 0;  //reset openCount
		}
		for (int i=0;i<4;i++)
		{
			//**check Diagonal
			int x = i;
			int y = j+3;
			for (int k = 0;k < 4;k++) //**check down-left   
			{
				if (piece == board[x][y]) //check if 3 or more, place a piece to block if so  
				{
					lineCount++;
				}
				else if ("| " == board[i][x])	//count open spaces in set of 4
				{
					openCount++;
				}
				if (thresh <= lineCount && openCount == 4 - thresh)	
				{
					while (x >= i)
					{
						if (board[x][y] == "| " && (x==6 || board[x+1][y] != "| "))
						{
							scores[y]++;
						}
						x = x-1;
						y = y+1;
					}
				}
				x = x+1;
				y = y-1;
			}
			lineCount = 0; //reset lineCount
			openCount = 0;  //reset openCount  
			
			x = i;
			y = j;
			for (int k = 0;k < 4;k++) //**check down-right   
			{
				if (piece == board[x][y]) //count number of one type of piece in a set of 4 
				{
					lineCount++;
				}
				else if ("| " == board[i][x])	//count open spaces in set of 4
				{
					openCount++;
				}
				if (thresh <= lineCount && openCount == 4 - thresh)	
				{
					while (x >= i)
					{
						if (board[x][y] == "| " && (x==6 || board[x+1][y] != "| "))
						{
							scores[y]++;
						}
						x = x-1;
						y = y-1;
					}
				}
				x = x+1;
				y = y+1;
			}
			lineCount = 0; //reset lineCount
			openCount = 0;  //reset openCount  
		}
		lineCount = 0;	//reset lineCount
		openCount = 0;  //reset openCount  
	}
	return *scores;
}
