%{
#include <stdio.h>
#include <stdlib.h>

#define xprintf printf

int x = 0;
int y = 0;
int wx = 10;
int wy = 1;

void (*command)(int);

enum {
	east = 0,
	south,
	west,
	north
} direction =  east;

void move_north(int step) {
	xprintf("%s: %d\n", __func__, step);
	wy += step;
}
void move_south(int step) {
	xprintf("%s: %d\n", __func__, step);
	wy -= step;
}
void move_east (int step) {
	xprintf("%s: %d\n", __func__, step);
	wx += step;
}
void move_west (int step) {
	xprintf("%s: %d\n", __func__, step);
	wx -= step;
}

void rright(int angle)
{
	int wt = wx;
	xprintf("%s: %d\n", __func__, angle);
	switch (3 &(angle / 90)) {
	case 1: wx = wy; wy = -wt; break;
	case 2: wx = -wx; wy = -wy; break;
	case 3: wx = -wy; wy = wt; break;
	}
}

void rleft(int angle)
{
	int wt = wx;
	xprintf("%s: %d\n", __func__, angle);
	switch (3 &(angle / 90)) {
	case 1: wx = -wy; wy = wt; break;
	case 2: wx = -wx; wy = -wy; break;
	case 3: wx = wy; wy = -wt; break;
	}
}

void forward(int step)
{
	x += step * wx;
	y += step * wy;
	return;
	switch (direction) {
	case east: move_east(step); break;
	case west: move_west(step); break;
	case south: move_south(step); break;
	case north: move_north(step); break;
	}
}

%}

%%

"N" { command = move_north; }
"S" { command = move_south; }
"E" { command = move_east; }
"W" { command = move_west; }
"L" { command = rleft; }
"R" { command = rright; }
"F" { command = forward; }
[0-9]+ { command(atoi(yytext)); }

%%

int yywrap() { return 1; }

int main() {
	unsigned ax, ay;
	(void)(input);
	(void)(yyunput);
	yylex();
	ax = x > 0 ? x : -x;
	ay = y > 0 ? y : -y;
	printf("%d %d %u\n", x, y, ax + ay);
	return 0;
}
