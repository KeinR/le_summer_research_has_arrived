#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

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

bool isTree(int *graph, int len, int vertices) {
    // Can't have loops if it's minimally connected, also
    // Trees are connected. And so yeah.
    return len / 2 == vertices - 1 && isConnected(graph, len, vertices);
}

// Output allocated size is assumed to be BUFFER_SIZE
// Input assumed to be a tree
void treeCodeList(int *tree, int len, int vertices, int *output, int *outputLen) {
    typedef struct {
        int path;
        int vertex;
    } pathStruc;
    // Maps vertices to a list of vertices that that
    // vertex is linked to
    // The first index of each list is the length
    // (and everything is initialized to zero, so...)
    int map[BUFFER_SIZE][BUFFER_SIZE] = {0};
    int vertVals[BUFFER_SIZE];
    pathStruc path[BUFFER_SIZE];
    int pathLen = 0;
    int numVerts = 0;

    *outputLen = 0;

    for (int i = 0; i < len / 2; i++) {
        int m = i * 2;
        int f = tree[m];
        int s = tree[m+1];
        // The length of each adjacency list,
        // stored at the beginning
        int *plf = &map[f][0];
        int *pls = &map[s][0];
        if (*plf == 0) {
            vertVals[numVerts++] = f;
        }
        if (*pls == 0) {
            vertVals[numVerts++] = s;
        }
        map[f][(*plf)++ + 1] = s;
        map[s][(*pls)++ + 1] = f;
    }

    assert(numVerts == vertices);

    for (int i = 0; i < vertices; i++) {
        int v = vertVals[i];
        if (map[v][0] == 1) {
            pathLen = 1;
            path[0].vertex = v;
            path[0].path = 0;
            while (path[0].path < 1) {
                int ver = path[pathLen - 1].vertex;
                int p = path[pathLen - 1].path;
                int newVert = map[ver][p + 1];
                int degree = map[newVert][0];
                if (degree == 1 || (pathLen >= 2 && path[pathLen - 2].vertex == newVert)) {
                    if (pathLen < 2 || path[pathLen - 2].vertex != newVert) {
                        output[(*outputLen)++] = pathLen;
                    }
                    // Increment the path
                    path[pathLen - 1].path++;
                    // i > 0 deliberate, need one look behind.
                    // Cut off all the nodes that have finished their search
                    for (int i = pathLen - 1; i > 0 && path[i].path >= map[path[i].vertex][0]; i--) {
                        path[i-1].path++;
                        pathLen--;
                    }
                } else {
                    path[pathLen].vertex = newVert;
                    path[pathLen].path = 0;
                    pathLen++;
                }
            }
        }
    }
}

int treeCode(int *tree, int len, int vertices) {
    int codes[BUFFER_SIZE];
    int codesLen;
    // https://oeis.org/A000040
    int primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,
        61,67,71,73,79,83,89,97,101,103,107,109,113,127,
        131,137,139,149,151,157,163,167,173,179,181,191,
        193,197,199,211,223,227,229,233,239,241,251,257,
        263,269,271
    };
    int counts[sizeof(primes) / sizeof(int)] = {0};
    int numPrimes = sizeof(primes) / sizeof(int);
    int result = 0;

    treeCodeList(tree, len, vertices, codes, &codesLen);
    for (int i = 0; i < codesLen; i++) {
        int c = codes[i];
        if (c >= numPrimes) {
            printf("Buddy that's breaking the prime gap\n");
            return -1;
        }
        counts[c]++;
    }
    for (int i = 0; i < numPrimes; i++) {
        if (counts[i] != 0) {
            result += pow(primes[i], counts[i]);
        }
    }

    return result;
}

bool isIsometricTree(int *graphA, int lenA, int verticesA, int *graphB, int lenB, int verticesB) {
    return lenA == lenB && verticesA == verticesB &&
        treeCode(graphA, lenA, verticesA) == treeCode(graphB, lenB, verticesB);
}

int main(int argc, char **argv) {
    int testGraph[] = {
        3, 7, 3, 9, 3, 2, 2, 0, 2, 1, 4, 10, 4, 11, 11, 12, 12, 3
    };
    int len = sizeof(testGraph) / sizeof(int);
    printf("Graph connected: %i\n", isConnected(testGraph, len, 10));
    int codeList[BUFFER_SIZE];
    int codeListLen;
    treeCodeList(testGraph, len, 10, codeList, &codeListLen);
    printf("[");
    for (int i = 0; i < codeListLen; i++) {
        printf("%i,", codeList[i]);
    }
    printf("]\n");
    return 0;
}



