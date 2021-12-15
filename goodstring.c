#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned *max_element(unsigned *block, size_t sz)
{
	unsigned *m = block;
	block++; sz--;
	while (sz--) {
		if (*block > *m)
			m = block;
		block++;
	}
	return m;
}

char *findlargest(const char *input)
{
	unsigned len = strlen(input);
	const char *pinput = input;
	char *p, *out;
	unsigned *block1, *block0;
	size_t nb = 0;
	size_t i, i0, i1, imax;
	unsigned *lmax;
	unsigned delta0 = 0;
	unsigned delta1 = 0;
	unsigned *tblock1, *tblock0;
	size_t lb, la;

	if (len <= strlen("10"))
		return strndup(input, len);

	/* (10)+ */
	block0 = calloc(len / 2, sizeof(size_t));
	block1 = calloc(len / 2, sizeof(size_t));
	pinput = input;
	while (*pinput) {
		block1[nb] = 0;
		block0[nb] = 0;
		for (;*pinput == '1';pinput++)
			block1[nb]++;
		for (;*pinput == '0';pinput++)
			block0[nb]++;
		nb++;
	}
	tblock0 = calloc(nb, sizeof(size_t));
	tblock1 = calloc(nb, sizeof(size_t));

	i = 0;
	while (i < nb - 1) {
		lmax = max_element(block1 + i, nb - i);
		if (lmax == block1 + i) {
			i++;
			continue;
		}
		imax = lmax - block1;
		delta0 = delta1 = 0;
		for (i1 = lmax - block1; i1 > i;) {
			i1--;
			if (delta1 + block0[i1] >= block1[i1]) {
				delta1 += (block0[i1] - block1[i1]);
			} else {
				delta1 += block0[i1];
				delta1 = block1[i1] - delta1;
				break;
			}
		}
		/* 111110.100001100100 */
		if (block1[i1] < delta1) {
			i++;
			continue;
		}
		block1[i1] -= delta1;
		lb = imax - i1;
		/* 101'110010|1111010000'010 */
		for (i0 = lmax - block1; i0 < nb; i0++) {
			if (delta0 + block1[i0] >= block0[i0]) {
				delta0 += (block1[i0] - block0[i0]);
			} else {
				delta0 += block1[i0];
				delta0 = block0[i0] - delta0;
				i0++;
				break;
			}
		}
		block0[i0 - 1] -= delta0;
		la = i0 - imax;
		memcpy(tblock1, block1 + i1, lb * sizeof(unsigned));
		memcpy(tblock0, block0 + i1, lb * sizeof(unsigned));
		memmove(block1 + i1, block1 + imax, la * sizeof(unsigned));
		memmove(block0 + i1, block0 + imax, la * sizeof(unsigned));
		memcpy(block1 + i1 + la, tblock1, lb * sizeof(unsigned));
		memcpy(block0 + i1 + la, tblock0, lb * sizeof(unsigned));
		block1[i1] += delta1;
		block0[i0 - 1] += delta0;
	}
	p = out = malloc(len + 1);
	for (i = 0; i < nb; i++) {
		for (i1 = block1[i]; i1; i1--)
			*p++ = '1';
		for (i0 = block0[i]; i0; i0--)
			*p++ = '0';
	}
	*p = 0;
	free(tblock1);
	free(tblock0);
	free(block1);
	free(block0);
	return out;
}

int main(int argc, char **argv)
{
	char *output;
	argc--; argv++;
	while (argc--) {
		output = findlargest(*argv++);
		puts(output);
		free(output);
	}
	return 0;
}
