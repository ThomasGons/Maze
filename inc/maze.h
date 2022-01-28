#ifndef __MAZE_H__
#define __MAZE_H__

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#define ALLOCATION_FAILURE -1

#define SIZE 5

typedef enum bool{
    false, true
}bool;

typedef struct Coord{
    uint16_t x, y;
}Coord;

typedef struct Node{
    Coord co;
    struct Node* next;
}Node, *List;


typedef struct Cell{
    bool visited;
    bool left, right, top, bottom;
}Cell, Grid[SIZE][SIZE];

/* PROTOTYPES */

// maze.c

void initMaze(Grid);
void randomizedAlgorithmOfPrim(Grid);
List addNeighbors(Grid, List, uint16_t*, uint16_t, uint16_t, bool);
void openEdge(Grid, uint16_t, uint16_t);
void displayMaze(Grid);

// list.c

List addList(List, Coord);
List delList(List, Coord);
List freeList(List);
bool lookUpList(List, uint16_t, uint16_t);

#endif
