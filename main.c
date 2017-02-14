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


struct BOARD{
    
    int **board;
    int numZero;
};


int randomFromRange(int min, int max){
    
    time_t current_time = time(NULL);
    return (int)((current_time + rand()) % (max + 1 - min) + min);
}


struct BOARD *initBoard(struct BOARD *A){
    
    A = (struct BOARD *)malloc(sizeof(struct BOARD));
    
    A->board = (int **)malloc(sizeof(int*)*4);
    for(int i = 0; i < 4; i ++)
        A->board[i] = (int *)malloc(sizeof(int)*4);
    
    int first = randomFromRange(0, 15);
    int second = first;
    while(second == first)
        second = randomFromRange(0, 15);
    
    A->board[first/4][first%4] = 2;
    A->board[second/4][second%4] = 2;
    
    A->numZero = 14;
    
    return A;
}


void deleteBoard(struct BOARD *A){
    
    for(int i = 0; i < 4; i++)
        free(A->board[i]);
    free(A->board);
    free(A);
    return;
}


void printBoard(struct BOARD *A){
    
    printf("Current board is as follows\n");
    printf("===========================\n");
    
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++)
            if(A->board[i][j] == 0) printf("     ");
            else printf("%5.d", A->board[i][j]);
        printf("\n");
    }
    
    printf("===========================\n\n");
    return;
}


bool arrayEqual(int *a, int *b, int size){
    
    bool moved = false;
    for(int i = 0; i < size; i++){
        
        if(a[i] != b[i])
            moved = true;
        a[i] = b[i];
    }
    return moved;
}


void arrayReset(int *array, int a, int b, int c, int d){
    
    array[0] = a;
    array[1] = b;
    array[2] = c;
    array[3] = d;
    return;
}


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


void recountNumZero(struct BOARD *A){
    
    A->numZero = 0;
    
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(A->board[i][j] == 0)
                A->numZero++;
    
    return;
}


void updateBoard(struct BOARD *A, char direction){
    
    int array1[4] = {0,0,0,0};
    int array2[4] = {0,0,0,0};
    int array3[4] = {0,0,0,0};
    int array4[4] = {0,0,0,0};
    int old_array1[4] = {0,0,0,0};
    int old_array2[4] = {0,0,0,0};
    int old_array3[4] = {0,0,0,0};
    int old_array4[4] = {0,0,0,0};
    bool moved = false;
    
    switch(direction){
            
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
    
    recountNumZero(A);
    
    return;
}


bool endOfGame(struct BOARD *A){
    
    if(A->numZero > 0)
        return false;
    
    bool dead = true;
    for(int i = 0; i < 4; i++){
        
        for(int j = 0; j < 4; j++){
            
            // insert code here
            // may use updateBoard() 4 times
        }
    }
    
    return dead;
}



int main(int argc, const char * argv[]) {
    
    char direction = 'w';
    char empty;
    
    struct BOARD *A;
    
    A = initBoard(A);
    printBoard(A);
    
    printf("Please enter next direction: ");
    direction = getchar();
    while(direction == '\n' && direction != 'q'){
        
        printf("Please enter next direction: ");
        direction = getchar();
    }
    empty = getchar();
    
    while(direction != 'q' ){
        
        updateBoard(A, direction);
        printBoard(A);
        
        // break if dead
        if(endOfGame(A)){
            printf("Gameover!\n");
            break;
        }
        
        printf("Please enter next direction: ");
        direction = getchar();
        while(direction == '\n' && direction != 'q'){
            
            printf("Please enter next direction: ");
            direction = getchar();
        }
        empty = getchar();
    }
    
    printf("\n");
    deleteBoard(A);
    
    return 0;
}
