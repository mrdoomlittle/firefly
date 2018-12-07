#include "../ffint.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
struct frag {
	ff_uint_t sz;
	ff_int_t offset;
	ff_int_t label;

	struct frag *dst, *link;
	
	ff_int_t m, k;
	ff_int_t m0, k0;
	char const *ident;
	ff_int_t u;
};

#define FN 8
struct frag frags[] = {
	{20, 0, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31ma\e[0m", 0},
	{20, 20, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31mb\e[0m", 0},
	{20, 40, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31mc\e[0m", 0},
	{20, 60, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31md\e[0m", 0},
	{20, 80, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31me\e[0m", 0},
	{20, 100, 0, NULL, NULL, 0, 0, 0, 0,"\e[1;31mf\e[0m", 0},
	{20, 120, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31mg\e[0m", 0},
	{20, 140, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31mh\e[0m", 0}
};

ff_uint_t __n(long long __val) {
	ff_uint_t i;
	i = 0;
	while((__val&(0xffffffffffffffff<<i))>0)
		i++;
	return i;
}

void
prb(long long __val, ff_u8_t __b) {
	ff_u8_t i;
	ff_uint_t d, offset;
	char buf[128];

	offset = 0;
	i = 0;
	for(;i != __b;i++) {
		if (((i-d)>>2)) {
			d+=(i-d);
			buf[i+offset] = ' ';
			offset++;
		}
		buf[i+offset] = '0'+((__val>>((__b-1)-i))&0x0000000000000001);
	}
	*(buf+i+offset) = '\0';
	printf("%s", buf);
}

int main(void) {
	struct frag *a0, *a1, *b0, *b1;
	a0 = frags;
	a1 = frags+2;
	b0 = frags+7;
	b1 = frags+6;

	a0->dst = b0;
	a1->dst = b1;
	a0->label = 20;
	a1->label = 20;

	ff_uint_t m, k;
	struct frag *f;
	ff_uint_t i;

	i = 0;
	while(i != FN) {
		if ((f = frags+(i++))->dst != NULL) {
			f->link = f->dst->link;
			f->dst->link = f;
		}
	}

	m = 0;
	k = 0;
	i = 0;
	while(i != FN) {
		f = frags+(i++);
		f->m = m;
		f->k = k;
		if (f->dst != NULL) {
			ff_int_t dis;
			dis = (f->dst->offset+f->label)-f->offset;
			if (dis<0)
				dis = -dis;
			ff_uint_t n;
			n = __n(dis);
			m+=(n+((1<<3)-1))>>3;
		}
	}

	for(i = 0;i != FN;i++) {
		f = frags+i;
		printf("%s->%s: M: %u, K: %u\n", f->ident, !f->dst?"X":f->dst->ident, f->m, f->k);

	}
}
