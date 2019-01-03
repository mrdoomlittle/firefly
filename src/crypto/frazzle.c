# include "../crypto.h"
# include "../ffint.h"
void static
spook(ff_u8_t *__p, ff_u8_t __imn) {
	ff_u8_t byte;
	ff_u8_t l,r;
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
despook(ff_u8_t *__p, ff_u8_t __imn) {
	ff_u8_t byte;
	ff_u8_t l,r;
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

void ffly_frzz_enc(void *__p, ff_uint_t __bc, ff_u64_t __key) {
	ff_u8_t *p = (ff_u8_t*)__p;
	ff_u8_t *end = p+__bc;
	while(p != end) {
		spook(p, __key&0xf);
		*p ^= __key;
		spook(p, __key>>4&0xf);
		__key = (__key>>60)|__key<<4;
		p++;
	}
}

void ffly_frzz_dec(void *__p, ff_uint_t __bc, ff_u64_t __key) {
	ff_u8_t *p = (ff_u8_t*)__p;
	ff_u8_t *end = p+__bc;
	while(p != end) {
		despook(p, __key>>4&0xf);
		*p ^= __key;
		despook(p, __key&0xf);
		__key = (__key>>60)|__key<<4;
		p++;
	}
}

void static
frzz_enc(long long *__args) {
	ffly_frzz_enc((void*)*__args, __args[1], __args[2]);
}

void static
frzz_dec(long long *__args) {
	ffly_frzz_enc((void*)*__args, __args[1], __args[2]);
}

void static(*op[])(long long*) = {
	frzz_enc,
	frzz_dec
};

void frzz_op(struct crypto_op *__op) {
	op[__op->op](__op->args);
}
