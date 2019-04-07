#include "degree_sort.h"

#include "core_decomposition.h"

unsigned long coreDecomposition(adjacency_array* graph){
    unsigned long n = graph->n;
    unsigned long i;
    unsigned long j;
    unsigned long d;
    unsigned long u;
    unsigned long v;
    unsigned long back;

    unsigned long* deg = malloc(graph->n*sizeof(unsigned long));
    unsigned long deg_max = 0;
    for (u = 0; u < graph->n; u++){
        deg[u] = graph->cd[u+1]-graph->cd[u];
        if (deg[u] > deg_max) deg_max = deg[u];
    }


    NodeArray nodesOfDegree[deg_max+1];
    unsigned long* nodeIndex = malloc(graph->n*sizeof(unsigned long));
    bool* isInGraph = malloc(graph->n*sizeof(bool));


    for (d = 0; d <= deg_max; d++){
        nodesOfDegree[d] = createNodeArray();
    }

    for (u = 0; u < graph->n; u++){
        isInGraph[u] = true;
    }

    for (u = 0; u < graph->n; u++){
        d = deg[u];
        nodeIndex[u] = nodesOfDegree[d].size;
        addNode(&nodesOfDegree[d],u);
    }

    unsigned long c_max = 0;
    unsigned long* c = malloc(graph->n*sizeof(unsigned long));
    unsigned long* o = malloc(graph->n*sizeof(unsigned long));

    while (n > 0) {
        d = 0;
        while (nodesOfDegree[d].size == 0)
            d++;

        u = popNode(&nodesOfDegree[d]);

        isInGraph[u] = false;

        if (d > c_max) c_max = d;
        c[u] = c_max;
        o[u] = n;

        for (i = graph->cd[u]; i < graph->cd[u+1]; i++) {
            v = graph->adj[i];
            if (isInGraph[v]) {
                d = deg[v];
                j = nodeIndex[v];
                back = getBackNode(&nodesOfDegree[d]);
                nodeIndex[back] = j;

                removeNode(&nodesOfDegree[d],j);

                nodeIndex[v] = nodesOfDegree[d-1].size;
                addNode(&nodesOfDegree[d-1],v);

                deg[v]--;
            }
        }

        n--;
    }

    return c_max;
}
