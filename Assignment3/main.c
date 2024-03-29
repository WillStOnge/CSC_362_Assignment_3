// Name: a3.c
// Author: Will St. Onge
// Description: Game of chutes and ladder. Uses rng to determine how far the player will move on the board, then uses pointer dereferencing to test what 'space' the player landed on. If it's a lower-case alpha, it will convert it to ASCII decimal and subtract 110 (n) from it. This determines how far the player will move backwards or forwards. If the character is and 'B' or and 'F', the program will search for an 'H' on the board to move the player to. If it is unsuccessful, it will put the player at the front of the board if it's an 'F' and stay in place otherwise. After a player is moved to a 'H', the 'H' is removes and replaced with a '*'. Each turn, there is a file output which prints out the current board and a console output based on what happened this turn. Once a player reaches the end of the 100-space board, that player will win.

#define SIZE 100 // Size of the board
#define FILENAME "output.txt" // Name of the file for the board to go into

#include <stdio.h> // Functions for input and output
#include <stdlib.h> // Functions for random number generation
#include <time.h> // Functions for working with time

output(char*, char*, char*, FILE*); // Output current board to a file (output.txt)
char* move(char*, char*, char*, int, int); // Move the target player a random (1-6) number of spaces and run tests on where it lands (letter, other player, etc.)
char* findHaven(char*, char*, int, int); // Finds the nearest haven either backwards or forewards with respect to the player
char* chuteLadder(char*, char*); // Get the number of spaces to move the player backwards/forwards based on the letter landed on (letter-'n')

void main()
{
	// Declare game board and file
	char board[SIZE] = "  mHk  H l B He Flq p H  hByHlho H B  jr HFB ir j H  F ku gd  H pjB mH x  BF i H  m oB HlHFBhoH BB ", *p1 = board, *p2 = board;
	FILE* file;

	// Declare the rng seed as the system time and open a file stream
	srand(time(NULL));
	fopen_s(&file, FILENAME, "w");

	// Keep going until the loop breaks
	while (!(p1 - board >= SIZE - 1 || p2 - board >= SIZE - 1))
	{
		// Move each player
		p1 = move(p1, p2, board, 1, SIZE);
		p2 = move(p2, p1, board, 2, SIZE);

		// Test if player pointers are before the board pointer. If one is, set it to the board pointer
		if (p1 < board)
			p1 = board;
		if (p2 < board)
			p2 = board;

		// Output the player pointers to a file
		output(board, p1, p2, file);
	}
	// Out the winner
	p1 >= board + SIZE ? printf("Player 1 wins!") : printf("Player 2 wins!");

	// Close the file
	fclose(file);
}

char* move(char* p1, char* p2, char* board, int player_number, int board_size)
{
	char* player = p1;
	// Get a random value from the seed in the main function
	int random = rand() % 6 + 1;
	// Increment the player pointer by the random number
	player += random;

	// Base output
	printf("Player %d rolled %d and moved to square %d", player_number, random, player - board);

	// If the pointer points to a space or a -, then do nothing
	if (*player == ' ' || *player == '-' || *player == 'n' || *player == '*')
		player = player;
	// If it is a lower case character besides 'n', then it is a chute or ladder
	else if (*player >= 'a' &&  *player <= 'z')
	{
		char* res = chuteLadder(player, board);

		res > player ? printf(" which is a ladder and is moving forwards to square %d", res - board) : printf(" which is a chute and is moving back to square %d", player - board);
		player = res;
	}
	// If the pointers are the same and its not the starting square, move p1 up my one
	if (player == p2 && player != board)
	{
		player--;
		printf(" -- Collision! %d is moving back 1 square to %d", player_number, player - board);
	}
	// If the pointer points to B then find a haven behind the pointer and output more info to the console
	if (*player == 'B')
	{
		player = findHaven(board, player, player_number, 0);

		(player == board) ? printf(" which is a 'B' so is moving back and with no more havens, lands at 0") : printf(" which is a 'B' so is moving back and lands at %d", player - board);
		if (*player == 'H')
			*player = '*';
	}
	// If the pointer points to B then find a haven ahead the pointer and output more info to the console
	if (*player == 'F')
	{
		char* res = findHaven(board, player, player_number, 1);

		res == player ? printf(" which is a 'F' so is moving forward and with no more havens, lands at %d", res - board) : printf(" which is a 'F' so is moving forward and lands at %d", res - board);
		player = res;
		if(*player == 'H')
			*player = '*';
	}
	// If the pointers are the same and its not the starting square, move p1 up my one
	if (player == p2 && player != board)
	{
		player--;
		printf(" -- Collision! %d is moving back 1 square to %d", player_number, player - board);
	}

	// Make sure player pointer is not behind the board pointer
	if (player < board)
		player = board;

	printf("\n");

	return player;
}

output(char* board, char* p1, char* p2, FILE* file)
{
	// Temp vars for output
	char* pboard = board, * pplayer1 = p1, * pplayer2 = p2;

	// Iterate through the string until end of string is reached
	while (*pboard != '\0')
	{
		// Output 1 if it is the p1 pointer, 2 for p2 pointer, and the letter if nothing is there
		if (pboard == pplayer1)
			putc('1', file);
		else if (pboard == pplayer2)
			putc('2', file);
		else
			putc(*pboard, file);
		pboard = pboard + 1;
	}

	// Print newline character
	fprintf(file, "\n");
}

char* findHaven(char *board, char *player, int player_num, int forwards)
{
	char* pplayer = player;

	// If the haven we're looking for is gonna be forwards
	if (forwards) 
	{
		// Iterate though until a haven is found or until end of string. End of string means the pointer will not move
		while (*pplayer != '\0')
		{
			if (*pplayer == 'H')
				return pplayer;
			pplayer++;
		}
		return player;
	}
	// Haven is gonna be behind the pointer
	else
	{
		// Iterate though until haven is found or it reaches the beginning of the board. If it reaches the beginning of the board, send it to the beginning of the board
		while (pplayer != board)
		{
			if (*pplayer == 'H')
				return pplayer;
			pplayer--;
		}
		return board;
	}
}

char* chuteLadder(char *player, char *board)
{
	// Move to player pointer based on the letter the pointer is currently pointing to (Example m - n = -1)
	char* pplayer = player + (((int)* player) - 'n');
	// Set previous position to a - to signify it has been used
	*player = '-';
	// Test to see if the player pointer is behind the board, then return the player pointer
	if (pplayer < board)
		pplayer = board;
	return pplayer;
}