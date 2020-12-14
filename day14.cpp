#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

void xmask(const std::string& line,
	   uint64_t& mask0, uint64_t& mask1,
	   std::vector<std::pair<uint64_t, uint64_t>>& maskx)
{
	unsigned sv = 0;
	size_t sidx = line.find_first_of("01X", strlen("mask="));
	uint64_t xmask = 0;
	mask0 = 0;
	mask1 = 0;
	while (true) {
		sv++;
		switch (line[sidx++]) {
		case '0':
			mask0 = mask0 * 2;
			mask1 = mask1 * 2;
			xmask = xmask * 2;
			mask0++;
			break;
		case '1':
			mask0 = mask0 * 2;
			mask1 = mask1 * 2;
			xmask = xmask * 2;
			mask1++;
			break;
		case 'X':
			mask0 = mask0 * 2;
			mask1 = mask1 * 2;
			xmask = xmask * 2 + 1;
			break;
		default:
			goto maskout;
		}
	}
maskout:
	mask0 = ~mask0;
	sv = 0;
	maskx.clear();
	maskx.push_back(std::pair<uint64_t, uint64_t>(0, ~0ULL));
	while (xmask) {
		if (xmask & 1) {
			maskx.reserve(maskx.size() * 2);
			auto end = maskx.end();
			for (auto it = maskx.begin(); it != end; it++) {
				maskx.emplace_back(
					it->first | (1ULL << sv),
					it->second);
				it->second = it->second & (~(1ULL << sv));
			}
		}
		xmask = xmask / 2;
		sv++;
	}
}

int main() {
	std::unordered_map<uint64_t, uint64_t> memory;
	std::unordered_map<uint64_t, uint64_t> xmem;
	uint64_t mask0 = ~0ULL;
	uint64_t mask1 = 0;
	std::vector<std::pair<uint64_t, uint64_t>> maskx;
	std::string mask;
	std::string line;

	mask.reserve(36);
	line.reserve(64);
	while (getline(std::cin, line)) {
		if (line.compare(0, strlen("mask"), "mask") == 0) {
			xmask(line, mask0, mask1, maskx);
		} else if (line.compare(0, strlen("mem"), "mem") == 0) {
			/* ugly */
			size_t sidx = line.find_first_of('[') + 1;
			const char *next = line.c_str() + sidx;
			uint64_t idx = strtoull(next, (char **)&next, 10);
			do {
				next++;
			} while (*next < '0' || '9' < *next);
			uint64_t value = strtoull(next, (char **)&next, 10);
			memory[idx] = (value & mask0) | mask1;
			for (auto addr: maskx) {
				idx = idx | addr.first;
				idx = idx & addr.second;
				idx = idx | mask1;
				xmem[idx] = value;
			}
		}
	}
	uint64_t sum = 0;
	for (auto& pair: memory)
		sum += pair.second;
	printf("%" PRIu64 "\n", sum);
	sum = 0;
	for (auto& pair: xmem)
		sum += pair.second;
	printf("%" PRIu64 "\n", sum);
	return 0;
}
