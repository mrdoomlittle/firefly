#include "../ffint.h"
#include <stdio.h>
#include <string.h>

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
	{20, 100, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31mf\e[0m", 0},
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
//#define T0
//#define T1
int main(void) {
	struct frag *a0, *a1, *a2, *b0, *b1, *b2;
#ifdef T0
	a0 = frags+2;
	a1 = frags+5;
	b0 = frags+6;
	b1 = frags+1;

	a2 = frags;
	b2 = frags+7;
	a2->dst = b2;
	a0->label = 433400;
	a1->label = 4444444;
#endif
#ifdef T1
	a0 = frags;
	a1 = frags+1;
	b0 = frags+7;
	b1 = frags+6;
#endif
	a0->dst = b0;
	a1->dst = b1;

	char bf0[24], bf1[24];
	struct frag *f, *r;
	ff_uint_t i;
	i = 0;

	ff_int_t m, k;
	while(i != FN) {
		f = frags+(i++);
		if (f->dst != NULL) {
			f->link = f->dst->link;
			f->dst->link = f;
		}
	}
	ff_i8_t c = -1;
_again:
	m = 0;
	k = 0;
	i = 0;
	while(i != FN) {
		f = frags+(i++);
		f->m = m;
		f->k = k;
		memset(bf0, '#', m);
		bf0[m] = '\0';
		memset(bf1, '#', k);
		bf1[k] = '\0';
		printf("%s\t%s\t%s>%s", bf0, bf1, f->ident, !f->dst?"non":f->dst->ident);
		if (f->dst != NULL) {
			ff_int_t dis;
			dis = (f->dst->offset+f->label)-f->offset;
			if (dis<0)
				dis = -dis;
			ff_int_t x;
			x = f->dst->m-f->m;
			dis+=(x<0?-x:x)-f->u;
			ff_uint_t n, u, h;
			n = __n(dis);
			u = (n+((1<<3)-1))>>3;
		_again0:
			n = __n(dis+u);
			h = (n+((1<<3)-1))>>3;
			if (h != u) {
				u = h;
				goto _again0;
			}
			u = h;
			f->dst->m0 = u;
			if (f->u != u) {
				c = 0;
			}
			f->u = u;
			m+=u;
			printf(", dis: %d, %d", dis, u);
		}

		printf("\n");
	}

	i = 0;
	while(i != FN) {
		f = frags+(i++);
		if (f->dst != NULL) {
			printf("%s, %d\n", f->ident, f->dst->m-f->m);
		}
	}
	if (!c) {
		c = -1;
		usleep(1000000);
		goto _again;
	}
}
