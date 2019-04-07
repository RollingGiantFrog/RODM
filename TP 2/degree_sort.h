#ifndef DEGREE_SORT_H_INCLUDED
#define DEGREE_SORT_H_INCLUDED

/* Implémente une structure de tri très efficace (O(1)) pour maintenir les noeuds triés par leurs degrés */

typedef struct{
    unsigned long size;
    unsigned long* p;
} NodeArray;

NodeArray createNodeArray();
unsigned long getBackNode(NodeArray* array);
void addNode(NodeArray* array, unsigned long node);
unsigned long popNode(NodeArray* array);
void removeNode(NodeArray* array, unsigned long idx);



#endif // DEGREE_SORT_H_INCLUDED
