#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRNEQU(line, STR) \
	strncmp(line, STR, sizeof(STR) - 1) == 0

struct dirstat {
	size_t size;
	struct dirstat *children;
	size_t nr;
} dir_t;

size_t less;
char *line;
size_t sz = 1024;

size_t compute(struct dirstat *dir)
{
	ssize_t len;
	size_t el = 0;
	char *cmd = strdup(line);
	while ((len = getline(&line, &sz, stdin)) != -1) {
		line[len - 1] = '\0';
		if (!*line)
			break;
		if (STRNEQU(line, "$ ls"))
			continue;
		if (STRNEQU(line, "$ cd .."))
			break;
		if (STRNEQU(line, "$ cd ")) {
			if (!dir->children)
				dir->children =
					calloc(dir->nr, sizeof(*dir->children));
			dir->size += compute(&dir->children[el++]);
			continue;
		}
		if (STRNEQU(line, "dir ")) {
			dir->nr++;
			continue;
		}
		dir->size += strtoull(line, NULL, 10);
	}
	// printf("%s: %zu\n", cmd, dir->size);
	free(cmd);
	if (dir->size <= 100000)
		less += dir->size;
	return dir->size;
}

size_t check(struct dirstat *dir, size_t least)
{
	size_t torm = dir->size;
	if (torm == least)
		return torm;
	if (torm < least)
		return SIZE_MAX;
	for (size_t el = 0; el < dir->nr; el++) {
		size_t sub = check(&dir->children[el], least);
		if (sub == least)
			return least;
		if (sub < torm)
			torm = sub;
	}
	return torm;
}

int main(int argc, char **argv)
{
	size_t torm;
	ssize_t len;
	struct dirstat dir = { 0 };
	line = malloc(sz);

	while ((len = getline(&line, &sz, stdin)) != -1) {
		line[len - 1] = '\0';
		if (!*line)
			break;
		compute(&dir);
	}

	free(line);

	printf("%zu\n", less);

	torm = check(&dir, dir.size - (70000000 - 30000000));
	printf("%zu\n", torm);

	return 0;
}
