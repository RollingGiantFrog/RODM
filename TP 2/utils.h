#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/* Implémente des fonctions d'écriture de tableaux */

void write_indexed_int_array(char* output, unsigned long* array, unsigned int n, bool filter_zeros);
void write_indexed_double_array(char* output, double* array, unsigned int n, bool filter_zeros);

void write_int_array(char* output, unsigned long* array, unsigned int n);
void write_double_array(char* output, double* array, unsigned int n);

#endif // UTILS_H_INCLUDED
