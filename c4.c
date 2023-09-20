//To add:
//Way to anticipate and prevent traps
//longer term thinking
//experiment with increased aggression
#include "c4.h"


int main()
{	 
	char* board[7][7];
	game(board);
}

char* game(char* board[7][7])
{
	printf("Welcome to C4!\nEnter 1 to play against an ai, 2 for a two player game, or 3 to exit the program.\n");
	int x = 0;
	int mode = 0;
	//select the game mode
	while (0 == mode)
	{
		//get user input
		char c[50];
		gets(c);
		sscanf(c,"%d",&x);
	
		if (1==x)
		{
			printf("You have selected to play against an ai! You will go first.\nTo place a piece simply enter the number shown below the desired column.\n");
			mode = 1;
		}
		else if (2==x)
		{
			printf("You have selected a two player game! Player 1 will go first.\nTo place a piece simply enter the number shown below the desired column.\n");
			mode = 2;
		} 
		else if (3==x)
		{
			printf("Thanks for playing!\n");
			exit(0);
		}
		else
		{
			printf("The number you have entered is invalid.\nEnter 1 to play against, 2 for a two player game, or 3 to exit the program.\n");
		}
	}
	//wipe the board
	for (int i=0;i<7;i++)
	{
		for (int j=0;j<7;j++)
		{
			board[i][j] = "| ";
			printf("%s",board[i][j]);
		}
		printf("|\n");		   
	}
	printf(" 1 2 3 4 5 6 7\n");
	
	//two player game
	if (2 == mode)
	{
		while (1)
		{
			char* piece = "|X";
			playerTurn(board,piece);
			if(checkWin(board,"|X") == 1)
			{
				printf("Player 1 Wins!\n\n\n");
				showBoard(board);
				game(board);
			}
			showBoard(board);
		
			piece = "|O";
			playerTurn(board,piece);
			if(checkWin(board,"|O") == 1)
			{
				printf("Player 2 Wins!\n\n\n");
				showBoard(board);
				game(board);
			}
			showBoard(board);
		}
	}
	//player vs. ai game
	else if (1 == mode)
	{
		while (1)
		{
			char* piece = "|X";
			playerTurn(board,piece);
			if(checkWin(board,"|X") == 1)
			{
				printf("Player 1 Wins!\n");
				showBoard(board);
				game(board);
			}
			showBoard(board);
		
			piece = "|O";
			aiTurn(board);
			if(checkWin(board,"|O") == 1)
			{
				printf("ai Wins!\n");
				showBoard(board);
				game(board);
			}
			showBoard(board);
		}
	}
	else
	{
		printf("feature not implemented yet");
	}
	return 0;   
}


//drop a piece in the column specified by the player
char* playerTurn(char* board[7][7],char* piece)
{
	int a = 7;
	int x = 0;
	while(0 == x)
	{
		//get user input
		char c[50];
		gets(c);
		sscanf(c,"%d",&a);
			
		if (a >= 0 && a <=7)
		{
			x = 1;
		}
		else
		{
			printf ("The number you have entered is invalid. Please enter a number from 1 to 7\n");
		}
	}
	
	for (int i=0;i<7;i++)
	{
		if ("| " == board[6-i][a-1])
		{
			board[6-i][a-1] = piece;
			i = 7;
		}
	}
	return **board;
}

//ai attempts to identify and make the best move
char* aiTurn(char* board[7][7])
{
	int a = 7;
	int scores[7] = {0,0,0,0,0,0,0};
	
	//see if it is the first turn
	int firstTurn = 1;
	for (int i=0; i<7; i++)
	{
		for (int j=0; j<6; j++)
		{
			if (1 == firstTurn && board[i][j] == "|O")
			{
				firstTurn = 0;
				i = 7;
				j = 7;
			}
		}
	}
	//if it is the first turn, select the middle column
	if (1 == firstTurn)
	{
		a = 3;
	}
	
	//see if the ai can win and make the move if so
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

	//identify and mark moves that would give the opponent an opportunity to win 
	int blacklist[7] = {7,7,7,7,7,7,7}; //stores index of column that would give the opponent the opportunity to win if a piece is placed
	char* testBoard[7][7];
	for (int i=0; i<7; i++)
	{
		for (int j=0; j<6; j++)
		{
			//copy the board to a test board
			for (int x=0; x<7; x++)
			{
				for (int y=0; y<7; y++)
				{
					testBoard[x][y] = board[x][y];
				}
			}
			
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
		
	if(a == 7) //if ai can't immediately win
	{
		int thresh = 2;
		//try to first find places to block the most 2s and then 1s if none are found
		//while(thresh>0)
		//{
			//get number of ways each move would benefit the player 
			int totalScores[7] = {0,0,0,0,0,0,0};
			evaluateMoves(board,"|O",thresh,totalScores); 
			//get number of ways each move would hinder the opponent
			evaluateMoves(board,"|X",thresh,scores);  
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
					//see if column is on blacklist, only choose it if its not
					for (int j=0;j<7;j++)
					{
						if (i == blacklist[j])
						{
							j = 7;
						}
						else if (j == 6)
						{
							x = i;
						}
					}
				}
				//find the highest scoring overall move
				if (totalScores[i] > y)
				{
					//see if column is on blacklist, only choose it if its not
					for (int j=0;j<7;j++)
					{
						if (i == blacklist[j])
						{
							j = 7;
						}
						else if (j == 6)
						{
							y = i;
						}
					}
				}
			}
			//if defensive move has at least one benefit, make that move 
			if (scores[x] > 0)
			{
				a = x;
			}
			//if higest scoring overall move is greater than highest defensive, choose to make that move instead
			if (totalScores[y] > scores[x])
			{
				a = y;
			}
			thresh = thresh - 1;
		//} 
		//default to column 3 otherwise 
		if (a == 7)
		{
			a = 3;
		}
	}
		
	int placed = 0;
	//attempt to lay a piece in the chosen column, find another free space if column is full
	int count = 0;
	while (0 == placed)
	{
		//verify that the column isn't on the blacklist, iterate through possibilities if it is
		//this is not used after 7 iterations in case the only moves remaining are blacklisted 
		if (count <= 7)
		{
			for (int j=0;j<7;j++)
			{
				if (a == blacklist[j])
				{
					a = a+1;
				}
				if (a >= 7)
				{
					a = 0;
				}
			}
		}
		//make sure the column is not full
		for (int i=0;i<7;i++)
		{
			if ("| " == board[6-i][a])
			{
				board[6-i][a] = "|O";
				i = 7;
				placed = 1;
			}
		}
		//choose a different a if it is
		if (a<6)
		{
			a = a+1;
		}
		else
		{
			a = 0;
		}
		count = count+1;
	}
	return **board;
}

int checkWin(char* board[7][7],char* piece)
{
	int lineCount = 0;
	//iterateBoard
	for (int i=0;i<7;i++)
	{
		for (int j=0;j<7;j++)
		{
			if (piece == board[i][j])	//if X or O
			{
				//check horizontal
				int x = j;
				while (x<6)	//check right of piece
				{
					x = x+1;
					if (piece == board[i][x])
					{
						lineCount++;
					}
					else
					{
						x = 6;
					}
				}
				if (3 <= lineCount)	//check if 4 or more, end game if so
				{
					return 1;
				}
				lineCount = 0;	//reset lineCount
				
				//check vertical
				x = i;
				while (x<6)	//check below piece
				{
					x = x+1;
					if (piece == board[x][j])
					{
						lineCount++;
					}
					else
					{
						x = 6;
					}
				}
				if (3 <= lineCount)	//check if 4 or more, end game if so 
				{
					return 1;
				}
				lineCount = 0;	//reset lineCount
				
				//check Diagonal	   
				x = i;
				int y = j;
				while (x<6 && y>0)	//check down-left   
				{
					x = x+1;
					y = y-1;
					if (piece == board[x][y])
					{
						lineCount++;
					}
					else
					{
						x = 7;
					}
				}
				if (3 <= lineCount)	//check if 4 or more, end game if so
				{
					return 1;
				}
				lineCount = 0;	//reset lineCount
				
				x = i;
				y = j;
				while (x<6 && y<6)	//check down-right
				{
					x = x+1;
					y = y+1;
					if (piece == board[x][y])
					{
						lineCount++;
					}
					else
					{
						x = 7;
					}
				}
				if (3 <= lineCount)	//check if 4 or more, end game if so
				{
					return 1;
				}
				lineCount = 0;	//reset lineCount 
			}
		}
	}
	return 0;
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
		lineCount = 0;	//reset lineCount
		openCount = 0;  //reset openCount
		}
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
			/*if (x ==3 && y == 2) //********
			{
				printf("\n");
			} */
			for (int k = 0;k < 4;k++) //**check down-right   
			{
				if (piece == board[x][y]) //count number of one type of piece in a set of 4 
				{
					lineCount++;
				}
				else if ("| " == board[x][y])	//count open spaces in set of 4
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

//display the current board
void showBoard(char* board[7][7])
{
	for (int i=0;i<7;i++)
	{
		for (int j=0;j<7;j++)
		{
			printf("%s",board[i][j]);
		}
		printf("|\n");		   
	}
	printf(" 1 2 3 4 5 6 7\n\n");
}
