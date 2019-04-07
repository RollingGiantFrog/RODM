#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the running time#include <stdio.h>

#include <set>

using namespace std;

void clean_file(char* input, char* output, bool directed){
    FILE *file = fopen(input, "r");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return;
    }

    set< pair<unsigned int, unsigned int> > edges;


    char buffer[100];
    unsigned int lines = 0;
    fgets(buffer,100,file);
    bool beginFound = false;
    while (!beginFound) {
        while (buffer[0] == '#') {
            fgets(buffer,100,file);
            lines++;
        };
        beginFound = true;
        while (buffer[0] == '\n') {
            beginFound = false;
            fgets(buffer,100,file);
            lines++;
        };
    }

    rewind(file);
    unsigned int counter = 0;
    while (lines > counter) {
        fgets(buffer,100,file);
        counter++;
    }

    unsigned int u, v;
    unsigned long n = 0;
    while (fscanf(file,"%u %u", &u, &v)==2) {
        if (u > n) n = u+1;
        if (v > n) n = v+1;
    }
    printf("Number of nodes: %lu\n",n);

    rewind(file);

    counter = 0;
    while (lines > counter) {
        fgets(buffer,100,file);
        counter++;
    }

    unsigned long c = 0;
    if (directed) {
        while (fscanf(file,"%u %u", &u, &v)==2) {
            if (u != v && edges.find(make_pair(u,v)) == edges.end())
                edges.insert(make_pair(u,v));

            c++;
            if (c % 100000 == 0)
                printf("Edge iteration: %lu\n",c);
        }
    }
    else {
        while (fscanf(file,"%u %u", &u, &v)==2) {
            if (u > v) {
                int temp = u;
                u = v;
                v = temp;
            }
            if (u != v && edges.find(make_pair(u,v)) == edges.end())
                edges.insert(make_pair(u,v));

            c++;
            if (c % 100000 == 0)
                printf("Edge iteration: %lu\n",c);
        }
    }
    fclose(file);

    file = fopen(output, "w");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return;
    }

    c = 0;
    for (set< pair<unsigned int, unsigned int> >::iterator it = edges.begin(); it != edges.end(); ++it){
        fprintf(file,"%u %u\n",it->first,it->second);

        c++;
        if (c % 100000 == 0)
            printf("Write edge iteration: %lu\n",c);
    }

    fclose(file);
}

void clean_file_renumerotation(char* input, char* output, char* num_output, bool directed){
    FILE *file = fopen(input, "r");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return;
    }

    set< pair<unsigned int, unsigned int> > edges;


    char buffer[100];
    unsigned int lines = 0;
    fgets(buffer,100,file);
    bool beginFound = false;
    while (!beginFound) {
        while (buffer[0] == '#') {
            fgets(buffer,100,file);
            lines++;
        };
        beginFound = true;
        while (buffer[0] == '\n') {
            beginFound = false;
            fgets(buffer,100,file);
            lines++;
        };
    }

    rewind(file);
    unsigned int counter = 0;
    while (lines > counter) {
        fgets(buffer,100,file);
        counter++;
    }

    unsigned int u, v;
    unsigned long n = 0;
    unsigned long m = 0;
    while (fscanf(file,"%u %u", &u, &v)==2) {
        if (u > n) n = u+1;
        if (v > n) n = v+1;
        m++;
    }
    printf("Number of nodes: %lu\n",n);

    rewind(file);

    counter = 0;
    while (lines > counter) {
        fgets(buffer,100,file);
        counter++;
    }

    bool* isIn = (bool*) malloc(n*sizeof(bool));
    unsigned long* order = (unsigned long*) malloc(n*sizeof(unsigned long));
    unsigned long orderCount = 0;
    unsigned long i;
    for (i = 0; i < n; i++)
        isIn[i] = false;

    unsigned long c = 0;
    if (directed) {
        while (fscanf(file,"%u %u", &u, &v)==2) {
            if (u != v && edges.find(make_pair(u,v)) == edges.end()) {
                edges.insert(make_pair(u,v));
                if (!isIn[u]) {
                    isIn[u] = true;
                    order[u] = orderCount;
                    orderCount++;
                }
                if (!isIn[v]) {
                    isIn[v] = true;
                    order[v] = orderCount;
                    orderCount++;
                }

                c++;
                if (c % 100000 == 0)
                    printf("Edge iteration: %lu (max = %lu)\n",c,m);
            }
        }
    }
    else {
        while (fscanf(file,"%u %u", &u, &v)==2) {
            if (u > v) {
                int temp = u;
                u = v;
                v = temp;
            }
            if (u != v && edges.find(make_pair(u,v)) == edges.end()) {
                edges.insert(make_pair(u,v));
                if (!isIn[u]) {
                    isIn[u] = true;
                    order[u] = orderCount;
                    orderCount++;
                }
                if (!isIn[v]) {
                    isIn[v] = true;
                    order[v] = orderCount;
                    orderCount++;
                }

                c++;
                if (c % 100000 == 0)
                    printf("Edge iteration: %lu (max = %lu)\n",c,m);
            }
        }
    }
    fclose(file);

    file = fopen(output, "w");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return;
    }

    c = 0;
    for (set< pair<unsigned int, unsigned int> >::iterator it = edges.begin(); it != edges.end(); ++it){
        fprintf(file,"%lu %lu\n",order[it->first],order[it->second]);

        c++;
        if (c % 100000 == 0)
            printf("Write edge iteration: %lu (max = %lu)\n",c,m);
    }

    fclose(file);

    file = fopen(num_output, "w");

	if(file == NULL){
        printf("The file doesn't exist ! Please check again .");
        return;
    }

    c = 0;
    for (i = 0; i < n; i++) {
        if (isIn[i]) {
            fprintf(file,"%lu %lu\n",i,order[i]);
        }

        c++;
        if (c % 100000 == 0)
            printf("Write numerotation iteration: %lu (max = %lu)\n",c,n);
    }

    fclose(file);
}


int main(int argc,char** argv){
    //char* filename = "../instances/email-Eu-core.txt";
    //char* cleaned_filename = "../cleaned_instances/email-Eu-core.txt";
    //char* filename = "../instances/alr21--dirLinks--enwiki-20071018.txt";
    //char* cleaned_filename = "../cleaned_instances/alr21--dirLinks--enwiki-20071018.txt";
    //char* num_filename = "../cleaned_instances/alr21--dirLinks--enwiki-20071018-num.txt";

    char* filename = "../instances/com-amazon.ungraph.txt";
    char* cleaned_filename = "../cleaned_instances/com-amazon.ungraph.txt";
    char* num_filename = "../cleaned_instances/com-amazon.ungraph-num.txt";

    clean_file_renumerotation(filename,cleaned_filename,num_filename,true);

	return 0;
}
