#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>//to estimate the running time#include <stdio.h>

#include "graph_properties_from_file.h"
#include "graph.h"
#include "utils.h"
#include "degree_sort.h"
#include "core_decomposition.h"
#include "page_rank.h"

int main(int argc,char** argv){
    // Core decomposition
    char* filename = "../cleaned_instances/com-amazon.ungraph.txt";
    adjacency_array* g = read_adjacency_array(filename);
    unsigned long c = coreDecomposition(g);
    printf("Core decomposition : %lu\n",c);

    // Page rank
    char* num_filename = "../cleaned_instances/alr21--dirLinks--enwiki-20071018-num.txt";
    char* name_filename = "../instances_names/alr21--pageNum2Name--enwiki-20071018.txt";

    filename = "../cleaned_instances/alr21--dirLinks--enwiki-20071018.txt";

    char* deg_in_filename = "../outputs/in_degree.txt";
    char* deg_out_filename = "../outputs/out_degree.txt";
    char* pagerank_filename = "../outputs/pagerank_09.txt";

    directed_adjacency_array* h = read_directed_adjacency_array(filename);
    printf("Number of nodes: %lu\n",g->n);

    double* P = pageRank(h,0.9,80);

    printRanks(num_filename, name_filename, P, g->n, 5);

    write_indexed_double_array(pagerank_filename,P,g->n,false);

    free(P);

    // Calcul des degrés entrants et sortants
    unsigned long* in_degree = directed_node_degree(filename,h->n,h->m,false);
    write_indexed_int_array(deg_in_filename,in_degree,h->n,false);
    free(in_degree);

    unsigned long* out_degree = directed_node_degree(filename,h->n,h->m,true);
    write_indexed_int_array(deg_out_filename,out_degree,h->n,false);
    free(out_degree);

	return 0;
}
