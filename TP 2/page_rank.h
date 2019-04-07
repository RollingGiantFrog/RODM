#ifndef PAGE_RANK_H_INCLUDED
#define PAGE_RANK_H_INCLUDED

/* Implémente l'algorithme de pageRank */

double* directedGraphProd(directed_adjacency_array* graph, double* A);
double* pageRank(directed_adjacency_array* graph, double alpha, unsigned long t);
void printRanks(char* num_filename, char* name_filename, double* P, unsigned long n, unsigned int ranks);

#endif // PAGE_RANK_H_INCLUDED
