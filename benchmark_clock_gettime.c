#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "impl.h"

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0
#define TEST_W 4096
#define TEST_H 4096


int main(int argc, char const *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    int *src  = (int *) malloc(sizeof(int) * TEST_W * TEST_H);
    int *out = (int *) malloc(sizeof(int) * TEST_W * TEST_H);

    srand(time(NULL));
    for (int y = 0; y < TEST_H; y++)
        for (int x = 0; x < TEST_W; x++)
            *(src + y * TEST_W + x) = rand();

    // Naive
    clock_gettime(CLOCK_ID, &start);
    naive_transpose(src, out, TEST_W, TEST_H);
    clock_gettime(CLOCK_ID, &end);
    printf("Naive %.4f\n", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);


    // SSE
    clock_gettime(CLOCK_ID, &start);
    sse_transpose(src, out, TEST_W, TEST_H);
    clock_gettime(CLOCK_ID, &end);
    printf("SSE %.4f\n", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);


    // SSE prefetch
    clock_gettime(CLOCK_ID, &start);
    sse_prefetch_transpose(src, out, TEST_W, TEST_H);
    clock_gettime(CLOCK_ID, &end);
    printf("SSE_prefetch %.4f\n", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);


    // AVX
    clock_gettime(CLOCK_ID, &start);
    avx_transpose(src, out, TEST_W, TEST_H);
    clock_gettime(CLOCK_ID, &end);
    printf("AVX %.4f\n", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);

    // AVX prefetch
    clock_gettime(CLOCK_ID, &start);
    avx_prefetch_transpose(src, out, TEST_W, TEST_H);
    clock_gettime(CLOCK_ID, &end);
    printf("AVX_prefetch %.4f\n", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);

    return 0;
}
