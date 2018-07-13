# include "ff5.h"
ff_uint_t ffly_ff5_enc(void const *__src, char *__dst ,ff_uint_t __len) {
	ff_u64_t buf;
	ff_u8_t n;
	ff_u8_t const *in = (ff_u8_t const*)__src;
	ff_u8_t const *e = in+__len;
	char *dst = __dst;

	buf = 0;
	n = 0;
_again:
	if (n>=5) {
		ff_u8_t v;
		if ((v = (buf&0x1f))>=16)
			*(dst++) = 'A'+(v-16);
		else
			*(dst++) = 'a'+v;
		n-=5;
		buf>>=5;
		goto _again;
	}

	if (in<e) {
		buf = buf|(*(in++))<<n;
		n+=8;
		goto _again;
	}

	if (n>0) {
		buf &= 0xff>>(8-n);
		n = 5;
		goto _again;
	}
	return dst-__dst;
}

ff_uint_t ffly_ff5_dec(char const *__src, void *__dst, ff_uint_t __len) {
	char const *in = __src;
	char const *e = in+__len;
	ff_u8_t *dst = (ff_u8_t*)__dst;
	ff_u64_t buf;
	ff_u8_t n;

	buf = 0;
	n = 0;
	ff_u8_t v;
_again:
	if (n>=8) {
		*(dst++) = buf&0xff;
		buf>>=8;
		n-=8;
		goto _again;
	}

	if (in<e) {
		if ((v = *(in++)) >= 'a')
			buf |= (v-'a')<<n;
		else
			buf |= (16+(v-'A'))<<n;
		n+=5;
		goto _again;
	}

	if (n>=8)
		goto _again;

	return dst-(ff_u8_t*)__dst;
}
