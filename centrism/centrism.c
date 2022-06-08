#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define BUFFER_SIZE 512

// Must have more than one vertex
void treeJordanCenter(int *tree, int len, int *outA, int *outB) {
    assert(len % 2 == 0);
    
    typedef struct {
        int ad[BUFFER_SIZE];
        int adLen;
        int connect;
    } vertexInfo;

    vertexInfo vertexMap[BUFFER_SIZE] = {0};
    // The last set of snapped off leaves
    int leaves[BUFFER_SIZE];
    int leavesLen = 0;

    // Special case, v=2 tree
    if (len / 2 == 2) {
        *outA = tree[0];
        *outB = tree[1];
        return;
    } 

    for (int i = 0; i < len; i++) {
        vertexMap[tree[i]].connect++;
        // the other value in the pair
        int oi = !(i % 2) + i - i % 2;
        vertexMap[tree[i]].ad[vertexMap[tree[i]].adLen++] = tree[oi];
    }

    for (int i = 0; i < len; i++) {
        int v = tree[i];
        if (vertexMap[v].connect == 1) {
            vertexMap[v].connect = 0;
            leaves[leavesLen++] = v;
        }
    }

    bool run = true;
    while (run) {
        // Remove all the leaves.
        // Replace old leaves with the new ones.
        int newLeavesLen = 0;
        for (int i = 0; i < leavesLen; i++) {
            int l = leaves[i];
            int nl = -1;
            for (int a = 0; a < vertexMap[l].adLen && nl == -1; a++) {
                int v = vertexMap[l].ad[a];
                if (vertexMap[v].connect == 2) {
                    vertexMap[v].connect = 0;
                    nl = v;
                } else {
                    vertexMap[v].connect--;
                }
            }
            if (nl != -1) {
                leaves[newLeavesLen++] = nl;
            }
        }
        leavesLen = newLeavesLen;
        if (leavesLen == 0) {
            *outA = leaves[0];
            *outB = leaves[1];
            run = false;
        }
        if (leavesLen == 1) {
            *outA = leaves[0];
            *outB = -1;
            run = false;
        }
    }
}

// Approximately O(n^2), which is, if nothing else, better than Floyd–Warshall
void graphJordanCenter(int *graph, int len, int *output, int *outputLen) {
    assert(len % 2 == 0);

    typedef struct {
        int data[BUFFER_SIZE];
        int len;
    } bufferType;
    
    // Adacency map
    bufferType vertexMap[BUFFER_SIZE] = {0};
    // List of all unique vertices
    bufferType vertices;
    // Alternating vertex buffers
    bufferType vertexBuffers[2];
    bool getMap[BUFFER_SIZE] = {0};
    
    int bestScore = INT_MAX;
    // Uncecessary, does nothing, but still good practice I think.
    *outputLen = 0;

    vertices.len = 0;
    for (int i = 0; i < len/2; i++) {
        // Could compress this by using modulo
        // to get the other one
        int a = graph[i*2];
        int b = graph[i*2+1];
        bufferType *av = &vertexMap[a];
        bufferType *bv = &vertexMap[b];
        av->data[av->len++] = b;
        bv->data[bv->len++] = a;
        if (av->len == 1) {
            vertices.data[vertices.len++] = a;
        }
        if (bv->len == 1) {
            vertices.data[vertices.len++] = b;
        }
    }

    // O(vertices^2)
    for (int i = 0; i < vertices.len; i++) {
        int v = vertices.data[i];
        int score = 0;
        int ab = 0;
        int ib = 1;
        vertexBuffers[ab].data[0] = v;
        vertexBuffers[ab].len = 1;
        vertexBuffers[ib].len = 0;

        // Can get rid of this entirely if we convert
        // getMap to an integer map and give each loop
        // a unique integer value to represent a GET
        memset(getMap, 0, sizeof(getMap));

        do {
            score++;
            vertexBuffers[ib].len = 0;
            for (int f = 0; f < vertexBuffers[ab].len; f++) {
                int vert = vertexBuffers[ab].data[f];
                if (!getMap[vert]) {
                    getMap[vert] = true;
                    memcpy(vertexBuffers[ib].data + vertexBuffers[ib].len, vertexMap[vert].data, sizeof(int) * vertexMap[vert].len);
                    vertexBuffers[ib].len += vertexMap[vert].len;
                }
            }

            int oab = ab;
            ab = ib;
            ib = oab;
        } while (vertexBuffers[ab].len > 0);

        if (score < bestScore) {
            bestScore = score;
            *outputLen = 0;
        }
        if (score == bestScore) {
            output[(*outputLen)++] = v;
        }
    }
}


int main(int argc, char **argv) {
    // Line graph, 3
    int data[] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6};
    // Binary tree, 1
    int data2[] = {4,2,2,5,2,1,1,3,3,6,3,7};
    // that one thing from wikipedia, 8,3,4
    int dataG[] = {1,2,2,3,3,4,4,5,4,6,6,7,6,8,8,9,8,3};
    int output[BUFFER_SIZE] = {0};
    int outLen = 0;
    treeJordanCenter(data2, sizeof(data)/sizeof(int),  output+0, output+1);
    printf("%i and %i\n", output[0], output[1]);
    graphJordanCenter(dataG, sizeof(dataG)/sizeof(int), output, &outLen);
    for (int i = 0; i < outLen; i++) {
        printf("%i,", output[i]);
    }
    printf("\n");
    return 0;
}



