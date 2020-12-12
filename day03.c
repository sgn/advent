#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YEAR 2020
#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
	char buffer[BUFFER_SIZE];
	unsigned width;
	unsigned lines = 0;
	unsigned c11, c31, c51, c71, c12;
	unsigned x1, x3, x5, x7, x12, y2;
	c11 = c31 = c51 = c71 = c12 = 0;
	x1 = x3 = x5 = x7 = x12 = 0;
	y2 = 1;

	buffer[BUFFER_SIZE - 2] = 0;
	fgets(buffer, BUFFER_SIZE, stdin);
	lines++;
	/* FIXME */
	if (buffer[BUFFER_SIZE - 2])
		return 1;
	width = strlen(buffer) - 1;
	while (fgets(buffer, BUFFER_SIZE, stdin)) {
		lines++;
		if (width != strlen(buffer) - 1)
			return 1;
		x1 = (x1 + 1) % width;
		x3 = (x3 + 3) % width;
		x5 = (x5 + 5) % width;
		x7 = (x7 + 7) % width;
		y2 = !y2;
		if (y2) {
			x12 = (x12 + 1) % width;
			c12 += buffer[x12] == '#';
		}
		c11 += buffer[x1] == '#';
		c31 += buffer[x3] == '#';
		c51 += buffer[x5] == '#';
		c71 += buffer[x7] == '#';
	}
	printf("%u %u %u %u %u\n", c11, c31, c51, c71, c12);
	printf("%llu\n", (unsigned long long)(c11) * c31 * c51 * c71 * c12);
	return 0;
}
