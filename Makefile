CFLAGS = -g -Wall -pedantic
CXXFLAGS = -g -Wall -pedantic
ALLTARGET = day01 day02 day03 day04 day05 day08 day11 day12 \
	    day13 day14 day15 day19

ALLTARGET += goodstring

all: $(ALLTARGET)

clean:
	rm -f $(ALLTARGET)
