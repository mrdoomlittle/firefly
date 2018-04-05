# include <mdlint.h>
void static
spook(mdl_u8_t *__p, mdl_u8_t __imn) {
	mdl_u8_t byte;
	mdl_u8_t l,r;
_again:
	byte = *__p;
	l = byte&0xe0;
	r = byte&0x7;
	*__p = ((((byte&0x8)<<4)|(l>>1))|((r<<1)|((byte&0x10)>>4)));
	if (__imn>0) {
		__imn--;
		goto _again;
	}
}

void static
despook(mdl_u8_t *__p, mdl_u8_t __imn) {
	mdl_u8_t byte;
	mdl_u8_t l,r;
_again:
	byte = *__p;
	l = byte&0x70;
	r = byte&0xe;
	*__p = ((((byte&0x80)>>4)|(l<<1))|((r>>1)|((byte&0x1)<<4)));
	if (__imn>0) {
		__imn--;
		goto _again;
	}
}

void ffly_frzz_enc(void *__p, mdl_uint_t __bc, mdl_u64_t __key) {
	mdl_u8_t *p = (mdl_u8_t*)__p;
	mdl_u8_t *end = p+__bc;
	while(p != end) {
		spook(p, __key&0xf);
		*p ^= __key;
		spook(p, __key>>4&0xf);
		__key = (__key>>60)|__key<<4;
		p++;
	}
}

void ffly_frzz_dec(void *__p, mdl_uint_t __bc, mdl_u64_t __key) {
	mdl_u8_t *p = (mdl_u8_t*)__p;
	mdl_u8_t *end = p+__bc;
	while(p != end) {
		despook(p, __key>>4&0xf);
		*p ^= __key;
		despook(p, __key&0xf);
		__key = (__key>>60)|__key<<4;
		p++;
	}
}