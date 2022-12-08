#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 128

int main(int argc, char **argv)
{
	uint8_t h[WIDTH][WIDTH] = { 0 };
	uint8_t visible[WIDTH][WIDTH] = { 0 };
	size_t sz = 1024;
	char *line = malloc(sz);
	ssize_t len;
	uint8_t ncol, nrow = 0;
	size_t total = 0;
	uint8_t ch[WIDTH] = { 0 };
	uint8_t lscore[WIDTH][WIDTH] = { 0 };
	uint8_t rscore[WIDTH][WIDTH] = { 0 };
	uint8_t tscore[WIDTH][WIDTH] = { 0 };
	uint8_t dscore[WIDTH][WIDTH] = { 0 };
	uint8_t lastrowblock[WIDTH][10] = { 0 };
	unsigned long long max = 0;

	while ((len = getline(&line, &sz, stdin)) != -1) {
		for (ncol = 0; ncol < len - 1; ncol++)
			h[nrow][ncol] = line[ncol];
		nrow++;
	}

	// left-to-right, top-down
	for (uint8_t row = 0; row < nrow; row++) {
		uint8_t rh = 0;
		uint8_t ci = 0;
		uint8_t lastcolblock[10] = { 0 };
		for (uint8_t col = 0; col < ncol; col++) {
		     	uint8_t raw = h[row][col];
		     	uint8_t high = raw - '0';
			if (raw > ch[col]) {
				visible[row][col] = 1;
				ch[col] = raw;
			}
			if (raw > rh) {
				visible[row][col] = 1;
				rh = raw;
			}

			lscore[row][col] =
				col - lastcolblock[high];
			for (uint8_t value = 0; value <= high; value++)
			     	lastcolblock[value] = col;

			tscore[row][col]
				= row - lastrowblock[col][high];
			for (uint8_t value = 0; value <= high; value++)
			     	lastrowblock[col][value] = row;
		}
	}

	memset(ch, 0, WIDTH);
	memset(lastrowblock, nrow - 1, WIDTH * 10);
	// right-to-left, bottom-up
	for (uint8_t row = nrow; row > 0;) {
		uint8_t rh = 0;
		uint8_t lastcolblock[10];
		memset(lastcolblock, ncol - 1, 10);
		row--;
		for (uint8_t col = ncol; col > 0;) {
			uint8_t raw, high;
			col--;
			raw = h[row][col];
			if (raw > ch[col]) {
				visible[row][col] = 1;
				ch[col] = raw;
			}
			if (raw > rh) {
				visible[row][col] = 1;
				rh = raw;
			}
			high = raw - '0';
			rscore[row][col] = lastcolblock[high] - col;
			for (uint8_t value = 0; value <= high; value++)
			     	lastcolblock[value] = col;

			dscore[row][col] =
				lastrowblock[col][high] - row;
			for (uint8_t value = 0; value <= high; value++)
			     	lastrowblock[col][value] = row;
		}
	}

	for (size_t row = 0; row < WIDTH; row++) {
		for (size_t col = 0;
		     col < WIDTH && h[row][col] != 0xFF; col++) {
			unsigned long long score = 1;
			score *= lscore[row][col];
			score *= rscore[row][col];
			score *= dscore[row][col];
			score *= tscore[row][col];
			if (visible[row][col]) {
				total++;
			}
			if (max < score)
				max = score;
		}
	}

	printf("%zu\n", total);
	printf("%llu\n", max);
	return 0;
}
