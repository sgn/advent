#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 4096
unsigned long findlcd(unsigned long lhs, unsigned long rhs)
{
	unsigned long lcd = lhs;
	if (rhs > lhs) {
		lhs = rhs;
		rhs = lcd;
		lcd = lhs;
	}
	while (lcd % rhs) lcd += lhs;
	return lcd;
}

int main() {
	char buffer[BUFSIZE];
	unsigned long earliest;
	unsigned long busid, delay = ULONG_MAX;
	unsigned long thisbus, thisdelay;
	char *next = buffer;
	unsigned long lcd = 1, newlcd;
	unsigned long fnr = ULONG_MAX, nr = 0;
	unsigned long long t = 0;
	int possible = 1;

	fgets(buffer, BUFSIZE, stdin);
	earliest = strtoul(buffer, NULL, 10);
	buffer[BUFSIZE - 2] = 0;
	*buffer = ',';
	fgets(buffer + 1, BUFSIZE - 1, stdin);
	if (buffer[BUFSIZE - 2])
		return 1;
	while (*next && *++next) {
		if (*next == 'x') {
			nr++;
			next++;
			continue;
		}
		thisbus = strtoul(next, &next, 10);
		thisdelay = thisbus - earliest % thisbus;

		if (delay > thisdelay) {
			delay = thisdelay;
			busid = thisbus;
		}

		if (fnr == ULONG_MAX)
			fnr = nr;
		newlcd = findlcd(lcd, thisbus);
		if (possible) {
			while (t < newlcd && ((t + nr - fnr) % thisbus))
				t += lcd;
			possible =  t < newlcd;
		}
		lcd = newlcd;
		nr++;
	}
	printf("%lu\n", delay * busid);
	printf("%llu\n", t);
	return 0;
}
