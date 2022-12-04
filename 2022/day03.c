#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	uint8_t priority[128] = { 0 };
	uint8_t dups[128] = { 0 };
	unsigned long long total = 0;
	unsigned long long common = 0;
	size_t sz = 1024;
	char *p = malloc(sz);
	int round = 0;

	for (size_t i = 0; i < 26; i++) {
		priority['a' + i] = i + 1;
		priority['A' + i] = i + 27;
	}

	while (getline(&p, &sz, stdin) != -1 && *p && *p != '\n') {
		uint8_t first[128] = { 0 };
		uint8_t second[128] = { 0 };
		size_t len = strlen(p);
		for (size_t i = 0; i < len / 2; i++) {
			first[p[i]] = 1;
		}
		for (size_t i = len / 2; i < len - 1; i++) {
			if (first[p[i]] && !second[p[i]])
				total += priority[p[i]];
			second[p[i]] = 1;
		}
		for (size_t i = 0; i < 26; i++) {
			size_t c = i + 'a';
			if (first[c] || second[c])
				dups[c]++;
			c = i + 'A';
			if (first[c] || second[c])
				dups[c]++;
		}

		round++;
		if (round == 3) {
			for (size_t i = 0; i < 26; i++) {
				size_t c = i + 'a';
				if (dups[c] == 3)
					common += priority[c];
				c = i + 'A';
				if (dups[c] == 3)
					common += priority[c];
			}
			memset(dups, 0, 128);
			round = 0;
		}
	}
	printf("%llu %llu\n", total, common);
	return 0;
}
