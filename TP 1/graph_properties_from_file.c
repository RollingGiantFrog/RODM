#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "graph_properties_from_file.h"

void graph_size(char* input, unsigned long* nb_nodes, unsigned long* nb_edges){
	FILE *file = fopen(input, "r");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return;
    }

    unsigned long u, v;

	(*nb_nodes) = 0;
    (*nb_edges) = 0;
	while (fscanf(file,"%lu %lu", &u, &v)==2){
		if (u > (*nb_nodes)) (*nb_nodes) = u;
		if (v > (*nb_nodes)) (*nb_nodes) = v;
		(*nb_edges)++;
	}
	(*nb_nodes) = (*nb_nodes) + 1;

	fclose(file);
}

unsigned long* node_degree(char* input, unsigned long nb_nodes, unsigned long nb_edges){
	FILE *file = fopen(input,"r");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return NULL;
    }

    unsigned long u, v;
	unsigned long* deg = malloc(nb_nodes*sizeof(unsigned long));

	unsigned long i;
    for(i = 0; i < nb_nodes; i++)
        deg[i] = 0;

	while (fscanf(file,"%lu %lu", &u, &v)==2) {
		deg[u] = deg[u] + 1;
		deg[v] = deg[v] + 1;
	}
	fclose(file);

    return deg;
}

unsigned int prod_degrees(char* input, char* deg_input, unsigned long nb_nodes){
    FILE *file = fopen(deg_input,"r");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return 0;
    }

    unsigned long i;
    unsigned long d;
    unsigned long* deg = malloc(nb_nodes*sizeof(unsigned long));
    while (fscanf(file,"%lu %lu", &i, &d)==2)
        deg[i] = d;

    fclose(file);

    file = fopen(input,"r");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return 0;
    }

    unsigned long Q = 0;
    unsigned long u, v;
    while (fscanf(file,"%lu %lu", &u, &v)==2)
        Q = Q + deg[u]*deg[v];

    free(deg);

    return Q;
}

void degree_distribution(char* deg_input, char* output, unsigned long nb_edges){
    FILE *file = fopen(deg_input,"r");

    if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return;
    }

    unsigned long* distribution = malloc(nb_edges*sizeof(unsigned long));
    unsigned long i;
    unsigned long d;

    for (d = 0; d < nb_edges; d++)
        distribution[d] = 0;

    while (fscanf(file,"%lu %lu", &i, &d)==2)
        distribution[d] = distribution[d] + 1;

    file = fopen(output,"w");
    for (d = 0; d < nb_edges; d++) {
        if (distribution[d] > 0)
            fprintf(file, "%lu %lu\n", d, distribution[d]);
    }
    fclose(file);

    free(distribution);
}
