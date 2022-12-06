#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void find_substr(const char *line, size_t len, size_t sublen)
{
	size_t start = 1;
	size_t map[128] = { 0 };

	map[line[0]] = 1;
	for (size_t i = 1; i < len; ++i) {
		// this character exists in substr
		if (map[line[i]] >= start)
			start = map[line[i]] + 1;
		else if (i + 1 - start > sublen - 2)
			break;
		map[line[i]] = i + 1;
	}
	printf("%zu\n", start + sublen - 1);
}

int main(int argc, char **argv)
{
	size_t sz = 1024;
	char *line = malloc(sz);
	size_t len;

	len = getline(&line, &sz, stdin);
	find_substr(line, len, 4);
	find_substr(line, len, 14);

	return 0;
}
