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
	ff_int_t u0, u1;
};

#define FN 8
struct frag frags[] = {
	{20, 0, 114, NULL, NULL, 0, 0, 0, 0, "\e[1;31ma\e[0m"},
	{20, 20, 200, NULL, NULL, 0, 0, 0, 0, "\e[1;31mb\e[0m"},
	{20, 40, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31mc\e[0m"},
	{20, 60, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31md\e[0m"},
	{20, 80, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31me\e[0m"},
	{20, 100, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31mf\e[0m"},
	{20, 120, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31mg\e[0m"},
	{20, 140, 0, NULL, NULL, 0, 0, 0, 0, "\e[1;31mh\e[0m"}
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
	a1 = frags+1;
	b0 = frags+7;
	b1 = frags+6;

	a0->dst = b0;
	a1->dst = b1;

	char buf[24];
	struct frag *f, *r;
	ff_uint_t i;
	i = 0;

	ff_int_t m, k;
	m = 0;
	k = 0;
	while(i != FN) {
		f = frags+(i++);
		f->m = m;
		f->k = k;
//		m-=f->m0;
//		k-=f->k0;
		
		memset(buf, '#', m);
		buf[m] = '\0';
		printf("%s\t\t%s>%s", buf, f->ident, !f->dst?"non":f->dst->ident);
		if (f->dst != NULL) {
			ff_int_t dis;
			dis = (f->dst->offset+f->label)-f->offset;
			if (dis<0)
				dis = -dis;
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
			m+=u;
			f->dst->m0 = u;
			f->u0 = u;
			f->link = f->dst->link;
			f->dst->link = f;
			printf(", dis: %d, %u", dis+u, u);
		}

		if (f->link != NULL) {
			r = f;
			f = f->link;
			while(f != NULL) {
				ff_int_t dis;
				dis = (r->offset+f->label)-f->offset;
				if (dis<0)
					dis = -dis;
				dis+=k;
				ff_uint_t n, u, h;
				n = __n(dis);
				u = (n+((1<<3)-1))>>3;
			_again1:
				n = __n(dis+u);
				h = (n+((1<<3)-1))>>3;
				if (h != u) {
					u = h;
					goto _again1;
				}
				u = h;
				k+=u;
				f->u1 = u;
				f->k0 = u;
				f = f->link;
			}
		}
		printf("\n");
	}

	i = 0;
	while(i != FN) {
		f = frags+(i++);

		if (f->dst != NULL) {
			if (f->u0 != f->u1) {
				printf("@");
			}
			printf("%s-%u,%u>", f->ident, f->u0, f->u1);
			if (f->offset<f->dst->offset) {
				printf("m: %d\n", f->dst->m-f->m);	
			} else {
				printf("k: %d\n", f->k-f->dst->k);
			}
		}
	}
}
