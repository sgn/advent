#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YEAR 2020
#define BUFFER_SIZE 1024

int is_pid(char *data) {
	return isdigit(data[0]) &&
		isdigit(data[1]) &&
		isdigit(data[2]) &&
		isdigit(data[3]) &&
		isdigit(data[4]) &&
		isdigit(data[5]) &&
		isdigit(data[6]) &&
		isdigit(data[7]) &&
		isdigit(data[8]) &&
		data[9] == 0;
}

void parse_year(char *raw, unsigned from, unsigned to, unsigned *dst) {
	char *dummy;
	unsigned value = strtoul(raw, &dummy, 10);
	if (!*dummy && value >= from && value <= to)
		*dst = value;
}

char *validate_hgt(char *hgt) {
	char *units;
	unsigned value = strtoul(hgt, &units, 10);
	if (strcmp(units, "cm") == 0) {
		if (150 <= value && value <= 193)
			return hgt;
	} else if (strcmp(units, "in") == 0) {
		if (59 <= value && value <= 76)
			return hgt;
	}
	return NULL;
}

char *validate_hcl(char *hcl) {
	if (*hcl == '#'
	    && isxdigit(hcl[1])
	    && isxdigit(hcl[2])
	    && isxdigit(hcl[3])
	    && isxdigit(hcl[4])
	    && isxdigit(hcl[5])
	    && isxdigit(hcl[6])
	    && hcl[7] == 0)
	    	return hcl;
	return NULL;
}

int is_ecl(char *ecl) {
	if (strcmp(ecl, "amb") &&
	    strcmp(ecl, "blu") &&
	    strcmp(ecl, "brn") &&
	    strcmp(ecl, "gry") &&
	    strcmp(ecl, "grn") &&
	    strcmp(ecl, "hzl") &&
	    strcmp(ecl, "oth"))
		return 0;
	return 1;
}

int decode(char *input)
{
	unsigned seat = 0;
	while (1) {
		switch (*input++) {
		case 'F':
		case 'L':
			seat <<= 1;
			break;
		case 'B':
		case 'R':
			seat <<= 1;
			seat++;
			break;
		default:
			return seat;
		}
	}
	return seat;
}

int main(int argc, char **argv)
{
	char buffer[BUFFER_SIZE];
	unsigned mseat = 0, seat;
	char mark[128*8] = { 0 };

	buffer[BUFFER_SIZE - 2] = 0;
	while (fgets(buffer, BUFFER_SIZE, stdin)) {
		/* FIXME */
		if (buffer[BUFFER_SIZE - 2])
			return 1;
		seat = decode(buffer);
		mark[seat] = 1;
		if (seat > mseat) mseat = seat;
	}
	for (seat = mseat; seat > 0 && mark[seat]; seat--);
	printf("%u\n", mseat);
	printf("%u\n", seat);
	return 0;
}
