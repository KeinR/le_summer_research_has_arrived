#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

// tree is the flattened array with all of the pairs
// of vertices that form the edges.
// treeLen is the length of this array - NOT the number
// of vertices or edges.
// vertices is the number of unique vertices
//
bool isConnected(int *graph, int graphLen, int vertices) {
    bool whitelist[BUFFER_SIZE] = {false};
    int whitelistCount = 0;

    // Yeah no
    if (graphLen < 2)
        return true;
    // Malformed tree - should be divisiable by
    // two if it's considted of ordered pairs
    if (graphLen % 2 != 0)
        return false;


    whitelist[graph[0]] = true;
    whitelist[graph[1]] = true;
    whitelistCount += graph[0] == graph[1] ? 1 : 2;
    graph += 2;
    graphLen -= 2;

    int wlChange = 0;
    do {
        wlChange = 0;
        for (int i = 0; i < graphLen / 2; i++) {
            int m = i * 2;
            int a = graph[m];
            int b = graph[m+1];
            if (a != b && whitelist[a] != whitelist[b]) {
                whitelist[a] = true;
                whitelist[b] = true;
                wlChange++;
            }
        }
        whitelistCount += wlChange;
    } while (wlChange > 0);

    return whitelistCount == vertices;
}

int main(int argc, char **argv) {
    int testGraph[] = {
        3, 7, 3, 9, 3, 2, 2, 0, 2, 1, 4, 10, 4, 11, 11, 12
    };
    printf("Graph connected: %i\n", isConnected(testGraph, sizeof(testGraph) / sizeof(int), 10));
    return 0;
}



