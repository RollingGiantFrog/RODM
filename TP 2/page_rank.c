#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"

double* directedGraphProd(directed_adjacency_array* graph, double* A){
    double* B = malloc(graph->n*sizeof(double));
    unsigned long u = 0;
    for (u = 0; u < graph->n; u++)
        B[u] = 0.;

    for (u = 0; u < graph->n; u++) {
        if (graph->cd[u+1] > graph->cd[u]){
            unsigned long j;

            for (j = graph->cd[u]; j < graph->cd[u+1]; j++) {
                unsigned long v = graph->adj[j];
                B[v] += A[u]/(graph->cd[u+1] - graph->cd[u]);
            }
        }
    }

    return B;
}

double* pageRank(directed_adjacency_array* graph, double alpha, unsigned long t){
    double* P = malloc(graph->n*sizeof(double));
    double* oldP = malloc(graph->n*sizeof(double));
    unsigned long i;
    unsigned long k;
    double p0 = 1./graph->n;
    for (i = 0; i < graph->n; i++)
        P[i] = p0;

    double alpha_n = alpha/graph->n;

    for (k = 0; k < t; k++){
        for (i = 0; i < graph->n; i++)
            oldP[i] = P[i];

        P = directedGraphProd(graph,P);

        double normP = 0.;
        for (i = 0; i < graph->n; i++) {
            P[i] = (1.-alpha)*P[i] + alpha_n;
            normP += P[i];
        }

        double normP_n = (1.-normP)/graph->n;
        double normdP = 0;
        for (i = 0; i < graph->n; i++) {
            P[i] += normP_n;
            if (P[i] > oldP[i])
                normdP += (P[i]-oldP[i]);
            else
                normdP += (oldP[i]-P[i]);
        }

        printf("%lu,%f\n",k,graph->n*normdP);
    }

    free(oldP);

    return P;
}


double* Pcomp;
int Pcomparator(const void *p, const void *q) {
    unsigned long l = *(const unsigned long *)p;
    unsigned long r = *(const unsigned long *)q;

    if (Pcomp[l] < Pcomp[r])
        return -1;
    else if (Pcomp[l] > Pcomp[r])
        return 1;
    else
        return 0;
}

void printRanks(char* num_filename, char* name_filename, double* P, unsigned nb_nodes, unsigned int ranks){

    // Setting up numerotation (because the graph file has been renumeroted when cleaned)
    FILE *file = fopen(num_filename, "r");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return;
    }

    char buffer[100];
    int lines = 0;
    fgets(buffer,100,file);
    while (buffer[0] == '#') {
        fgets(buffer,100,file);
        lines = lines + 1;
    };
    rewind(file);
    while (lines > 0) {
        fgets(buffer,100,file);
        lines = lines - 1;
    }
    fgets(buffer,100,file);

    unsigned long* node = malloc(nb_nodes*sizeof(unsigned long));

    unsigned long n = 0;
    unsigned long u, num_u;
    while (fscanf(file,"%lu %lu", &u, &num_u)==2){
        if (u > n) n = u;
        node[num_u] = u;
    }
    n = n + 1;

    fclose(file);

    printf("Numerotation has been setup.\n");

    // Sorting page ranks
    unsigned long i;
    Pcomp = malloc(nb_nodes*sizeof(double));
    for (i = 0; i < nb_nodes; i++){
        Pcomp[i] = P[i];
    }

    unsigned long* atRank = malloc(nb_nodes*sizeof(unsigned long));
    for (i = 0; i < nb_nodes; i++){
        atRank[i] = i;
    }

    qsort(atRank,nb_nodes,sizeof(unsigned long),Pcomparator);

    // Reading names
    file = fopen(name_filename, "r");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return;
    }

    lines = 0;
    fgets(buffer,100,file);
    while (buffer[0] == '#') {
        fgets(buffer,100,file);
        lines = lines + 1;
    };

    rewind(file);
    while (lines > 0) {
        fgets(buffer,100,file);
        lines = lines - 1;
    }
    fgets(buffer,100,file);

    char names[ranks*2][256];
    char s[256];
    unsigned int c;
    unsigned long num;
    while (fgets(s,256,file) != NULL) {
        c = 0;
        num = 0;
        while (s[c] >= '0' && s[c] <= '9') {
            num = 10*num + s[c]-'0';
            c++;
        }
        c++;

        for (i = 0; i < ranks; i++)
            if (node[atRank[i]] == num) {
                strcpy(names[i],s+c);
            }

        for (i = nb_nodes-ranks; i < nb_nodes; i++)
            if (node[atRank[i]] == num){
                strcpy(names[ranks + i - (nb_nodes - ranks)],s+c);
            }
    }

    for (i = 0; i < ranks; i++){
        printf("Rank %lu = page no %lu\n",i,atRank[i]);
        printf("%s",names[i]);
    }
    printf("\n");

    for (i = nb_nodes-ranks; i < nb_nodes; i++){
        printf("Rank %lu = page no %lu\n",i,atRank[i]);
        printf("%s",names[ranks + i - (nb_nodes - ranks)]);
    }

    fclose(file);

    free(Pcomp);
    free(atRank);
    free(names);
    free(node);
}
