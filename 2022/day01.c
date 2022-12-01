#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	unsigned long long max[3] = { 0 }, total = 0;
	size_t sz = 32;
	char *p = malloc(sz);

	while (getline(&p, &sz, stdin) != -1) {
		if (*p != '\n') {
			unsigned long long item = strtoull(p, NULL, 10);
			total += item;
			continue;
		}
		if (total > max[0]) {
			max[2] = max[1];
			max[1] = max[0];
			max[0] = total;
		} else if (total > max[1]) {
			max[2] = max[1];
			max[1] = total;
		} else if (total > max[2]) {
			max[2] = total;
		}
		total = 0;
	}

	if (total > max[0]) {
		max[2] = max[1];
		max[1] = max[0];
		max[0] = total;
	} else if (total > max[1]) {
		max[2] = max[1];
		max[1] = total;
	} else if (total > max[2]) {
		max[2] = total;
	}
	for (size_t i = 0; i < 3; i++)
		printf("%llu\n", max[i]);
	printf("%llu\n", max[0] + max[1] + max[2]);
	return 0;
}
