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

enum opcode {
	INVALID,
	ACC,
	JMP,
	NOP,
};

struct instruction {
	enum opcode opcode;
	int data;
	int visited;
	struct instruction *prev;
	struct instruction *next;
};

int starts_with(char *s, char *p)
{
	while (*p && *s++ == *p++);
	return !*p;
}

struct instruction *mkop(char *buffer) {
	enum opcode opcode;
	struct instruction *i;
	if (starts_with(buffer, "acc"))
		opcode = ACC;
	else if (starts_with(buffer, "jmp"))
		opcode = JMP;
	else if (starts_with(buffer, "nop"))
		opcode = NOP;
	else
		return NULL;
	i = malloc(sizeof(struct instruction));
	i->opcode = opcode;
	i->data = strtol(buffer + strlen("nop "), NULL, 10);
	i->visited = 0;
	return i;
}

struct instruction *advance(struct instruction *p, int l)
{
	if (l > 0)
		while (l--) p = p->next;
	else
		while (l++) p = p->prev;
	return p;
}

int main(int argc, char **argv)
{
	unsigned counter = 0;
	long long int total = 0;
	long long int base = 0;
	struct instruction root = { 0 };
	struct instruction *tail = &root;
	struct instruction *head = &root;

	char buffer[32];
	buffer[30] = 0;
	while (fgets(buffer, 30, stdin)) {
		struct instruction *i = mkop(buffer);
		i->next = NULL;
		i->prev = tail;
		tail->next = i;
		tail = i;
	}

	tail = root.next;
	counter++;
	while (tail && !tail->visited) {
		tail->visited = counter;
		if (tail->opcode == JMP) {
			tail = advance(tail, tail->data);
		} else {
			if (tail->opcode == ACC)
				total += tail->data;
			tail = tail->next;
		}
	}

	head = root.next;
	while (head) {
		while (head &&
		       (head->opcode == ACC ||
		       (head->opcode == NOP && head->data == 0))) {
		       	base += head->data;
		       	head = head->next;
		}
		total = base;
		tail = head;
		counter++;
		if (tail->opcode == NOP)
			tail = advance(tail, tail->data);
		else
			tail = tail->next;
		while (tail && tail->visited != counter) {
			tail->visited = counter;
			if (tail->opcode == JMP) {
				tail = advance(tail, tail->data);
			} else {
				if (tail->opcode == ACC)
					total += tail->data;
				tail = tail->next;
			}
		}
		if (!tail) {
			printf("%lld\n", total);
			return 0;
		}
		if (head->opcode == JMP)
			head = advance(head, head->data);
		else
			head = head->next;
	}

	return 1;
}
