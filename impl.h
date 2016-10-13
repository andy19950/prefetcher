#ifndef TRANSPOSE_IMPL
#define TRANSPOSE_IMPL

void naive_transpose(int *src, int *dst, int w, int h);
void sse_transpose(int *src, int *dst, int w, int h);
void sse_prefetch_transpose(int *src, int *dst, int w, int h);

#endif
