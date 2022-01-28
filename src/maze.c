#include "../inc/maze.h"

int main(){
    srand(time(NULL));
    static Cell maze[SIZE][SIZE];
    clock_t begin = clock();
    initMaze(maze);
    randomizedAlgorithmOfPrim(maze);
    displayMaze(maze);
    printf("Maze generated in %ld ms.\n\n", (clock() - begin) * 1000 / CLOCKS_PER_SEC);
    return EXIT_SUCCESS;
}

void initMaze(Grid maze){
    uint16_t j = 0;
    for (uint16_t i = 0; i < SIZE; i++){
        for (j = 0; j < SIZE; j++)
            // initialize the maze with only walls
            maze[i][j] = (Cell) {.visited = false, .left = false, .right = false, .top = false, .bottom = false};       
    }
}

void randomizedAlgorithmOfPrim(Grid maze){
    /* nbr means neighbors 
       rdm means random */
    List nbr = NULL, tmp = NULL;
    Coord rdmNbr;
    uint16_t i = 0, x = rand() % (SIZE + 1), y = rand() % (SIZE + 1);
    // put the first visited cell in the maze randomly
    printf("xi = %u, yi = %u\n", x, y);
    maze[x][y].visited = true;
    uint16_t rdm, lengthNbrList = 0, cellVisited = 1, cellVisitable = SIZE * SIZE;
    // add neighbors of the init cell
    nbr = addNeighbors(maze, nbr, &lengthNbrList, x, y, false);
    // stop only if all visitable cells have been visited
    while (cellVisited < cellVisitable){
        // choose a random cell among the neighbors list
        rdm = rand() % lengthNbrList;
        // seek the cell at rdm index
        for (i = 0, tmp = nbr; i < rdm; i++, tmp = tmp->next)
            ;
        rdmNbr = tmp->co;
        maze[rdmNbr.x][rdmNbr.y].visited = true;
        // remove an edge between the random cell and a neighboring visited cell 
        openEdge(maze, rdmNbr.x, rdmNbr.y);
        // add the neighbors of this cell
        nbr = addNeighbors(maze, nbr, &lengthNbrList, rdmNbr.x , rdmNbr.y, false);
        // remove the cell from the neighbors list
        nbr = delList(nbr, rdmNbr);
        lengthNbrList--;
        displayMaze(maze);
        cellVisited++;
    }
}

List addNeighbors(Grid maze, List nbr, uint16_t *lengthList, uint16_t x, uint16_t y, bool visitedStatus){
    // add neighbor to the list only if it is in the maze
    if (x - 1 >= 0 && maze[x - 1][y].visited == visitedStatus && !lookUpList(nbr,  x - 1, y)){
        nbr = addList(nbr, (Coord) {x - 1, y});
        (*lengthList)++;
    }
    if (x + 1 < SIZE && maze[x + 1][y].visited == visitedStatus && !lookUpList(nbr, x + 1, y)){
        nbr = addList(nbr, (Coord) {x + 1, y});
        (*lengthList)++;
    }
    if (y - 1 >= 0 && maze[x][y - 1].visited == visitedStatus && !lookUpList(nbr, x, y - 1)){
        nbr = addList(nbr, (Coord) {x, y - 1});
        (*lengthList)++;
    }
    if (y + 1 < SIZE && maze[x][y + 1].visited == visitedStatus && !lookUpList(nbr, x, y + 1)){
        nbr = addList(nbr, (Coord) {x, y + 1});
        (*lengthList)++;
    }
    return nbr;
}

void openEdge(Grid maze, uint16_t x, uint16_t y){
    /* tmpNbr contains the neighbors of the random cell chosen from the neighbors list
       psgNbr contains the visited cell in tmpNbr list */
    List tmpNbr = NULL, psgNbr = NULL, tmp = NULL;
    uint16_t tmpLengthList = 0, psgLengthList = 0;
    tmpNbr = addNeighbors(maze, tmpNbr, &tmpLengthList, x, y, true);
    for (tmp = tmpNbr; tmp; tmp = tmp->next){
        if (maze[tmp->co.x][tmp->co.y].visited){
            psgNbr = addList(psgNbr, tmp->co);
            psgLengthList++;
        }
    }
    tmpNbr = freeList(tmpNbr);
    // Random choice of a visited neighbor of the given cell to connect them
    Coord rdmPsg = {0, 0};
    tmp = psgNbr;
    for (short i = 0, rdm = rand() % psgLengthList; i < rdm; i++, tmp = tmp->next)
        ;
    rdmPsg = (Coord) {tmp->co.x, tmp->co.y};
    psgNbr = freeList(psgNbr);
    // Connect cells by removing edge according to the position of the visited neighbor next to the cell
    if (rdmPsg.x - 1 == x){
        printf("right: {%u, %u} (VNbr); left: {%u, %u} (RNbr)\n", rdmPsg.x, rdmPsg.y, x, y);
        maze[rdmPsg.x][rdmPsg.y].left = true;
        maze[x][y].right = true;
    }
    else if (rdmPsg.x + 1 == x){
        printf("left: {%u, %u} (VNbr); right: {%u, %u} (RNbr)\n", rdmPsg.x, rdmPsg.y, x, y);
        maze[rdmPsg.x][rdmPsg.y].right = true;
        maze[x][y].left = true;
    }
    else if (rdmPsg.y - 1 == y){
        printf("top: {%u, %u} (VNbr); bottom: {%u, %u} (RNbr)\n", rdmPsg.x, rdmPsg.y, x, y);
        maze[rdmPsg.x][rdmPsg.y].top = true;
        maze[x][y].bottom = true;
    }
    else if (rdmPsg.y + 1 == y){
        printf("bottom: {%u, %u} (VNbr); top: {%u, %u} (RNbr)\n", rdmPsg.x, rdmPsg.y, x, y);
        maze[rdmPsg.x][rdmPsg.y].bottom = true;
        maze[x][y].top = true;
    }
}

void displayMaze(Grid maze){
    short i, j;
    for (i = 0; i <= SIZE; i++){
        for (j = 0; j < SIZE; j++){
            if (i == SIZE) printf("\u200a\u203e");
            else {
                /* display edges if they exist
                   only left and top to avoid repetition between lines and between columns */
                !maze[i][j].left ? printf("|"): printf(" ");
                !maze[i][j].top ? printf("\u203e"): printf(" ");
                j == (SIZE - 1) ? printf("|"): 0;
            }
        }
        printf("\n");
    }
    printf("\n");
}
