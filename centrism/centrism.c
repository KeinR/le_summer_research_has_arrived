#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

void treeJordanCenter(int *tree, int len, int *outA, int *outB) {
    // Worst time complexity of O(len * 5/2)
    
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


int main(int argc, char **argv) {

    return 0;
}



