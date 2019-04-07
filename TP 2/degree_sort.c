#include "degree_sort.h"
#include <stdlib.h>

NodeArray createNodeArray(){
    NodeArray array;
    array.p = malloc(0);
    array.size = 0;
    return array;
}

unsigned long getBackNode(NodeArray* array){
    return array->p[array->size-1];
}

void addNode(NodeArray* array, unsigned long node){
    array->p = realloc(array->p,(array->size+1)*sizeof(unsigned long));
    array->p[array->size] = node;
    array->size++;
}

unsigned long popNode(NodeArray* array){
    unsigned long node = getBackNode(array);
    array->size--;
    array->p = realloc(array->p,array->size*sizeof(unsigned long));
    return node;
}

void removeNode(NodeArray* array, unsigned long idx){
    array->p[idx] = getBackNode(array);
    popNode(array);
}

