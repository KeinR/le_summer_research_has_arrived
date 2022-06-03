#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

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
// len must be divisable by two and > 0
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

unsigned long treeCode(int *tree, int len, int vertices, int *error) {
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
    const int numPrimes = sizeof(primes) / sizeof(int);
    unsigned long result = 1;

    treeCodeList(tree, len, vertices, codes, &codesLen);
    for (int i = 0; i < codesLen; i++) {
        int c = codes[i];
        if (c >= numPrimes) {
            printf("Buddy that's breaking the prime gap\n");
            if (error != NULL)
                *error = 1;
            return -1;
        }
        counts[c]++;
    }
    for (int i = 0; i < numPrimes; i++) {
        if (counts[i] != 0) {
            result *= pow(primes[i], counts[i]);
        }
    }

    return result;
}

bool isIsometricTree(int *graphA, int lenA, int verticesA, int *graphB, int lenB, int verticesB) {
    return lenA == lenB && verticesA == verticesB &&
        treeCode(graphA, lenA, verticesA, NULL) == treeCode(graphB, lenB, verticesB, NULL);
}

// Output is a 2-d matrix of BUFFER_SIZE x BUFFER_SIZE
// Vertices >= 2
void bruteTestVertices(int vertices, int *output, int *outputLen, int *seriesLenOutput) {
    // The current tested series
    int series[BUFFER_SIZE] = {0};
    // List of non-isomorphic series
    const int seriesLen = (vertices - 1) * 2;
    const size_t outputStride = BUFFER_SIZE;

    *outputLen = 0;
    *seriesLenOutput = seriesLen;

    while (series[seriesLen - 1] < vertices) {
        if (isTree(series, seriesLen, vertices)) {
            bool good = true;
            for (int i = 0; i < *outputLen; i++) {
                if (isIsometricTree(output + i * outputStride, seriesLen, vertices, series, seriesLen, vertices)) {
                    good = false;
                    break;
                }
            }
            if (good) {
                memcpy(output + *outputLen * outputStride, series, sizeof(int) * seriesLen);
                (*outputLen)++;
            }

        }
        series[0] += 1;
        // Do not iterate over the last series item, since that is checked
        // as the exit condition for the wrapper loop
        for (int i = 0; i < seriesLen - 1 && series[i] >= vertices; i++) {
            series[i] = 0;
            series[i+1]++;
        }
    }
}

// Func is a function like bruteTestVertices()
// tests is assumed to be size of size endVertex - startVertex
void validateTests(void (*func)(int,int*,int*,int*), int startVertex, int endVertex, bool *tests) {
    // https://oeis.org/A000055
    // Index directly corresponds to vertex (start is vertex zero)
    const long answers[] = {
            1,1,1,1,2,3,6,11,23,47,106,235,551,1301,3159,
            7741,19320,48629,123867,317955,823065,2144505,
            5623756,14828074,39299897,104636890,279793450,
            751065460,2023443032,5469566585,14830871802,
            40330829030,109972410221,300628862480,
            823779631721,2262366343746,6226306037178
    };
    const int numAnswers = sizeof(answers) / sizeof(long);
    int answerable = 0;

    int series[BUFFER_SIZE][BUFFER_SIZE];
    int seriesLen;
    int indvSeriesLen;

    // To prevent buffer overflow, only take what I can...
    for (; answerable < numAnswers && answers[answerable] * 2 < BUFFER_SIZE; answerable++);

    answerable = answerable < endVertex ? answerable : endVertex;

    for (int i = startVertex; i < answerable; i++) {
        bruteTestVertices(i, (int *) series, &seriesLen, &indvSeriesLen);
        printf("seriesLen = %i\n", seriesLen);
        for (int i = 0; i < seriesLen; i++) {
            printf("[");
            for (int s = 0; s < indvSeriesLen; s++) {
                printf("%i,", series[i][s]);
            }
            printf("]\n");
        }
        tests[i] = seriesLen == answers[i];
    }
}

int main(int argc, char **argv) {
    /*
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
    */

    /*
    int series[BUFFER_SIZE][BUFFER_SIZE];
    int seriesLen;
    int indvSeriesLen;
    bruteTestVertices(4, (int *) series, &seriesLen, &indvSeriesLen);
    for (int i = 0; i < seriesLen; i++) {
        printf("[");
        for (int s = 0; s < indvSeriesLen; s++) {
            printf("%i,", series[i][s]);
        }
        printf("]\n");
    }
    */


    const int start = 2;
    const int end = 5;
    const int numResults = end - start;
    bool results[numResults];
    validateTests(bruteTestVertices, start, end, results);
    for (int i = start; i < end; i++) {
        printf("i=%i -> %s\n", i, results[i] ? "PASS" : "FAIL");
    }
    
    return 0;
}



