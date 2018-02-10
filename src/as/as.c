# include "as.h"
# include "../data/str_len.h"
# include "../ffly_def.h"
# include "../system/string.h"
# include "../elf.h"
# include <stdio.h>
# include "../data/str_cmp.h"
struct hash symbols;
struct hash globl;
mdl_u64_t offset = 0;
void* memdup(void *__p, mdl_uint_t __bc) {
	void *ret = _alloca(__bc);
	mdl_u8_t *p = (mdl_u8_t*)ret;
	mdl_u8_t *end = p+__bc;
	while(p != end) {
		*p = *((mdl_u8_t*)__p+(p-(mdl_u8_t*)ret));
		p++;
	}
	return ret;
}

char* read_str(char *__p, mdl_uint_t *__len) {
	char *p = __p;
	char buf[128];
	char *bufp = buf;
	while(*p >= 'a' && *p <= 'z') {
		*(bufp++) = *(p++);
	}
	*bufp = '\0';
	*__len = bufp-buf;
	return memdup(buf, (bufp-buf)+1);
}

mdl_uint_t read_no(char *__p, mdl_uint_t *__len) {
	char *p = __p;
	char buf[128];
	char *bufp = buf;
	while(*p >= '0' && *p <= '9') {
		*(bufp++) = *(p++);
	}

	*bufp = '\0';
	*__len = bufp-buf;

	return ffly_stno(buf);
}

# include <unistd.h>
char*
copyln(char *__dst, char *__src, char *__end, mdl_uint_t *__len) {
	char *p = __src;
	while(*p != '\n' && *p != '\0' && p < __end) {
		*(__dst+(p-__src)) = *p;
		p++;
	}
	*__len = p-__src;
	return p;
}

void
assemble(char *__p, char *__end) {
	char buf[256];
	char *p = __p;
	mdl_uint_t len;
	while(p < __end) {
		while (*p == ' ' && p < __end) p++;
		if (*p == '\0') break;

		p = copyln(buf, p, __end, &len)+1;
		*(buf+len) = '\0';

		symbolp sy;
		if ((sy = parse(buf)) != NULL) {
			if (is_flag(sy->flags, SY_MAC)) {
				if (!ffly_str_cmp(sy->p, "define")) {
					printf("got: macro\n");
				}
			} else {
				insp ins;

				if ((ins = (insp)hash_get(&globl, sy->p, sy->len))) {
					ins->post(ins);	
					printf("got: %s\n", ins->name);

				} else
					printf("unknown.\n");
			}
		}
	}
}

void oustbyte(mdl_u8_t __byte) {
	lseek(out, offset++, SEEK_SET);
	write(out, &__byte, 1);
}

void load(insp* __list) {
	insp* p = __list;
	char *name;
	while(*p != NULL) {
		name = (*p)->name;
		hash_put(&globl, name, ffly_str_len(name), *p);
		p++;
	}
}
