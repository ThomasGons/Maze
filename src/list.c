#include "../inc/maze.h"

List addList(List li, Coord elm){
    Node* new = malloc(sizeof *new);
    if (!new){
        fprintf(stderr, "Allocation failure\n");
        exit(ALLOCATION_FAILURE);
    }
    *new = (Node) { elm, NULL};
    if (!li){
        return new;
    }
    List tmp = li;
    while (tmp->next){
        tmp = tmp->next;
    }
    tmp->next = new;
    return li;
}

List delList(List li, Coord elm){
    if (!li) return NULL;
    List tmp = li, tmpB = NULL;
    while (tmp->next){
        if (tmp->co.x == elm.x && tmp->co.y == elm.y){
            if (!tmpB) return li->next;
            tmpB->next = tmp->next;
            free(tmp);
            break;
        }
        tmpB = tmp;
        tmp = tmp->next;
    }
    return li;
    
}

List freeList(List li){
    List tmp = NULL;
    while (li){
        tmp = li;
        li = li->next;
        free(tmp);
    }
    return li;
}

bool lookUpList(List li, uint16_t x, uint16_t y){
    if (!li) return false;
    for (; li->next; li = li->next)
        if (li->co.x == x && li->co.y == y) return true;
    return false;
}