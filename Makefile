CC = gcc

CFLAGS = --std gnu99 -O0 -Wall -msse2 -mavx2
EXECUTABLE = \
	time_test_naive time_test_sse \
	time_test_sse_pre time_test_avx \
	time_test_avx_pre benchmark_clock_gettime

GIT_HOOKS := .git/hooks/pre-commit

default: impl.o
	$(CC) $(CFLAGS) impl.o time_test.c -DNAIVE -o time_test_naive
	$(CC) $(CFLAGS) impl.o time_test.c -DSSE -o time_test_sse
	$(CC) $(CFLAGS) impl.o time_test.c -DSSEPRE -o time_test_sse_pre
	$(CC) $(CFLAGS) impl.o time_test.c -DAVX -o time_test_avx
	$(CC) $(CFLAGS) impl.o time_test.c -DAVXPRE -o time_test_avx_pre
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
	time ./time_test_avx
	time ./time_test_avx_pre

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
	./time_test_avx
	perf stat --repeat 100 \
	-e cache-misses,cache-references,instructions,cycles \
	./time_test_avx_pre

gencsv: default
		./benchmark_clock_gettime > result_clock_gettime.csv
		gnuplot scripts/plot.gp
		eog runtime.png

clean:
	rm -f $(EXECUTABLE) *.o *.s *csv
