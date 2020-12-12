#include <stdio.h>
#include <stdlib.h>

#define YEAR 2020

int main(int argc, char **argv)
{
	unsigned int value;
	unsigned int m, n, p;
	unsigned int offset;
	char marker[YEAR + 1] = { 0 };

	while (scanf("%u\n", &value) > 0) {
		if (value <= YEAR)
			++marker[value];
	}

	for (value = 0; value <= YEAR / 2; value++)
		if (marker[value] && marker[YEAR - value])
			printf("%d\n", value * (YEAR - value));
	for (m = 0; m <= YEAR; m++) {
		if (marker[m] == 0)
			continue;
		offset = marker[m] > 1;
		for (n = m + offset; m + 2 * n <= YEAR; n++)
			if (marker[n] && marker[YEAR - m - n])
				printf("%llu\n", (unsigned long long)(m) * n * (YEAR - m - n));
	}
	return 0;
}
