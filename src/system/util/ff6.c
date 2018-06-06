# include "ff6.h"
ff_uint_t ffly_ff6_enc(void *__src, char *__dst, ff_uint_t __len) {
	ff_u8_t *p = (ff_u8_t*)__src;
	ff_u8_t *end = p+__len;
	char c0;
	ff_i16_t *dst = (ff_u16_t*)__dst;
	ff_u64_t buf = 0;
	ff_u8_t v;

	ff_uint_t n = 0;
_again:
	buf = buf<<8|*p; 
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
	
	if (p != end-1) {
		p++;
		goto _again;
	}

	if (n>0) {
		v = (buf>>(n-6)&0x3f);
		c0 = 'a'+(buf>>3);
		buf-=((buf>>3)*(1<<3));
		*(ff_u16_t*)dst = ('0'+buf)<<8|c0;
		dst++;
	}

	return (char*)dst-__dst;
}

void ffly_ff6_dec(char *__src, void *__dst, ff_uint_t __len) {
	char c0, c1;
	char *p = __src;
	char *end = p+__len;
	ff_u8_t *dst = (ff_u8_t*)__dst;

	ff_u64_t buf = 0;
	ff_uint_t n = 0;
	while(p != end) {
		c0 = *(p++);
		if (c0 < 'a' || c0 > 'h') {
//			printf("error.\n");
			break;
		}

		c1 = *(p++);
		if (c1 < '0' || c1 > '7') {
//			printf("error.\n");
			break;
		}
		
		buf = (buf<<6)|(((c0-'a')<<3)+(c1-'0'));
		n+=6;

		if (n>=8) {
			*(dst++) = buf>>(n-8);
			n-=8;
			buf &= 0xffffffffffffffff>>(64-n);
		}
	}

	if (n>0)
		*(dst++) = buf>>(n-8);
}
