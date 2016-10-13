CC = gcc

CFLAGS = --std gnu99 -O0 -Wall -msse2
EXECUTABLE = \
	time_test_naive time_test_sse \
	time_test_sse_pre time_test_sse_avx \
    benchmark_clock_gettime

GIT_HOOKS := .git/hooks/pre-commit

default: impl.o
	$(CC) $(CFLAGS) impl.o time_test.c -DNAIVE -o time_test_naive
	$(CC) $(CFLAGS) impl.o time_test.c -DSSE -o time_test_sse
	$(CC) $(CFLAGS) impl.o time_test.c -DSSEPRE -o time_test_sse_pre
	$(CC) $(CFLAGS) impl.o time_test.c -DSSEAVX -o time_test_sse_avx
	$(CC) $(CFLAGS) impl.o benchmark_clock_gettime.c -o benchmark_clock_gettime

.PHONY: clean default

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

check: default
	time ./time_test_naive
	time ./time_test_sse
	time ./time_test_sse_pre
	time ./time_test_sse_avx

cache-test: default
	perf stat --repeat 100 \
	-e cache-misses,cache-references,instructions,cycles \
	./time_test_naive
	perf stat --repeat 100 \
	-e cache-misses,cache-references,instructions,cycles \
	./time_test_sse
	perf stat --repeat 100 \
	-e cache-misses,cache-references,instructions,cycles \
	./time_test_sse_pre
	perf stat --repeat 100 \
	-e cache-misses,cache-references,instructions,cycles \
	./time_test_sse_avx

gencsv: default
	for i in `seq 1000 5000 1000000`; do \
		printf "%d " $$i;\
		./benchmark_clock_gettime; \
	done > result_clock_gettime.csv

clean:
	rm -f $(EXECUTABLE) *.o *.s *csv
