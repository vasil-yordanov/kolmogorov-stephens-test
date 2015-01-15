CC=gcc
LIBS=-lm

all: stephens simulate_data deviation_statistics

stephens: stephens.c kolmogorov.c kolmogorov.h
	$(CC) -o $@ $^ $(LIBS)

simulate_data: simulate_data.c random_generator.c random_generator.h
	$(CC) -o $@ $^ $(LIBS)

deviation_statistics: deviation_statistics.c kolmogorov.h kolmogorov.c random_generator.c random_generator.h
	$(CC) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	rm -f stephens simulate_data deviation_statistics
