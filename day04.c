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

int main(int argc, char **argv)
{
	unsigned valid = 0;
	char buffer[BUFFER_SIZE];
	char *key, *data, *p;
	unsigned byr, iyr, eyr;
	char *hgt, *hcl, *ecl, *pid, *cid;

	buffer[BUFFER_SIZE - 2] = 0;
	byr = iyr = eyr = 0;
	hgt = hcl = ecl = pid = cid = NULL;
	while (fgets(buffer, BUFFER_SIZE, stdin)) {
		/* FIXME */
		if (buffer[BUFFER_SIZE - 2])
			return 1;
		if (!*buffer || *buffer == '\n') {
			if (byr && iyr && eyr && hgt && hcl && ecl && pid)
				valid++;
			byr = iyr = eyr = 0;
			hgt = hcl = ecl = pid = cid = NULL;
			continue;
		}
		p = buffer;
		while ((key = strtok(p, ":")) &&
		       (data = (strtok(NULL, " \n")))) {
		       	p = NULL;
		       	if (strcmp(key, "byr") == 0)
		       		parse_year(data, 1920, 2002, &byr);
			else if (strcmp(key, "iyr") == 0)
		       		parse_year(data, 2010, 2020, &iyr);
			else if (strcmp(key, "eyr") == 0)
		       		parse_year(data, 2020, 2030, &eyr);
			else if (strcmp(key, "hgt") == 0) {
				if (data = validate_hgt(data))
					hgt = data;
			} else if (strcmp(key, "hcl") == 0) {
				if (data = validate_hcl(data))
					hcl = data;
			} else if (strcmp(key, "ecl") == 0) {
				if (is_ecl(data))
					ecl = data;
			} else if (strcmp(key, "pid") == 0) {
				if (is_pid(data))
					pid = data;
			} else if (strcmp(key, "cid") == 0)
				cid = data;
		}
	}
	if (byr && iyr && eyr && hgt && hcl && ecl && pid)
		valid++;
	printf("%u\n", valid);
	return 0;
}
