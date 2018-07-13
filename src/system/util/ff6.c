# include "ff6.h"
//# include "../io.h"
ff_uint_t ffly_ff6_enc(void const *__src, char *__dst, ff_uint_t __len) {
	ff_u8_t const *in = (ff_u8_t const*)__src;
	ff_u8_t const *e = in+__len;
	char c0;
	ff_i16_t *dst = (ff_u16_t*)__dst;
	ff_u64_t buf;
	ff_u8_t v;

	ff_uint_t n;


	buf = 0;
	n = 0;
_again:
	buf = buf<<8|*in; 
	n+=8;
_bk:
	if (n>=6) {
		v = ((buf>>(n-6))&0x3f);
		c0 = 'a'+(v>>3);
		v-=((v>>3)*(1<<3));
		*(ff_u16_t*)dst = ('0'+v)<<8|c0;
		dst++;
		n-=6;
		goto _bk;
	}
	
	if (in<e) {
		in++;
		goto _again;
	}

	if (n>0) {
		v = (buf&(0xffffffffffffffff>>(64-n)));
		v = v<<(6-n);
		c0 = 'a'+(v>>3);
		v-=((v>>3)*(1<<3));
		*(ff_u16_t*)dst = ('0'+v)<<8|c0;
		dst++;
	}

	return (char*)dst-__dst;
}

ff_uint_t ffly_ff6_dec(char const *__src, void *__dst, ff_uint_t __len) {
	char c0, c1;
	char const *in = __src;
	char const *e = in+__len;
	ff_u8_t *dst = (ff_u8_t*)__dst;

	ff_u64_t buf;
	ff_uint_t n;

	buf = 0;
	n = 0;
_again:
	if(in<e) {
		c0 = *(in++);
		if (c0 < 'a' || c0 > 'h') {
//			printf("error.\n");
			goto _fail;
		}

		c1 = *(in++);
		if (c1 < '0' || c1 > '7') {
//			printf("error.\n");
			goto _fail;
		}
		
		buf = (buf<<6)|(((c0-'a')<<3)+(c1-'0'));
		n+=6;

		if (n>=8) {
			*(dst++) = buf>>(n-8);
			n-=8;
			buf &= 0xffffffffffffffff>>(64-n);
		}
		goto _again;
	}
	return dst-(ff_u8_t*)__dst;
_fail:
	return 0;
}
