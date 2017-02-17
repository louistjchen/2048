//
//  main.c
//  2048_qt
//
//  Created by Louis Chen & William Fan on 2017-02-12.
//  Copyright © 2017 Louis Chen & William Fan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>



// basic struct of BOARD
struct BOARD{
    
    int **board;
    int numEmpty;
    int score;
};


// function randomFromRange
// This function returns a random number between arguments min/max. It takes current time (seconds) as its seed to avoid repetition.
int randomFromRange(int min, int max){
    
    time_t current_time = time(NULL);
    return (int)((current_time + rand()) % (max + 1 - min) + min);
}


// function initBoard
// This function memory-allocates a struct, a 2D array. It initializes 2D array to have 2 2's in random positions.
struct BOARD *initBoard(struct BOARD *A){
    
    A = (struct BOARD *)malloc(sizeof(struct BOARD));
    
    A->board = (int **)malloc(sizeof(int*)*4);
    for(int i = 0; i < 4; i ++) {
        A->board[i] = (int *)malloc(sizeof(int)*4);{
	for (int j = 0; j < 4; j++)
		A->board[i][j] = 0;
	}
    }
    int first = randomFromRange(0, 15);
    int second = first;
    while(second == first)
        second = randomFromRange(0, 15);
    
    A->board[first/4][first%4] = 2;
    A->board[second/4][second%4] = 2;
    
    A->numEmpty = 14;
    A->score = 0;
    
    return A;
}


// function deleteBoard
// This function frees the 2D array of board and the entire struct.
void deleteBoard(struct BOARD *A){
    
    for(int i = 0; i < 4; i++)
        free(A->board[i]);
    free(A->board);
    free(A);
    return;
}


// function restartBoard
// This function restarts the game. It reintializes game to have 2 2's in random positions.
void restartBoard(struct BOARD *A){

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            A->board[i][j] = 0;
    
    int first = randomFromRange(0, 15);
    int second = first;
    while(second == first)
        second = randomFromRange(0, 15);
    
    A->board[first/4][first%4] = 2;
    A->board[second/4][second%4] = 2;
    
    A->numEmpty = 14;
    A->score = 0;

    return;
}


// function printBoard
// This function prints the board in 2048's manner (4x4).
void printBoard(struct BOARD *A){
    
    printf("\nCurrent board is as follows\n");
    printf("================================\n");
    
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++)
            if(A->board[i][j] == 0) printf("       |");
            else printf("%5.d  |", A->board[i][j]);
        printf("\n\n");
    }
    
    printf("===============================\n");
    printf("Your current score: %d\n", A->score);
    return;
}

//function findNextNonzeroEntry
//This function acts as a helper function to updateLeftUp/updateRightDown.
//It finds the next non zero cell with a given start point and direction.
int findNextNonzeroEntry(int **board, int row, int col, char dir) {
	
	int result = 0;

	switch (dir) {
	case 'a': {
		for (col; col < 4; col++) { // search in increasing direction, which is rightward.
			if (board[row][col] != 0)
				return col;
		}
		result = 4;
		break;
	}

	case 'w': {
		for (row; row < 4; row++) { // search in increasing direction, which is downward.
			if (board[row][col] != 0)
				return row;
		}
		result = 4;
		break;
	}

	case 'd': {
		for (col; col >= 0 ; col--) { // search in decreasing direction, which is leftward.
			if (board[row][col] != 0)
				return col;
		}
		result = -1;
		break;
	}

	case 's': {
		for (row; row >= 0; row--) { // search in decreasing direction, which is upward.
			if (board[row][col] != 0)
				return row;
		}
		result = -1;
		break;
	}

	default:
		break;
	}
	return result;
}

// function updateLeftUp
// This function update the grid responding to left/up input detected.
bool updateLeftUp(struct BOARD* A, char dir) {
	bool moved = false;
	for (int i = 0; i < 4; i++) {
		int j = 0;
		int k = 0;
		for (; j < 4; j++) {
			int val = 0;
			if (k > 3) {  // The next date entry point is out of range, means the rest value for this column/row will be 0.
				val = 0;
				k = 4;
			}
			else if (k == 3) { // The next date entry point is the last (i.e fourth) element of this row/column.
				val = (dir == 'a') ? A->board[i][k] : A->board[k][i];
				k = 4;
			}
			else if (k < 3) {
				int n1 = findNextNonzeroEntry(A->board, (dir == 'a') ? i : k, (dir == 'a') ? k : i, dir);
				if (n1 > 3) { //The next nonzero date entry does not exist. thus make rest column/row all 0.
					val = 0;
					k = 4;
				}
				else if (n1 == 3) {
					val = (dir == 'a') ? A->board[i][n1] : A->board[n1][i];
					k = 4; //Set the next dataentry point to be 4, which is out of 0-3 range, make rest values in column/row all 0.
				}
				else if (n1 < 3) {
					int n2 = findNextNonzeroEntry(A->board, (dir == 'a') ? i : n1 + 1, (dir == 'a') ? n1 + 1 : i, dir);
					if (n2 > 3) {
						val = (dir == 'a') ? A->board[i][n1] : A->board[n1][i]; //  Set current value to the only found nonzero value left, which is A->board[i][n1].
						k = 4;
					}
					else if (n2 <= 3) { // There are at least two nonzero values left to be continued, check if they equal.
						if (dir == 'a' && A->board[i][n1] == A->board[i][n2]
							|| dir == 'w' && A->board[n1][i] == A->board[n2][i]) {
							val = ((dir == 'a') ? A->board[i][n1] : A->board[n1][i]) * 2;
							A->score += val;
							k = n2 + 1;
						}
						else {
							val = (dir == 'a') ? A->board[i][n1] : A->board[n1][i];
							k = n1 + 1;
						}
					}
				}
			}
			if (dir == 'a') {
				if (!moved)
					moved = (A->board[j][i] == val) ? false : true;
				A->board[i][j] = val;
			}
			else {
				if (!moved)
					moved = (A->board[j][i] == val) ? false : true;
				A->board[j][i] = val;
			}
		}
	}
	return moved;
}

// function updateRightDown
// This function update the grid responding to right/down input detected.
bool updateRightDown(struct BOARD* A, char dir) {
	bool moved = false;
	for (int i = 0; i < 4; i++) {
		int j = 3;
		int k = 3;
		for (; j >= 0; j--) {
			int val = 0;
			if (k < 0) {  // The next date entry point is out of range, means the rest value for this column/row will be 0.
				val = 0;
				k = -1;
			}
			else if (k == 0) { // The next date entry point is the last (i.e fourth) element of this row/column.
				val = (dir == 'd') ? A->board[i][k] : A->board[k][i];
				k = -1;
			}
			else if (k > 0) {
				int n1 = findNextNonzeroEntry(A->board, (dir == 'd') ? i : k, (dir == 'd') ? k : i, dir);
				if (n1 < 0) { //The next nonzero date entry does not exist. thus make rest column/row all 0.
					val = 0;
					k = -1;
				}
				else if (n1 == 0) {
					val = (dir == 'd') ? A->board[i][n1] : A->board[n1][i];
					k = -1; //Set the next dataentry point to be 4, which is out of 0-3 range, make rest values in column/row all 0.
				}
				else if (n1 > 0) {
					int n2 = findNextNonzeroEntry(A->board, (dir == 'd') ? i : n1 - 1, (dir == 'd') ? n1 - 1 : i, dir);
					if (n2 < 0) {
						val = (dir == 'd') ? A->board[i][n1] : A->board[n1][i]; //  Set current value to the only found nonzero value left, which is A->board[i][n1].
						k = -1;
					}
					else if (n2 >= 0) { // There are at least two nonzero values left to be continued, check if they equal.
						if (dir == 'd' && A->board[i][n1] == A->board[i][n2]
							|| dir == 's' && A->board[n1][i] == A->board[n2][i]) {
							val = ((dir == 'd') ? A->board[i][n1] : A->board[n1][i]) * 2;
							A->score += val;
							k = n2 - 1;
						}
						else {
							val = (dir == 'd') ? A->board[i][n1] : A->board[n1][i];
							k = n1 - 1;
						}
					}
				}
			}
			if (dir == 'd') {
				if (!moved)
					moved = (A->board[i][j] == val) ? false : true;
				A->board[i][j] = val;
			}
			else {
				if (!moved)
					moved = (A->board[j][i] == val) ? false : true;
				A->board[j][i] = val;
			}
		}
	}
	return moved;
}

// function "insertNewNum"
// This function inserts a random 2 or 4 (probability is defined below) to board. It assumes there's always empty space to insert.
void insertNewNum(struct BOARD *A){
    
    int index = 0;
    int unused[16] = {0};
    int size = 0;
    bool set = false;
    
    for(int i = 0; i < 4; i++){
        
        for(int j = 0; j < 4; j++, index++)
            if(A->board[i][j] == 0){
                
                set = true;
                unused[size] = index;
                size++;
            }
    }
    
    // can remove because this function assumes there's always empty space to insert
    // but leave for debug
    if(!set){
        
        printf("No more empty space to add 2\n");
        return;
    }
    
    int newIndex = randomFromRange(0, size-1);
    newIndex = unused[newIndex];
    int newValue = randomFromRange(0, 4);
    if(newValue < 4)
        newValue = 2;
    else
        newValue = 4;
    
    A->board[newIndex/4][newIndex%4] = newValue;
    
    return;
}



// function "recountNumEmpty"
// This function keeps track of number of empty spaces.
void recountNumEmpty(struct BOARD *A){
    
    A->numEmpty = 0;
    
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(A->board[i][j] == 0)
                A->numEmpty++;
    
    return;
}



// function "updateBoard"
// This function updates the board after certain command is detected. If there is a move, it inserts new number 2 or 4.
void updateBoard(struct BOARD *A, char command){
    
    bool moved = false;
    
    switch(command){
            
        case 'w':      
			moved = updateLeftUp(A, 'w');
            break;
            
        case 's':      
			moved = updateRightDown(A, 's');
            break;
            
        case 'a':
			moved = updateLeftUp(A, 'a');
            break;
            
        case 'd':
			moved = updateRightDown(A, 'd');
            break;

	default:

	    break;
    }
    
    if(moved)
        insertNewNum(A);
    
    recountNumEmpty(A);
    
    return;
}


// function "playableRecursion"
// This is a recursive helper function for function "playable", and should always be launched from A[0][0].
bool playableRecursion(struct BOARD *A, int row, int col){
    
    // return false if 2 out-of-bound cases
    if(row == 4 || col == 4)
        return false;
    if(row == 3 && col == 3)
        return false;
    
    // return equal? if 2 limit cases
    if(row == 3)
        return (A->board[row][col] == A->board[row][col+1]);
    if(col == 3)
        return (A->board[row][col] == A->board[row+1][col]);
    
    // return true (playable) if right or down is equal
    if(A->board[row][col] == A->board[row][col+1])
        return true;
    if(A->board[row][col] == A->board[row+1][col])
        return true;
    
    // recursively search right/down if not base case nor equal adjacent found
    return playableRecursion(A, row, col+1) | playableRecursion(A, row+1, col);
}


// function "playable"
// This function returns false if game no longer valid to play, and vice versa.
bool playable(struct BOARD *A){
    
    if(A->numEmpty > 0)
        return true;
    
    return playableRecursion(A, 0, 0);
}


// function printHelper Menu
// This function prints the welcome message followed by valid commands
void printHelperMenu(){

    printf("\n===================================================\n");
    printf("Welcome to 2048 command line game!\n");
    printf("                developed by Louis Chen & Zeyu Fan!\n\n");
    printf("Some useful commands as follows:\n");
    printf("        w: shift up\n");
    printf("        s: shift down\n");
    printf("        a: shift left\n");
    printf("        d: shift right\n");
    printf("        r: restart game\n");
    printf("        q: exit game\n");
    printf("  ****  h: help\n");
    printf("===================================================\n\n");

    return;
}


// main function
int main(int argc, const char * argv[]) {
   
    // introduction message
    printHelperMenu();    

    char command;
    char empty;
	bool gameOver = false;

    struct BOARD *A = NULL;
    
    A = initBoard(A);
    printBoard(A);

    printf("Please enter command (h: help): ");
    command = getchar();

    while((command == '\n' && command != 'q') || command == 'h'){
        
	if(command == 'h')
	    printHelperMenu();
        // get rid of all chars after first char in stdio
        // Don't do if first char is '\n'
        if(command != '\n')
            do empty = getchar();
            while(empty != '\n');
        printf("Please enter command (h: help): ");
        command = getchar();
    }
    do empty = getchar();
    while(empty != '\n');

    while(command != 'q' ){
        
		if (gameOver) {
			if (command == 'r') {
				restartBoard(A);
				gameOver = true;
			}
			else
				goto INPUT;
		}

        updateBoard(A, command);
        printBoard(A);
        
        if(!playable(A)){
			gameOver = true;
            printf("=====G=A=M=E===O=V=E=R=====!\n");
			printf("YOUR FINAL SCORE IS: %d !\n", A->score);
			printf("PRESS R TO RESTART!")
			goto INPUT;
			continue;
        }
INPUT:       
        printf("Please enter command (h: help): ");
        command = getchar();

        while((command == '\n' && command != 'q') || command== 'h'){
            
	    if(command == 'h')
		printHelperMenu();
	    if(command != '\n')
    	        do empty = getchar();
	        while(empty != '\n');
            printf("Please enter command (h: help): ");
            command = getchar();
        }
        do empty = getchar();
    	while(empty != '\n');
    }
    
    printf("\n");
    
    deleteBoard(A);
    
    return 0;
}
