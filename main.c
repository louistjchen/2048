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
    printf("===========================\n");
    
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++)
            if(A->board[i][j] == 0) printf("     ");
            else printf("%5.d", A->board[i][j]);
        printf("\n\n");
    }
    
    printf("===========================\n");
    printf("Your current score: %d\n", A->score);
    return;
}


// function arrayEqual
// This function copies array b to array a. It return true if they are equal; vice versa.
bool arrayEqual(int *a, int *b, int size){
    
    bool moved = false;
    for(int i = 0; i < size; i++){
        
        if(a[i] != b[i])
            moved = true;
        a[i] = b[i];
    }
    return moved;
}


// function arrayReset
// This function resets an array size of 4 with a, b, c, d.
void arrayReset(int *array, int a, int b, int c, int d){
    
    array[0] = a;
    array[1] = b;
    array[2] = c;
    array[3] = d;
    return;
}


// function "pushZero
// This function pushes an array's internal 0's to end of array, if any.
int *pushZero(int *array){
    
    int new_array[4] = {0,0,0,0};
    int index = 0;
    
    for(int i = 0; i < 4; i++)
        if(array[i] != 0){
            new_array[index] = array[i];
            index++;
        }
    arrayEqual(array, new_array, 4);

    return array;
}


// function "pushCombine"
// This function adds up an array's internal adjacent elements if they are equal.
int *pushCombine(int *array){
    
    int i = 0;
    int j = 0;
    for (; i < 4;) {
        
        if(j < 3) { // j , j+1 entry both in range
            if(array[j] == array [j+1]) {
                array[i] = 2 * array[j];
                j = j + 2;
            }
            else {
                array[i] = array[j];
                j++;
            }
        }
        else if(j == 3) { // j is last entry
            array[i] = array[j];
            j++;
        }
        else if(j > 3)   // j out of range
            array[i] = 0;
        
        i++;
        
    }
    return array;
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
    
    int array1[4] = {0,0,0,0};
    int array2[4] = {0,0,0,0};
    int array3[4] = {0,0,0,0};
    int array4[4] = {0,0,0,0};
    int old_array1[4] = {0,0,0,0};
    int old_array2[4] = {0,0,0,0};
    int old_array3[4] = {0,0,0,0};
    int old_array4[4] = {0,0,0,0};
    bool moved = false;
    
    switch(command){
            
        case 'w':
            arrayReset(array1, A->board[0][0], A->board[1][0], A->board[2][0], A->board[3][0]);
            arrayReset(array2, A->board[0][1], A->board[1][1], A->board[2][1], A->board[3][1]);
            arrayReset(array3, A->board[0][2], A->board[1][2], A->board[2][2], A->board[3][2]);
            arrayReset(array4, A->board[0][3], A->board[1][3], A->board[2][3], A->board[3][3]);
            arrayReset(old_array1, A->board[0][0], A->board[1][0], A->board[2][0], A->board[3][0]);
            arrayReset(old_array2, A->board[0][1], A->board[1][1], A->board[2][1], A->board[3][1]);
            arrayReset(old_array3, A->board[0][2], A->board[1][2], A->board[2][2], A->board[3][2]);
            arrayReset(old_array4, A->board[0][3], A->board[1][3], A->board[2][3], A->board[3][3]);
            
            pushCombine(pushZero(array1));
            pushCombine(pushZero(array2));
            pushCombine(pushZero(array3));
            pushCombine(pushZero(array4));
            moved = arrayEqual(old_array1, array1, 4) | arrayEqual(old_array2, array2, 4) | arrayEqual(old_array3, array3, 4) | arrayEqual(old_array4, array4, 4);

            A->board[0][0] = array1[0];
            A->board[1][0] = array1[1];
            A->board[2][0] = array1[2];
            A->board[3][0] = array1[3];
            A->board[0][1] = array2[0];
            A->board[1][1] = array2[1];
            A->board[2][1] = array2[2];
            A->board[3][1] = array2[3];
            A->board[0][2] = array3[0];
            A->board[1][2] = array3[1];
            A->board[2][2] = array3[2];
            A->board[3][2] = array3[3];
            A->board[0][3] = array4[0];
            A->board[1][3] = array4[1];
            A->board[2][3] = array4[2];
            A->board[3][3] = array4[3];
            
            break;
            
        case 's':
            arrayReset(array1, A->board[3][0], A->board[2][0], A->board[1][0], A->board[0][0]);
            arrayReset(array2, A->board[3][1], A->board[2][1], A->board[1][1], A->board[0][1]);
            arrayReset(array3, A->board[3][2], A->board[2][2], A->board[1][2], A->board[0][2]);
            arrayReset(array4, A->board[3][3], A->board[2][3], A->board[1][3], A->board[0][3]);
            arrayReset(old_array1, A->board[3][0], A->board[2][0], A->board[1][0], A->board[0][0]);
            arrayReset(old_array2, A->board[3][1], A->board[2][1], A->board[1][1], A->board[0][1]);
            arrayReset(old_array3, A->board[3][2], A->board[2][2], A->board[1][2], A->board[0][2]);
            arrayReset(old_array4, A->board[3][3], A->board[2][3], A->board[1][3], A->board[0][3]);
            
            pushCombine(pushZero(array1));
            pushCombine(pushZero(array2));
            pushCombine(pushZero(array3));
            pushCombine(pushZero(array4));
            moved = arrayEqual(old_array1, array1, 4) | arrayEqual(old_array2, array2, 4) | arrayEqual(old_array3, array3, 4) | arrayEqual(old_array4, array4, 4);
            
            A->board[0][0] = array1[3];
            A->board[1][0] = array1[2];
            A->board[2][0] = array1[1];
            A->board[3][0] = array1[0];
            A->board[0][1] = array2[3];
            A->board[1][1] = array2[2];
            A->board[2][1] = array2[1];
            A->board[3][1] = array2[0];
            A->board[0][2] = array3[3];
            A->board[1][2] = array3[2];
            A->board[2][2] = array3[1];
            A->board[3][2] = array3[0];
            A->board[0][3] = array4[3];
            A->board[1][3] = array4[2];
            A->board[2][3] = array4[1];
            A->board[3][3] = array4[0];
            
            break;
            
        case 'a':
            arrayReset(array1, A->board[0][0], A->board[0][1], A->board[0][2], A->board[0][3]);
            arrayReset(array2, A->board[1][0], A->board[1][1], A->board[1][2], A->board[1][3]);
            arrayReset(array3, A->board[2][0], A->board[2][1], A->board[2][2], A->board[2][3]);
            arrayReset(array4, A->board[3][0], A->board[3][1], A->board[3][2], A->board[3][3]);
            arrayReset(old_array1, A->board[0][0], A->board[0][1], A->board[0][2], A->board[0][3]);
            arrayReset(old_array2, A->board[1][0], A->board[1][1], A->board[1][2], A->board[1][3]);
            arrayReset(old_array3, A->board[2][0], A->board[2][1], A->board[2][2], A->board[2][3]);
            arrayReset(old_array4, A->board[3][0], A->board[3][1], A->board[3][2], A->board[3][3]);
            
            pushCombine(pushZero(array1));
            pushCombine(pushZero(array2));
            pushCombine(pushZero(array3));
            pushCombine(pushZero(array4));
            moved = arrayEqual(old_array1, array1, 4) | arrayEqual(old_array2, array2, 4) | arrayEqual(old_array3, array3, 4) | arrayEqual(old_array4, array4, 4);
            
            A->board[0][0] = array1[0];
            A->board[0][1] = array1[1];
            A->board[0][2] = array1[2];
            A->board[0][3] = array1[3];
            A->board[1][0] = array2[0];
            A->board[1][1] = array2[1];
            A->board[1][2] = array2[2];
            A->board[1][3] = array2[3];
            A->board[2][0] = array3[0];
            A->board[2][1] = array3[1];
            A->board[2][2] = array3[2];
            A->board[2][3] = array3[3];
            A->board[3][0] = array4[0];
            A->board[3][1] = array4[1];
            A->board[3][2] = array4[2];
            A->board[3][3] = array4[3];
            
            break;
            
        case 'd':
            arrayReset(array1, A->board[0][3], A->board[0][2], A->board[0][1], A->board[0][0]);
            arrayReset(array2, A->board[1][3], A->board[1][2], A->board[1][1], A->board[1][0]);
            arrayReset(array3, A->board[2][3], A->board[2][2], A->board[2][1], A->board[2][0]);
            arrayReset(array4, A->board[3][3], A->board[3][2], A->board[3][1], A->board[3][0]);
            arrayReset(old_array1, A->board[0][3], A->board[0][2], A->board[0][1], A->board[0][0]);
            arrayReset(old_array2, A->board[1][3], A->board[1][2], A->board[1][1], A->board[1][0]);
            arrayReset(old_array3, A->board[2][3], A->board[2][2], A->board[2][1], A->board[2][0]);
            arrayReset(old_array4, A->board[3][3], A->board[3][2], A->board[3][1], A->board[3][0]);
            
            pushCombine(pushZero(array1));
            pushCombine(pushZero(array2));
            pushCombine(pushZero(array3));
            pushCombine(pushZero(array4));
            moved = arrayEqual(old_array1, array1, 4) | arrayEqual(old_array2, array2, 4) | arrayEqual(old_array3, array3, 4) | arrayEqual(old_array4, array4, 4);
            
            A->board[0][0] = array1[3];
            A->board[0][1] = array1[2];
            A->board[0][2] = array1[1];
            A->board[0][3] = array1[0];
            A->board[1][0] = array2[3];
            A->board[1][1] = array2[2];
            A->board[1][2] = array2[1];
            A->board[1][3] = array2[0];
            A->board[2][0] = array3[3];
            A->board[2][1] = array3[2];
            A->board[2][2] = array3[1];
            A->board[2][3] = array3[0];
            A->board[3][0] = array4[3];
            A->board[3][1] = array4[2];
            A->board[3][2] = array4[1];
            A->board[3][3] = array4[0];
            
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
        
        updateBoard(A, command);
        printBoard(A);
        
        if(!playable(A)){
            printf("Gameover!\n");
            break;
        }
        
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
