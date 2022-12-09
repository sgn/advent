#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 512

int sign(int value)
{
	return !!value | value >> (sizeof(value) * CHAR_BIT - 1);
}

int main(int argc, char **argv)
{
	size_t v1 = 1, v9 = 1;
	uint8_t n1[2 * WIDTH][2 * WIDTH] = { 0 };
	uint8_t n9[2 * WIDTH][2 * WIDTH] = { 0 };
	int xnode[10] = { 0 }, ynode[10] = { 0 };
	ssize_t len;
	size_t sz = 1024;
	char *line = malloc(sz);

	n1[xnode[1] + WIDTH][ynode[1] + WIDTH] = 1;
	n9[xnode[9] + WIDTH][ynode[9] + WIDTH] = 1;

	while ((len = getline(&line, &sz, stdin)) != -1) {
		char direction = line[0];
		long step;
		line[len - 1] = '\0';
		if (!direction)
			break;
		step = strtoul(line + 2, NULL, 10);
		while (step--) {
			switch (direction) {
			case 'R': xnode[0]++; break;
			case 'L': xnode[0]--; break;
			case 'U': ynode[0]++; break;
			case 'D': ynode[0]--; break;
			default:
				return 1;
			}
			for (size_t i = 1; i < 10; i++) {
				int dx = xnode[i - 1] - xnode[i];
				int dy = ynode[i - 1] - ynode[i];
				if ((dx == -1 || dx == 0 || dx == 1) &&
				    (dy == -1 || dy == 0 || dy == 1))
					break;
				xnode[i] += sign(dx);
				ynode[i] += sign(dy);
			}
			if (!n1[xnode[1] + WIDTH][ynode[1] + WIDTH]) {
				n1[xnode[1] + WIDTH][ynode[1] + WIDTH] = 1;
				v1++;
			}
			if (!n9[xnode[9] + WIDTH][ynode[9] + WIDTH]) {
				n9[xnode[9] + WIDTH][ynode[9] + WIDTH] = 1;
				v9++;
			}
		}
	}

	free(line);

	printf("%zu %zu\n", v1, v9);

	return 0;
}
