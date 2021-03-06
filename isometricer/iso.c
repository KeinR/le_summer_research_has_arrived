#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#define BUFFER_SIZE 512


// TODO:
// Memioization
// Threading



// Terminology here:
//
// graph/tree: the flattened array with all of the pairs
// of vertices that form the edges.
// So, the tree:
// 1 - 2 - 3
//     |
//     4
// Would have edges (1, 2), (2, 3), (2, 4)
// And thus graph = {1, 2, 2, 3, 2, 4}
// No particular order is required.
// It follows that the length of this graph must always be divisiable
// by two
//
// Graphs here must always have two or more vertices



// graph: the graph array
// graphLen: the length of graph array - NOT the number
// of vertices or edges. Must be divisiable by 2.
// vertices: the number of unique vertices. Thus MUST be
// correct for the given graph.
// Return: the specified graph is connected or not.
bool isConnected(int *graph, int graphLen, int vertices) {
    // Maps vertices to wheter they are connected to the 
    // "main" graph or not.
    bool whitelist[BUFFER_SIZE] = {false};
    // The number of unique vertices that have been added
    // to the whitelist
    int whitelistCount = 0;

    // Yeah no
    if (graphLen < 2)
        return true;
    // Malformed tree - should be divisiable by
    // two if it's considted of ordered pairs
    if (graphLen % 2 != 0)
        return false;

    // Set the starting vertices of the main graph
    whitelist[graph[0]] = true;
    whitelist[graph[1]] = true;
    whitelistCount += graph[0] == graph[1] ? 1 : 2;
    graph += 2;
    graphLen -= 2;

    // Loop through the graph array.
    // Whitelist nodes attached to other whitelisted nodes.
    // Exit when no nodes were whitelisted after an iteration-
    // this will mean that either all nodes that are connected to
    // the main graph have been accounted for (whitelisted).
    // This, afterwards, we compare the number of whitelisted vertices
    // with the total vertices to see if we got everything - if the
    // graph is connected.
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

// graph: The graph array
// len: Length of the graph array
// vertices: Unique vertices in the graph 
// Return: if the graph is a tree
bool isTree(int *graph, int len, int vertices) {
    // Tree will be minimally connected, so the number of edges (len/2)
    // must be one less than the vertices.
    // A tree is also connected.
    // These  two conditions rule out the possiblity of loops.
    return len / 2 == vertices - 1 && isConnected(graph, len, vertices);
}

// Finds distances between each of the leaves in the given tree, and packs them into
// the output array.
//
// tree: The graph array. Must be a tree
// len: Length of the graph array
// vertices: Unique vertices in the graph 
// output: The code list (buffer). Assumed to be of size BUFFER_SIZE
// outputLen: Length of the code list
void treeCodeList(int *tree, int len, int vertices, int *output, int *outputLen) {
    typedef struct {
        int path;
        int vertex;
    } pathStruc;

    // Maps vertices to a list of vertices that that
    // vertex is connected by an edge to.
    // The first index of each list is the length
    // (and everything is initialized to zero, so...)
    int map[BUFFER_SIZE][BUFFER_SIZE] = {0};
    bool catchMap[BUFFER_SIZE][BUFFER_SIZE] = {false};
    // List of each unique vertex (no duplicates).
    // This allows for the node labels to not be sequential.
    int vertVals[BUFFER_SIZE];
    pathStruc path[BUFFER_SIZE];
    // The length of `path`
    int pathLen = 0;
    // The length of `vertVals`
    int numVerts = 0;

    // We will be incrementing this later
    *outputLen = 0;

    // Build `map`
    for (int i = 0; i < len / 2; i++) {
        int m = i * 2;
        int f = tree[m];
        int s = tree[m+1];
        // The length of each adjacency list,
        // stored at the beginning
        int *plf = &map[f][0];
        int *pls = &map[s][0];
        // Add a new unique verti
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

    // How do you you like my *tail* recursion uwu
    // Correct, this would be normally implemented with recursion if you
    // were a human with a soul.
    // But I lost mine a long time ago... That fateful day when I discovered hentai.
    // Anyways, the princible behind how this works is based on how arithmatic works,
    // except that the base depends on each index.
    // You could also think it as a decision tree encoded as an array.
    // `path` is the number, or decision tree array. It is first initialized with a starting
    // value. This will be be a pathStruc object with `vertex` being the leaf vertex label and `path`
    // being the "pointer", the index of the node in map[vertex]. This is the node that this leaf is connected to.
    //
    // Enter the loop, and test the next node, the one being pointed to by the last element in the path array.
    // If that one is a leaf, we have a path: so, if a path between those two nodes doesn't exist, add it to the output.
    // If it is actually a previous node we're going in circles, and we want to terminate this train.
    // In both cases, we want to increment the `path` variable of the path array element. In the decision tree sense, we
    // are ending one path and going down another, unexplored one in a defined order.
    // We increment the path variable. Then, we loop through the path array backwards. Each time, we truncate the array
    // if the last element's path varialbe is incremented to be greater than or equal to the vertex variable's mapping
    // in `map`'s list length. This removes paths that have iterated over all those that they are connected to: removes
    // nodes in the decision tree for which all their children have been explored.
    //
    // If the element being pointed to is not a leaf and is not a previous node in the path, then add it to the path.
    // We will, in the next iteration, test those that is is connected to for leaves or loops.
    // Note that I am referring to "loops" in the loose sense: the input is a tree, there are no loops. But we can loop
    // if we go parent-child-parent-child-parent-child....
    //
    // Since the leaf is only connected to one element, we are looping until all those connected to the beginning path
    // element have been explored. When that has happened, path[0].path will drop below 1.

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
                        int min, max;
                        if (v < newVert) {
                            min = v;
                            max = newVert;
                        } else {
                            min = newVert;
                            max = v;
                        }
                        if (!catchMap[max][min]) {
                            catchMap[max][min] = true;
                            output[(*outputLen)++] = pathLen;
                        }
                    }
                    // Increment the path
                    path[pathLen - 1].path++;
                    // i > 0 deliberate, need one look behind.
                    // Besides, it's the exit condition.
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

// tree: The graph array. Must be a tree
// len: Length of the graph array
// vertices: Unique vertices in the graph 
// error: pointer to variable to store errors. Can be NULL.
//      - 1: Not enough primes are known to get this tree's code
// Return: The unique isomorphic code for the tree type
unsigned long treeCode(int *tree, int len, int vertices, int *error) {
    int codes[BUFFER_SIZE];
    int codesLen;
    // By the fundamentla theorum of arithmatic, every number
    // has a unique prime factorization.
    // That means that we can encode a sequence of numbers of
    // varying sizes as a prime factorization.
    // Map the numbers to primes, then multiply them together.
    // Only issue is that the numbers can get pretty large...
    // So it might break unless I bust out that one GNU big
    // numbers library.
    // Primes from: https://oeis.org/A000040
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

// Takes two trees and tests if they're isomorphic.
// They MUST be trees.
bool isIsometricTree(int *graphA, int lenA, int verticesA, int *graphB, int lenB, int verticesB) {
    // Same trees must have same number edges, vertices, and the same path lengths between each leaf.
    return lenA == lenB && verticesA == verticesB &&
        treeCode(graphA, lenA, verticesA, NULL) == treeCode(graphB, lenB, verticesB, NULL);
}

// Generates every possible tree with X number of vertices and finds all unique, non-isomorphic trees.
// In other words, finds all tree classes for a given number of vertices.
// As of writing, has been verified for like... Oh actually I don't remember.
// 5 or so vertices. 6?
//
// vertices: tree class to test; test all trees with x vertices. vertices >= 2
// output: a 2-d matrix of BUFFER_SIZE x BUFFER_SIZE, holding all the non-isomorphic trees.
// outputLen: length of output.
// seriesLenOutput: length of each tree array in output.
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

// Takes a given function, such as bruteTestVertices(), that tries to determine all of the non-isomorphic
// trees for a given number of vertices.
// It then compares these to the actual numbers from OEIS, and returns the results.
//
// func: a function like bruteTestVertices(), used to get non-isomorphic trees.
// startVertex: vertex to start at
// endVertex: vertex to end at
// tests: assumed to be size of size endVertex - startVertex. Output for test results.
// the test results are whether func really was able to find all the non-isomorphic graphs
// for a given number of vertices
void validateTests(void (*func)(int vertices,int*output,int*outputLen,int*seriesLenOutput), int startVertex, int endVertex, bool *tests) {
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
        func(i, (int *) series, &seriesLen, &indvSeriesLen);
        /*
        printf("seriesLen = %i\n", seriesLen);
        for (int i = 0; i < seriesLen; i++) {
            printf("[");
            for (int s = 0; s < indvSeriesLen; s++) {
                printf("%i,", series[i][s]);
            }
            printf("]\n");
        }
        */
        tests[i - startVertex] = seriesLen == answers[i];
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
    const int end = 6;
    const int numResults = end - start;
    bool results[numResults];
    validateTests(bruteTestVertices, start, end, results);
    for (int i = start; i < end; i++) {
        printf("i=%i -> %s\n", i, results[i - start] ? "PASS" : "FAIL");
    }
    
    return 0;
}



