#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "graph_properties_from_file.h"
#include "graph.h"

edge_list* read_edge_list(char* input){
	unsigned long nb_nodes, nb_edges;
	graph_size(input,&nb_nodes,&nb_edges);

	FILE *file = fopen(input,"r");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return NULL;
    }

	edge_list *g = malloc(sizeof(edge_list));
	g->n = nb_nodes;
	g->e = nb_edges;
	g->edges = malloc(nb_edges*sizeof(edge));

	unsigned long e = 0;
	while (fscanf(file,"%lu %lu", &(g->edges[e].s), &(g->edges[e].t))==2)
		e++;

	fclose(file);

	return g;
}

void free_edge_list(edge_list *g){
	free(g->edges);
	free(g);
}

adjacency_matrix* read_adjacency_matrix(char* input){

	unsigned long nb_nodes, nb_edges;
	graph_size(input,&nb_nodes,&nb_edges);

	adjacency_matrix *g = malloc(sizeof(adjacency_matrix));
	g->n = nb_nodes;
	g->e = nb_edges;
	g->adj = malloc((nb_nodes*nb_nodes)*sizeof(bool));

    FILE *file = fopen(input,"r");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return NULL;
    }

    unsigned long s, t;
    while (fscanf(file,"%lu %lu", &s, &t)==2)
        g->adj[s*nb_nodes + t] = true;

    fclose(file);

    return g;
}

void free_adjacency_matrix(adjacency_matrix *g){
	free(g->adj);
	free(g);
}

// comparator for neighbors
int comparator(const void *p, const void *q) {
    unsigned long l = *(const unsigned long *)p;
    unsigned long r = *(const unsigned long *)q;

    if (l < r)
        return -1;
    else if (l > r)
        return 1;
    else
        return 0;
}

adjacency_array* read_adjacency_array(char* input){
	unsigned long nb_nodes, nb_edges;
	graph_size(input,&nb_nodes,&nb_edges);

    unsigned long* deg = node_degree(input,nb_nodes,nb_edges);

	adjacency_array *g = malloc(sizeof(adjacency_array));
	g->n = nb_nodes;
	g->e = nb_edges;
	g->cd = malloc((nb_nodes+1)*sizeof(unsigned long));
	g->adj = malloc((2*nb_edges)*sizeof(unsigned long));

    FILE *file = fopen(input,"r");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again.");
        return NULL;
    }

    unsigned long i;
    g->cd[0] = 0;
    for (i = 1; i <= nb_nodes; i++)
        g->cd[i] = g->cd[i-1] + deg[i-1];

    unsigned long* count = malloc(nb_nodes*sizeof(unsigned long));
    for (i = 0; i < nb_nodes; i++)
        count[i] = 0;

    unsigned long s, t;
    while (fscanf(file,"%lu %lu", &s, &t)==2){
        g->adj[g->cd[s] + count[s]] = t;
        count[s]++;

        g->adj[g->cd[t] + count[t]] = s;
        count[t]++;
    }

    fclose(file);

    for (i = 0; i < nb_nodes; i++) {
        qsort(g->adj + g->cd[i],deg[i],sizeof(unsigned long),comparator);
    }

    free(deg);
    free(count);

    return g;
}

void free_adjacency_array(adjacency_array *g){
	free(g->cd);
	free(g->adj);
	free(g);
}

directed_adjacency_array* read_directed_adjacency_array(char* input){
	unsigned long nb_nodes, nb_edges;
	graph_size(input,&nb_nodes,&nb_edges);

    unsigned long* deg_out = directed_node_degree(input,nb_nodes,nb_edges,true);

	directed_adjacency_array *g = malloc(sizeof(directed_adjacency_array));
	g->n = nb_nodes;
	g->m = nb_edges;
	g->cd = malloc((nb_nodes+1)*sizeof(unsigned long));
	g->adj = malloc((2*nb_edges)*sizeof(unsigned long));

    FILE *file = fopen(input, "r");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return NULL;
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

    unsigned long i;
    g->cd[0] = 0;
    for (i = 1; i <= nb_nodes; i++)
        g->cd[i] = g->cd[i-1] + deg_out[i-1];

    unsigned long* count = malloc(nb_nodes*sizeof(unsigned long));
    for (i = 0; i < nb_nodes; i++)
        count[i] = 0;

    unsigned long s, t;
    while (fscanf(file,"%lu %lu", &s, &t)==2){
        g->adj[g->cd[s] + count[s]] = t;
        count[s]++;
    }

    fclose(file);

    for (i = 0; i < nb_nodes; i++) {
        qsort(g->adj + g->cd[i],deg_out[i],sizeof(unsigned long),comparator);
    }

    return g;
}

void free_directed_adjacency_array(directed_adjacency_array *g){
	free(g->cd);
	free(g->adj);
	free(g);
}
