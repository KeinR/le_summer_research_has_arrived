#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define BUFFER_SIZE 512

void treeJordanCenter(int *tree, int len, int *outA, int *outB) {
    // Worst time complexity of O(len * 5/2)
    // IE, this is O(n) in terms of the size of the tree
    
    typedef struct {
        int value;
        int index;
    } leaf;

    leaf leaves[BUFFER_SIZE];
    int leavesLen = 0;
    int counterMap[BUFFER_SIZE] = {0};
    // O(len)
    for (int i = 0; i < len; i++) {
        counterMap[tree[i]]++;
    }
    // O(len)
    for (int i = 0; i < len; i++) {
        int v = tree[i];
        if (counterMap[v] == 1) {
            counterMap[v] = -1;
            leaf l;
            l.value = v;
            l.index = i;
            leaves[leavesLen++] = l;
        }
    }

    bool running = true;
    // O(vertices)
    // vertices = len/2 
    // O(len/2)
    while (running) {
        if (leavesLen == 2 && abs(leaves[0].index - leaves[1].index) == 1) {
            // There are two leaves, and they are connected- meaning,
            // there are two nodes in teh graph left. Thus, we have just two
            // centers
            *outA = leaves[0].value;
            *outB = leaves[1].value;
            running = false;
        } else if (leavesLen == 1) {
            // The only case where a tree can have one leaf is when it only
            // has one vertex.
            // Now that all the leaves have been removed, we have the center.
            *outA = leaves[0].value;
            *outB = -1;
            running = false;
        } else {
            // Remove all the leaves.
            // Replace old leaves with the new ones.
            int newLeavesLen = 0;
            for (int i = 0; i < leavesLen; i++) {
                leaf l = leaves[i];
                leaf nl;
                // The other node in the 2-indexed value pair
                nl.index = !(l.index % 2) + l.index - l.index % 2;
                nl.value = tree[nl.index];
                // Reduce the number of connections to vertex nl.value
                counterMap[nl.value]--;
                // When connections are equal to 1, then we have created a leaf
                if (counterMap[nl.value] == 1) {
                    leaves[newLeavesLen++] = nl;
                }
            }
            leavesLen = newLeavesLen;
        }
    }
}

// Approximately O(n^2), which is, if nothing else, better than Floyd–Warshall
void graphJordanCenter(int *graph, int len, int *output, int *outputLen) {
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

    // O(edges)
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
            vertices.data[vertices.len++] = a;
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
    int data[] = {
        0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6
    };
    int output[BUFFER_SIZE] = {0};
    treeJordanCenter(data, sizeof(data)/sizeof(int),  output+1, output+2);
    printf("%i and %i\n", output[0], output[1]);
    return 0;
}



