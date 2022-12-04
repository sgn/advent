#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sign(long long value)
{
	return !!value | value >> (sizeof(value) * CHAR_BIT - 1);
}

int main(int argc, char **argv)
{
	unsigned long long total = 0;
	unsigned long long common = 0;
	size_t sz = 1024;
	char *line = malloc(sz);

	while (getline(&line, &sz, stdin) != -1 && *line && *line != '\n') {
		int64_t ll, lr, rl, rr;
		int ls, rs;
		char *p = line, *endptr;
		ll = strtoull(p, &endptr, 10);
		p = endptr + 1;
		lr = strtoull(p, &endptr, 10);
		p = endptr + 1;
		rl = strtoull(p, &endptr, 10);
		p = endptr + 1;
		rr = strtoull(p, &endptr, 10);
		p = endptr + 1;
		if (sign(rr - lr) * sign(rl - ll) != 1)
			common++;
		if (sign(rr - ll) * sign(rl - lr) != 1)
			total++;
	}
	printf("%llu %llu\n", total, common);
	return 0;
}
