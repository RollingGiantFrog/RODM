#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "utils.h"

void write_indexed_int_array(char* output, unsigned long* array, unsigned int n, bool filter_zeros) {
    FILE *file = fopen(output,"w");
    unsigned long i;
    for (i = 0; i < n; i++) {
        if (!filter_zeros || array[i] > 0)
            fprintf(file, "%lu %lu\n", i, array[i]);
    }
    fclose(file);
}

void write_indexed_double_array(char* output, double* array, unsigned int n, bool filter_zeros) {
    FILE *file = fopen(output,"w");
    unsigned long i;
    for (i = 0; i < n; i++) {
        if (!filter_zeros || array[i] > 0)
            fprintf(file, "%lu %f\n", i, array[i]);
    }
    fclose(file);
}

void write_int_array(char* output, unsigned long* array, unsigned int n) {
    FILE *file = fopen(output,"w");
    unsigned long i;
    for (i = 0; i < n; i++) {
        fprintf(file, "%lu\n", array[i]);
    }
    fclose(file);
}

void write_double_array(char* output, double* array, unsigned int n) {
    FILE *file = fopen(output,"w");
    unsigned long i;
    for (i = 0; i < n; i++) {
        fprintf(file, "%f\n", array[i]);
    }
    fclose(file);
}
