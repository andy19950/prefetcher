#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>

#include <xmmintrin.h>

#define TEST_W 4096
#define TEST_H 4096

#include "impl.h"

int main(int argc, char *argv[])
{
    /* verify the result of 4x4 matrix */
    /*{
        int testin[16] = { 0, 1,  2,  3,  4,  5,  6,  7,
                           8, 9, 10, 11, 12, 13, 14, 15
                         };
        int testout[16];
        int expected[16] = { 0, 4,  8, 12, 1, 5,  9, 13,
                             2, 6, 10, 14, 3, 7, 11, 15
                           };

        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++)
                printf(" %2d", testin[y * 4 + x]);
            printf("\n");
        }
        printf("\n");
        sse_transpose(testin, testout, 4, 4);
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++)
                printf(" %2d", testout[y * 4 + x]);
            printf("\n");
        }
        assert(0 == memcmp(testout, expected, 16 * sizeof(int)) &&
               "Verification fails");
    }*/

    {
        int *src  = (int *) malloc(sizeof(int) * TEST_W * TEST_H);
        int *out = (int *) malloc(sizeof(int) * TEST_W * TEST_H);

        srand(time(NULL));
        for (int y = 0; y < TEST_H; y++)
            for (int x = 0; x < TEST_W; x++)
                *(src + y * TEST_W + x) = rand();

#ifdef SSEPRE
        sse_prefetch_transpose(src, out, TEST_W, TEST_H);
#endif

#ifdef SSE
        sse_transpose(src, out, TEST_W, TEST_H);
#endif

#ifdef NAIVE
        naive_transpose(src, out, TEST_W, TEST_H);
#endif

#ifdef AVX
        avx_transpose(src, out, TEST_W, TEST_H);
#endif

#ifdef AVXPRE
        avx_prefetch_transpose(src, out, TEST_W, TEST_H);
#endif

        free(src);
        free(out);
        //data_correctness();
    }

    return 0;
}
