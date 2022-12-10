#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int xwrite(int cycle, int xvalue)
{
	if (cycle % 40 == 0) {
		if (xvalue ==  38 || xvalue == 39)
			puts("#");
		else
			puts(".");
	} else if (cycle % 40 - 2 <= xvalue && xvalue <= cycle % 40)
		putchar('#');
	else
		putchar('.');
}

int main(int argc, char **argv)
{
	ssize_t len;
	size_t sz = 1024;
	char *line = malloc(sz);
	int cycle = 0;
	int xvalue = 1;
	size_t ecycle = 20;
	long long total = 0;

	while ((len = getline(&line, &sz, stdin)) != -1) {
		if (*line == '\0' || *line == '\n')
			break;
		cycle++;
		if (cycle == ecycle) {
			total += cycle * xvalue;
			ecycle += 40;
		}
		xwrite(cycle, xvalue);
		if (*line == 'n') {
			continue;
		}
		cycle++;
		if (cycle == ecycle) {
			total += cycle * xvalue;
			ecycle += 40;
		}
		xwrite(cycle, xvalue);
		xvalue += strtoll(line + 5, NULL, 10);
	}

	free(line);

	printf("\n%lld\n", total);

	return 0;
}
