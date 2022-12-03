#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum shape {
	shape_rock = 1,
	shape_paper = 2,
	shape_scissors = 3,
};

enum score {
	score_lost = 0,
	score_draw = 3,
	score_win  = 6,
};

int main(int argc, char **argv)
{
	unsigned long long total = 0;
	unsigned long long total2 = 0;
	size_t sz = 32;
	char *p = malloc(sz);
	uint8_t value[] = { shape_rock, shape_paper, shape_scissors, };
	uint8_t result[][3] = {
		{ score_draw, score_win, score_lost, },
		{ score_lost, score_draw, score_win, },
		{ score_win, score_lost, score_draw },
	};
	uint8_t value2[][3] = {
		/* rock */
		{ shape_scissors, shape_rock, shape_paper, },
		/* paper */
		{ shape_rock, shape_paper, shape_scissors, },
		/* scissors */
		{ shape_paper, shape_scissors, shape_rock, },
	};
	uint8_t result2[] = { score_lost, score_draw, score_win, };

	while (getline(&p, &sz, stdin) != -1 && *p && *p != '\n') {
		uint8_t abc = p[0] - 'A';
		uint8_t xyz = p[2] - 'X';
		total += value[xyz] + result[abc][xyz];
		total2 += result2[xyz] + value2[abc][xyz];
	}
	printf("%llu %llu\n", total, total2);
	return 0;
}
