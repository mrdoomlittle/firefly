# include "dep/str_cpy.h"
# include "dep/str_len.h"
# include "dep/mem_set.h"
# include "system/io.h"
void print_bin(mdl_u8_t);


# define NUM 16
void mdl_encrypt(void *__p, mdl_uint_t __bc, mdl_u64_t __key) {
	mdl_u8_t *p = (mdl_u8_t*)__p;
	mdl_u8_t *end = p+__bc;
	mdl_u8_t c;
	while(p != end) {
		c = NUM;
_again:
		*p = ((*p&0xf)^(__key>>4&0xf))|(((*p>>4&0xf)^(__key&0xf))<<4);
		__key = (__key>>60)|__key<<4;
		if (c>0) {
			c--;
			goto _again;
		}
		p++;
	} 
}

void mdl_decrypt(void *__p, mdl_uint_t __bc, mdl_u64_t __key) {
	mdl_u8_t *p = (mdl_u8_t*)__p;
	mdl_u8_t *end = p+__bc;
	mdl_u8_t c;
	while(p != end) {
		c = NUM;
_again:
		*p = ((__key>>4&0xf)^(*p&0xf))|(((__key&0xf)^(*p>>4&0xf))<<4);
		__key = (__key>>60)|__key<<4;
		if (c>0) {
			c--;
			goto _again;
		}
		p++;
	}
}

void print_bin(mdl_u8_t __d) {
	mdl_u8_t i = 0;
	for (;i != 8;i++)
		ffly_printf("%u", __d>>(7-i)&0x1);
	ffly_printf("\n");
}

void(*ffly_encrypt)(void*, mdl_uint_t, mdl_u64_t) = mdl_encrypt;
void(*ffly_decrypt)(void*, mdl_uint_t, mdl_u64_t) = mdl_decrypt;

/*
int main() {
	ffly_io_init();
	char const *c = "21299";
	char buf[40];
	ffly_mem_set(buf, '\0', 40);
	ffly_str_cpy(buf, c);
	mdl_uint_t l = ffly_str_len(c);
	mdl_encrypt(buf, l, 101987);
	mdl_decrypt(buf, l, 101987);
	char *p = buf;
	while(p != buf+40 && *p != '\0') {
		ffly_printf("%c", *p);
		p++;
	}
	ffly_printf("\n");
	ffly_io_closeup();
}*/
