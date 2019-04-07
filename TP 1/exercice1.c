#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time#include <stdio.h>

#include "graph_properties_from_file.h"
#include "graph.h"
#include "utils.h"


bool* BFS(adjacency_array* g, unsigned long s) {
    size_t sz = sizeof(unsigned long);
    unsigned long *fifo = malloc(sz*g->n);

    bool *visited = malloc(sizeof(bool)*g->n);
    unsigned long i;
    for (i = 0; i < g->n; i++)
        visited[i] = false;
    unsigned long u;

    fifo[0] = s;
    unsigned long begin = 0;
    unsigned long end = 0;
    unsigned long count = 1;
    visited[s] = true;

    while (count > 0) {
        u = fifo[begin];
        begin = begin + 1;
        count = count - 1;

        for (i = g->cd[u]; i < g->cd[u+1]; i++) {
            if (!visited[g->adj[i]]) {
                end = (end+1)%g->n;
                fifo[end] = g->adj[i];
                count = count + 1;

                visited[g->adj[i]] = true;
            }
        }
    }

    free(fifo);
    free(visited);

    return visited;
}

void maxdepth_BFS(adjacency_array* g, unsigned long s, unsigned long* furthest, unsigned long* maxdepth) {
    size_t sz = sizeof(unsigned long);
    unsigned long *fifo = malloc(sz*g->n);

    bool *visited = malloc(sizeof(bool)*g->n);
    unsigned long i;
    for (i = 0; i < g->n; i++)
        visited[i] = false;
    unsigned long u;

    fifo[0] = s;
    unsigned long begin = 0;
    unsigned long end = 0;
    unsigned long count = 1;
    visited[s] = true;

    (*maxdepth) = 0;
    (*furthest) = s;
    unsigned long *depth = malloc(sizeof(unsigned long)*g->n);
    for (i = 0; i < g->n; i++)
        depth[i] = 0;

    while (count > 0) {
        u = fifo[begin % g->n];
        begin = begin + 1;
        count = count - 1;

        for (i = g->cd[u]; i < g->cd[u+1]; i++) {
            if (!visited[g->adj[i]]) {
                end = (end+1)%g->n;
                count = count + 1;
                fifo[end] = g->adj[i];

                visited[g->adj[i]] = true;
                unsigned long d = depth[u] + 1;
                depth[g->adj[i]] = d;
                if (d > (*maxdepth)) {
                    (*maxdepth) = d;
                    (*furthest) = g->adj[i];
                }
            }
        }
    }

    free(fifo);
    free(visited);
}

unsigned long triangle_list(adjacency_array* g, unsigned long* count_triangles_per_node) {
    unsigned long count_triangles = 0;
    count_triangles_per_node = malloc(sizeof(unsigned long)*g->n);

    unsigned long u;
    for (u = 0; u < g->n; u++)
            count_triangles_per_node[u] = 0;

    for (u = 0; u < g->n; u++) {
        unsigned long firstNeighbor = g->cd[u];
        while (firstNeighbor < g->cd[u+1] && g->adj[firstNeighbor] < u) firstNeighbor++;

        unsigned long i;
        for (i = firstNeighbor; i < g->cd[u+1]; i++) {

            unsigned long v = g->adj[i];

            unsigned long idx_u = i+1;
            unsigned long idx_v = g->cd[v];
            while (idx_v < g->cd[v+1] && g->adj[idx_v] < v) idx_v++;
            if (idx_v == g->cd[v+1]) continue;

            while (idx_u < g->cd[u+1] && idx_v < g->cd[v+1]) {
                while (g->adj[idx_u] != g->adj[idx_v]) {
                    while (idx_u < g->cd[u+1] && g->adj[idx_u] < g->adj[idx_v])
                        idx_u++;

                    if (idx_u == g->cd[u+1]) break;

                    while (idx_v < g->cd[v+1] && g->adj[idx_v] < g->adj[idx_u])
                        idx_v++;

                    if (idx_v == g->cd[v+1]) break;
                }

                if (idx_u < g->cd[u+1] && idx_v < g->cd[v+1]) {
                    //printf("Triangle %lu %lu %lu\n",u,v,g->adj[idx_u]);
                    count_triangles++;
                    count_triangles_per_node[u]++;
                    count_triangles_per_node[v]++;
                    count_triangles_per_node[g->adj[idx_u]]++;
                }

                idx_u++;
                idx_v++;
            }
        }
    }

    return count_triangles;
}

int main(int argc,char** argv){
    char* filename = "../cleaned_instances/com-amazon.ungraph.txt";
    char* degree_filename = "../outputs/degree/com-amazon.ungraph_deg.txt";
    char* distribution_filename = "../outputs/distribution/com-amazon.ungraph_distrib.txt";

    // On récupère la taille du graphe (nombre de noeuds et d'arêtes)
    unsigned long nb_nodes, nb_edges;
    graph_size(filename, &nb_nodes, &nb_edges);

	printf("Number of nodes : %lu\n", nb_nodes);
	printf("Number of edges : %lu\n", nb_edges);

    // On calcule le degré de chaque noeud
	unsigned long *deg = node_degree(filename,nb_nodes,nb_edges);
	write_indexed_int_array(degree_filename,deg,nb_nodes,true);

    // On calcule la quantité Q_G
    unsigned long Q = prod_degrees(filename,degree_filename,nb_nodes);

	printf("Q_G : %lu\n", Q);

    // On calcule le degré de chaque noeud
	degree_distribution(degree_filename,distribution_filename,nb_edges);

    // On charge l'adjacency array
    adjacency_array* g = read_adjacency_array(filename);

    // On calcule les propriétés sur les composantes
    unsigned long countcomponents = 0;
    unsigned long maxcomponent = 0;
    bool *visited = malloc(nb_nodes*sizeof(bool));
    unsigned long u;
    for (u = 0; u < nb_nodes; u++)
            visited[u] = false;

    for (u = 0; u < nb_nodes; u++){
        if (!visited[u]) {
            bool *component = BFS(g,u);
            countcomponents = countcomponents + 1;

            unsigned long v;
            unsigned long componentsize = 0;
            for (v = 0; v < nb_nodes; v++){
                if (component[v]) {
                    visited[v] = true;
                    componentsize = componentsize + 1;
                }
            }

            if (componentsize > maxcomponent) maxcomponent = componentsize;
        }
    }

    printf("Number of components : %lu\n",countcomponents);
    printf("Max component size : %lu\n",maxcomponent);

    // On calcule une borne sur le diamiètre
    unsigned long depth;
    unsigned long furthest;
    unsigned long maxdepth = 0;

    unsigned long start = 500;

    unsigned int i;
    for (i = 0; i < 20; i++) {
        maxdepth_BFS(g,start,&furthest,&depth);
        start = furthest;
        if (depth > maxdepth) maxdepth = depth;
    }

    printf("Diameter (lower bound) : %lu\n",maxdepth);

    // On calcule les propriétés sur la triangularité
    unsigned long* count_triangles_per_node;
    unsigned long count_triangles = triangle_list(g,count_triangles_per_node);
    printf("Number of triangles : %lu\n",count_triangles);

    unsigned long edge_couples = 0;
    for (i = 0; i < nb_nodes; i++)
        edge_couples = edge_couples + deg[i]*(deg[i]-1);

    float transitivity_ratio = 3*(float)(count_triangles)/(edge_couples);
    printf("Transitivity ratio : %f\n",transitivity_ratio);

	return 0;
}
