# include "ffint.h"
# include "crypto.h"
# include "dep/str_cpy.h"
# include "dep/str_len.h"
# include "dep/mem_set.h"
# include "system/io.h"
void print_bin(ff_u8_t);

#define NUM 16
void ff_encrypt(void *__p, ff_uint_t __bc, ff_u64_t __key) {
	ff_u8_t *p = (ff_u8_t*)__p;
	ff_u8_t *end = p+__bc;
	ff_u8_t c;
	while(p != end) {
		c = NUM;
	_again:
		*p = ((*p&0x0f)^(__key>>4&0x0f))|(((*p>>4&0x0f)^(__key&0x0f))<<4);
		__key = (__key>>60)|__key<<4;
		if (c>0) {
			c--;
			goto _again;
		}
		p++;
	} 
}

void ff_decrypt(void *__p, ff_uint_t __bc, ff_u64_t __key) {
	ff_u8_t *p = (ff_u8_t*)__p;
	ff_u8_t *end = p+__bc;
	ff_u8_t c;
	while(p != end) {
		c = NUM;
	_again:
		*p = ((__key>>4&0x0f)^(*p&0x0f))|(((__key&0x0f)^(*p>>4&0x0f))<<4);
		__key = (__key>>60)|__key<<4;
		if (c>0) {
			c--;
			goto _again;
		}
		p++;
	}
}

void print_bin(ff_u8_t __d) {
	ff_u8_t i = 0;
	for (;i != 8;i++)
		ffly_printf("%u", __d>>(7-i)&0x01);
	ffly_printf("\n");
}

void(*ffly_encrypt)(void*, ff_uint_t, ff_u64_t) = ff_encrypt;
void(*ffly_decrypt)(void*, ff_uint_t, ff_u64_t) = ff_decrypt;
void frzz_op(struct crypto_op*);
void(*crypto_func[])(struct crypto_op*) = {
	frzz_op
};
/*
int main() {
	ffly_io_init();
	char const *c = "21299";
	char buf[40];
	ffly_mem_set(buf, '\0', 40);
	ffly_str_cpy(buf, c);
	ff_uint_t l = ffly_str_len(c);
	ff_encrypt(buf, l, 101987);
	ff_decrypt(buf, l, 101987);
	char *p = buf;
	while(p != buf+40 && *p != '\0') {
		ffly_printf("%c", *p);
		p++;
	}
	ffly_printf("\n");
	ffly_io_closeup();
}*/
