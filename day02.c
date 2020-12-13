#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YEAR 2020
#define BSIZE 1024

int main(int argc, char **argv)
{
	unsigned int l, h;
	char letter;
	char buffer[BSIZE];
	char *p;
#ifdef PART_ONE
	unsigned counter = 0;
#endif
	unsigned valid = 0;

	buffer[BSIZE - 2] = 0;
	while (fgets(buffer, BSIZE, stdin)) {
		/* FIXME */
		if (buffer[BSIZE - 2])
			return 1;
		l = strtoul(buffer, &p, 10);
		h = strtoul(p+1, &p, 10);
		letter = p[1];
		p = p+3;
#ifdef PART_ONE
		counter = 0;
		while (*p++ && counter <= h) {
			if (*p == letter)
				counter++;
		}
		if (l <= counter && counter <= h)
			valid++;
#else
		if ((strlen(p) > h) &&
		    ((p[l] == letter) ^ (p[h] == letter)))
			valid++;
#endif
	}
	printf("%u\n", valid);
	return 0;
}
