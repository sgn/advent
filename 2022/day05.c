#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CRATE_LEN 128

struct stack {
	size_t len;
	char *crates;
};

int main(int argc, char **argv)
{
	char top[32] = { 0 };
	struct stack s[32] = { 0 };
	struct stack n[32] = { 0 };
	size_t sz = 1024;
	char *line = malloc(sz);
	size_t len;

	for (size_t i = 0; i < 32; i++)
		s[i].crates = malloc(CRATE_LEN);

	while ((len = getline(&line, &sz, stdin)) != -1) {
		// This is stack name line
		if (line[0] == ' ' && line[1] != ' ')
			break;
		size_t col = 1;
		for (size_t i = 1; i < len; i += 4, col++)
			if (line[i] != ' ')
				s[col].crates[s[col].len++] = line[i];
	}

	// and empty line
	getline(&line, &sz, stdin);

	// Reverse the stack
	for (size_t i = 0; i < 32; i++) {
		char *crates = s[i].crates;
		size_t len = s[i].len;
		for (size_t j = 0; j < len / 2; j++) {
			char c = crates[j];
			crates[j] = crates[len - 1 - j];
			crates[len - 1 - j] = c;
		}
		crates[len] = 0;
		n[i].len = s[i].len;
		n[i].crates = calloc(CRATE_LEN, 1);
		strcpy(n[i].crates, s[i].crates);
	}
	while (getline(&line, &sz, stdin) != -1 && *line == 'm') {
		char *next;
		unsigned long num = strtoul(line + 5, &next, 10);
		unsigned long from = strtoul(next + 6, &next, 10);
		unsigned long to = strtoul(next + 4, &next, 10);
		n[from].len -= num;
		for (int i = 0; i < num; i++) {
			s[to].crates[s[to].len++] = s[from].crates[--s[from].len];
			n[to].crates[n[to].len++] = n[from].crates[n[from].len + i];

		}
	}

	for (size_t i = 0; i < 32; i++) {
		if (s[i].len)
			putchar(s[i].crates[s[i].len - 1]);
	}
	puts("");
	for (size_t i = 0; i < 32; i++) {
		if (n[i].len)
			putchar(n[i].crates[n[i].len - 1]);
	}
	puts("");
	return 0;
}
