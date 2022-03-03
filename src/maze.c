#include "../inc/maze.h"

int main(){
    srand(time(NULL));
    Cell maze[SIZE][SIZE];
    clock_t begin = clock();
    initMaze(maze);
    randomizedAlgorithmOfPrim(maze);
    displayMaze(maze);
    printf("Maze generated in %ld ms", (clock() - begin) * 1000 / CLOCKS_PER_SEC);
    return EXIT_SUCCESS;
}

void initMaze(Grid maze){
    uint16_t j = 0;
    for (uint16_t i = 0; i < SIZE; i++){
        for (j = 0; j < SIZE; j++)
            maze[i][j] = (Cell) {.visited = false, .top = false, .right = false, .bottom = false, .left = false};
    }
}

void randomizedAlgorithmOfPrim(Grid maze){
    List neighbors = NULL;
    uint16_t x = rand() % SIZE , y = rand() % SIZE;
    maze[x][y].visited = true;
    uint16_t neighborsNumber = 0, cellVisited = 0; 
    neighbors = addNeighbors(maze, neighbors, &neighborsNumber, x, y, false);   // unvisited neighbors
    while (cellVisited < (SIZE * SIZE)){ 
        displayMaze(maze);
        usleep(10000); 
        printf("\e[1;1H\e[2J");
        seekRandomNeighbor(neighbors, neighborsNumber, &x, &y);
        maze[x][y].visited = true;
        openEdge(maze, x, y);
        neighbors = addNeighbors(maze, neighbors, &neighborsNumber, x, y, false);   // unvisited neighbors
        neighbors = delList(neighbors, (Coord) {x, y});
        neighborsNumber--;
        cellVisited++;
    }
    displayList(neighbors);
    freeList(neighbors);
}

List addNeighbors(Grid maze, List neighbors, uint16_t* lengthList, uint16_t x, uint16_t y, bool visitedStatus){
    if ((x - 1) >= 0 && maze[x - 1][y].visited == visitedStatus && !lookUpList(neighbors, x - 1, y)){  // top
        neighbors = addList(neighbors, (Coord) {x - 1, y});
        (*lengthList)++;
    }
    if ((x + 1) < SIZE && maze[x + 1][y].visited == visitedStatus && !lookUpList(neighbors, x + 1, y)){  // bottom
        neighbors = addList(neighbors, (Coord) {x + 1, y});
        (*lengthList)++;
    }
    if ((y - 1) >= 0 && maze[x][y - 1].visited == visitedStatus && !lookUpList(neighbors, x, y - 1)){  // left
        neighbors = addList(neighbors, (Coord) {x, y - 1});
        (*lengthList)++;
    }
    if ((y + 1) < SIZE && maze[x][y + 1].visited == visitedStatus && !lookUpList(neighbors, x, y + 1)){   // right
        neighbors = addList(neighbors, (Coord) {x, y + 1});
        (*lengthList)++;
    }
    return neighbors;
}

void seekRandomNeighbor(List neighbors, uint16_t length, uint16_t* x, uint16_t* y){
    uint16_t index = rand() % (length);
    for (; index > 0; neighbors = neighbors->next, index--)
        ;
    *x = neighbors->co.x;
    *y = neighbors->co.y;
}

void openEdge(Grid maze, uint16_t x, uint16_t y){
    List visitedNeighbors = NULL;
    uint16_t visitedNeighborsNumber = 0, xNeighbor = 0, yNeighbor = 0;
    visitedNeighbors = addNeighbors(maze, visitedNeighbors, &visitedNeighborsNumber, x, y, true);   // visited neighbors
    seekRandomNeighbor(visitedNeighbors, visitedNeighborsNumber, &xNeighbor, &yNeighbor);
    freeList(visitedNeighbors);
    if ((xNeighbor - 1) == x){
        maze[xNeighbor][yNeighbor].top = true;
        maze[x][y].bottom = true;
    }
    else if ((xNeighbor + 1) == x){
        maze[xNeighbor][yNeighbor].bottom = true;
        maze[x][y].top = true;
    }
    else if ((yNeighbor - 1) == y){
        maze[x][yNeighbor].left = true;
        maze[x][y].right = true;
    }
    else if ((yNeighbor + 1) == y){
        maze[xNeighbor][yNeighbor].right = true;
        maze[x][y].left = true;
    }
}

void displayMaze(Grid maze){
    uint16_t i, j;
    for (i = 0; i <= SIZE; i++){
        for (j = 0; j < SIZE; j++){
            if (i == SIZE)
                printf("\u200a\u203e");
            else {
                if (maze[i][j].visited)
                    printf("\e[1;32m");
                !maze[i][j].left ? printf("|"): printf(" ");
                !maze[i][j].top ? printf("\u203e"): printf(" ");
                if (j == (SIZE - 1))
                    printf("|"); 
            }
            printf("\e[0;37m");
        }
        printf("\n");
    }
    printf("\n");
}