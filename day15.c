#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	unsigned *last;
	unsigned turn = 0;
	unsigned spoken;
	unsigned prev = 0;
	unsigned numturn;

	numturn = strtoul(*++argv, NULL, 10);
	last = calloc(numturn, sizeof(unsigned));
	memset(last, 0, numturn * sizeof(unsigned));

	while (*++argv)
		last[spoken = strtoul(*argv, NULL, 10)] = ++turn;
	while (turn < numturn) {
		if (prev) {
			spoken = turn - prev;
		} else {
			spoken = 0;
		}
		prev = last[spoken];
		last[spoken] = ++turn;
	}
	printf("%u\n", spoken);
	return 0;
}
