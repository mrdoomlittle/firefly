# include "tools.h" 
# include "system/io.h"
# include "dep/mem_cpy.h"
ff_u8_t pb_gouge = 2;
ff_u8_t static n = 0;
ff_uint_t static end;

char static _buf[256];
ff_uint_t static bn;
void static
deplete(void) {
	if (!--end) {
		_buf[bn++] = '\n';		
	} else
		_buf[bn++] = ' ';
	_buf[bn] = '\0';
	ffly_printf("%s", _buf);
	bn = 0;
}

void static
out(char *__buf) {
	ff_uint_t gc, desired;
_again:
	gc = bn>>pb_gouge;	
	desired = (1<<pb_gouge)-(bn-(gc*(1<<pb_gouge)));
	if (n>=desired) {
		ffly_mem_cpy(_buf+bn, __buf, desired);
		bn+=desired;
		n-=desired;
		__buf+=desired;
		deplete();
		if (!n)
			return;
		goto _again;
	}

	ffly_mem_cpy(_buf+bn, __buf, n);
	bn+=n;
}

void ffly_prbin(ff_u8_t const *__bin, ff_uint_t __n) {
	bn = 0;
	end = (__n+((1<<pb_gouge)-1))>>pb_gouge;
	ff_uint_t b;
	ff_u8_t const *p;
	char buf[8];
	b = __n>>3;
	p = __bin+b;
	ff_uint_t i;
	ff_u8_t bits;

	ff_u8_t bs;
	bs = __n-(b*8);
	if (bs>0) {
		i = 0;
		bits = *(__bin+b);
		for(;i != bs;i++) {
			buf[i] = '0'+((bits>>((bs-1)-i))&0x01);
		}
		n = bs;
		out(buf);
	}

	while(p != __bin) {
		bits = *(--p);
		*buf = '0'+((bits>>7)&0x01);
		buf[1] = '0'+((bits>>6)&0x01);
		buf[2] = '0'+((bits>>5)&0x01);
		buf[3] = '0'+((bits>>4)&0x01);
		buf[4] = '0'+((bits>>3)&0x01);
		buf[5] = '0'+((bits>>2)&0x01);
		buf[6] = '0'+((bits>>1)&0x01);
		buf[7] = '0'+(bits&0x01);
		n = 8;
		out(buf);
	}
	
	if (bn>0)
		deplete();
}
