#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <threads.h>

#define BUFFER_SIZE 512

typedef struct {
    int vertices;
    long result;
    long stats[BUFFER_SIZE];
} orderFmt;

long treeCount(bool *map, const int d, const int vertices, long *stats) {
    stats[vertices - 2 - d] += 1;
    if (d == 0) {
        return 1;
    }
    long collect = 0;
    for (int i = 0; i < vertices - 1; i++) {
        int first = i;
        int second = i + d;
        if (second < vertices && map[first] != map[second]) {
            bool firstOld = map[first];
            bool secondOld = map[second];
            map[first] = true;
            map[second] = true;
            collect += treeCount(map, d - 1, vertices, stats);
            map[first] = firstOld;
            map[second] = secondOld;
        }
    }

    return collect;
}

long run(int vertices, long *stats) {
    if (vertices > BUFFER_SIZE) {
        return -1;
    }
    bool map[BUFFER_SIZE];
    memset(map, 0, sizeof(map));
    map[0] = true;
    map[vertices-1] = true;
    long result = treeCount(map, vertices - 2, vertices, stats);
    return result;
}

int worker(void *arg) {
    orderFmt *orders = (orderFmt *) arg;

    // printf("|START|\n");
    memset(orders->stats, 0, sizeof(orders->stats));
    int vertices = orders->vertices;
    orders->result = run(vertices, orders->stats);
    // printf("<TERM>\n");
    return 0;
}

int main(int argc, char **argv) {
    int runs[BUFFER_SIZE];
    if (argc - 1 > BUFFER_SIZE) {
        printf("Too many arguments\n");
        return 1;
    }
    if (argc < 2) {
        printf("Too few arguments\n");
        return 1;
    }


    for (int i = 1; i < argc; i++) {
        int val = strtol(argv[i], NULL, 10);
        if (val <= 1) {
            printf("Parameters must be base 10 integers that are >1 (pos=%i)\n", i);
            return 1;
        }
        runs[i-1] = val;
    }

    const int runCount = argc - 1;

    const int numWorkers = 3;
    thrd_t workers[numWorkers];
    for (int i = 0; i < runCount; i += numWorkers) {
        orderFmt orders[numWorkers];
        for (int w = 0; i + w < runCount && w < numWorkers; w++) {
            orders[w].vertices = runs[i + w];
            int err = thrd_create(&workers[w], worker, &orders[w]);
            if (err != thrd_success) {
                printf("Thread creation error!\n");
                return 1;
            }
        }
        for (int w = 0; i + w < runCount && w < numWorkers; w++) {
            thrd_join(workers[w], NULL);
            printf("[%3i:%12li =>", orders[w].vertices, orders[w].result);
            for (int f = 0; f < orders[w].vertices - 1; f++) {
                printf(" %li", orders[w].stats[f]);
                if (f + 1 < orders[w].vertices - 1) {
                    printf(",");
                }
            }
            printf("]\n");
        }
    }

    return 0;
}



