// Name: a3.c
// Author: Will St. Onge
// Description: Game of chutes and ladders based on pointer arithmetic. Each player gets 1 6-sided dice spin per round and the board is printed out to an output file each round as well as output who moved where and what else happened.

#define SIZE 100 // Size of the board

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
	char board[SIZE] = "  mHk nH l B He Flq p H  hByHlho H B  jr HFB ir j H F ku gd  H pjB mH x  BF i H  m oB HlHFBhoH BB ", *p1 = board, *p2 = board;
	FILE* file;

	// Declare the rng seed as the system time and open a file stream
	srand(time(NULL));
	fopen_s(&file, "output.txt", "w");

	// Keep going until the loop breaks
	while (1)
	{
		// Move each player
		p1 = move(p1, p2, board, 1, SIZE);
		p2 = move(p2, p1, board, 2, SIZE);

		// Test if player pointers are outside the board. If it is, set it to the beginning of the board
		if (p1 < board)
			p1 = board;
		if (p2 < board)
			p2 = board;

		// Output the player pointers to a file
		output(board, p1, p2, file);

		// Win condition
		if (p1 - board >= SIZE - 1 || p2 - board >= SIZE - 1)
			break;
	}
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
	char* pplayer = player + (((int)* player) - (int)'n');
	// Set previous position to a - to signify it has been used
	*player = '-';
	// Test to see if the player pointer is behind the board, then return the player pointer
	if (pplayer < board)
		pplayer = board;
	return pplayer;
}