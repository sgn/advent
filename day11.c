#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 128

unsigned left[BUFFER][BUFFER], right[BUFFER][BUFFER];
unsigned up[BUFFER][BUFFER], down[BUFFER][BUFFER];
unsigned upleft[BUFFER][BUFFER], downright[BUFFER][BUFFER];
unsigned upright[BUFFER][BUFFER], downleft[BUFFER][BUFFER];

int ntaken(char (*input)[BUFFER], unsigned i, unsigned j)
{
	int result =
		(input[i - upleft[i][j]][j - upleft[i][j]] == '#') +
		(input[i - up[i][j]][j] == '#') +
		(input[i - upright[i][j]][j + upright[i][j]] == '#') +
		(input[i][j - left[i][j]] == '#') +
		(input[i][j + right[i][j]] == '#') +
		(input[i + downleft[i][j]][j - downleft[i][j]] == '#') +
		(input[i + down[i][j]][j] == '#') +
		(input[i + downright[i][j]][j + downright[i][j]] == '#');
	return result;
}

unsigned xtransform(char (*input)[BUFFER], char (*output)[BUFFER],
		    unsigned h, unsigned w)
{
	unsigned occupied = 0;
	unsigned taken;
	int changed = 0;
	unsigned i, j;
	for (i = 1; i < h - 1; i++)
		for (j = 1; j < w - 1; j++) {
			if (input[i][j] == 'L') {
				taken = ntaken(input, i, j);
				if (taken == 0) {
					output[i][j] = '#';
					occupied++;
					changed = 1;
				} else
					output[i][j] = 'L';
			} else if (input[i][j] == '#') {
				taken = ntaken(input, i, j);
				if (taken >= 5) {
					output[i][j] = 'L';
					changed = 1;
				} else {
					output[i][j] = '#';
					occupied++;
				}
			}
		}
	if (changed)
		return 0;
	return occupied;
}

int main()
{
	unsigned occupied = 0;
	unsigned h = 2, w;
	unsigned i, j;
	unsigned delta;
	char x[BUFFER][BUFFER];
	char y[BUFFER][BUFFER];
	memset(y, '.', BUFFER*BUFFER);

	x[1][0] = '.';
	fgets(x[1] + 1, BUFFER - 1, stdin);
	w = strlen(x[1]);
	if (w == BUFFER - 1)
		return 1;
	x[1][w - 1] = '.';
	x[1][w] = 0;
	memset(x[0], '.', w);
	x[0][w] = 0;
	while (fgets(x[h] + 1, BUFFER, stdin)) {
		x[h][0] = '.';
		if (w != strlen(x[h]))
			return 1;
		x[h][w - 1] = '.';
		x[h][w] = 0;
		h++;
	}
	memset(x[h++], '.', w);
	x[h][w + 1] = 0;

	memset(up, 0, BUFFER * sizeof(unsigned));
	memset(upleft, 0, BUFFER * sizeof(unsigned));
	memset(upright, 0, BUFFER * sizeof(unsigned));
	for (j = 0; j < w; j++)
		up[1][j] = upleft[1][j] = upright[1][j] = 1;
	for (i = 1; i < h; i++) {
		left[i][1] = 1;
		upleft[i][1] = 1;
		for (j = 1; j < w; j++) {
			if (x[i-1][j+1] == '.')
				upright[i][j] = upright[i-1][j+1] + 1;
			else
				upright[i][j] = 1;

			down[i][j] = h - 1 - i;
			right[i][j] = w - 1 - j;
			downright[i][j] = down[i][j] > right[i][j] ?
				right[i][j] : down[i][j];
			downleft[i][j] = down[i][j] > j ?  j : down[i][j];
			if (x[i][j] == '.') {
				left[i][j + 1] = left[i][j] + 1;
				up[i+1][j] = up[i][j] + 1;
				upleft[i+1][j+1] = upleft[i][j] + 1;
			} else {
				left[i][j + 1] = 1;
				up[i+1][j] = 1;
				upleft[i+1][j+1] = 1;

				for (delta = left[i][j]; delta; delta--)
					right[i][j - delta] = delta;
				for (delta = up[i][j]; delta; delta--)
					down[i - delta][j] = delta;
				for (delta = upleft[i][j]; delta; delta--)
					downright[i - delta][j - delta] = delta;
				for (delta = upright[i][j]; delta; delta--)
					downleft[i - delta][j + delta] = delta;
			}
		}
	}

	while (!occupied && !(occupied = xtransform(x, y, h, w)))
	       occupied = xtransform(y, x, h, w);
	printf("%u\n", occupied);
	return 0;
}
