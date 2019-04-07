#ifndef GRAPH_PROPERTIES_FROM_FILE_H_INCLUDED
#define GRAPH_PROPERTIES_FROM_FILE_H_INCLUDED

/* Implémente des fonctions de lecture de graphes dans un fichier */

void graph_size(char* input, unsigned long* nb_nodes, unsigned long* nb_edges);
unsigned long* node_degree(char* input, unsigned long nb_nodes, unsigned long nb_edges);
unsigned long* directed_node_degree(char* input, unsigned long nb_nodes, unsigned long nb_edges, bool out_degree);
unsigned int prod_degrees(char* input, char* deg_input, unsigned long nb_nodes);
void degree_distribution(char* deg_input, char* output, unsigned long nb_edges);

#endif // GRAPH_PROPERTIES_FROM_FILE_H_INCLUDED
