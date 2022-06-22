#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

void partitiond(bool *grid, size_t gridStride, int width, int height) {
    const int negh[16] = {
        -1,-1,   0,-1,   1,-1
        -1,0,            1,0
        -1,1,    0,1,    1,1
    };
    const int neghLen = sizeof(negh) / sizeof(int) / 2;
    bool getMap[BUFFER_SIZE] = {0};

    for (int x = 1; x < width; x += 2) {
        for (int y = 1; y < height; y += 2) {
            for (int i = 0; i < neghLen; i++) {
                int nx = x + negh[i*2];
                int ny = y + negh[i*2+1];
                if (nx >= 0 && ny >= 0 && nx < width && ny < height) {

                }
            }
        }
    }
}



int main(int argc, char **argv) {

}




