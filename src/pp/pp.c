# include "pp.h"
# include "../malloc.h"
# include "../linux/unistd.h"
# include "../stdio.h"
# include "../string.h"
# include "../system/lat.h"
# include "../system/util/hash.h"
ff_u8_t *p = NULL, *end = NULL;
int src, dst;

struct ffly_lat defines;

ff_u8_t at_eof() {
	return p >= end;
}

# define OBUFSZ 128

typedef struct def {
	struct def *next;
	ff_u8_t *beg, *end;	
	char *name;
} *defp;

struct {
	ff_u8_t bed[OBUFSZ];
	ff_u8_t *p, *end;
} obuf;

void oust(ff_u8_t __val) {
	if (obuf.p >= obuf.end) {
		obuf.p = obuf.bed;
		write(dst, obuf.bed, OBUFSZ);
	}

	*(obuf.p++) = __val;
}

void flushbuf() {
	if (obuf.p > obuf.bed)
		write(dst, obuf.bed, obuf.p-obuf.bed);
}

ff_u8_t* getp() {
	return p;
}

void sk_endif() {
	bucketp p;
	while((p = lex()) != NULL) {
		if (p->sort == _percent) {
			if (!strcmp(lex()->p, "endif"))
				break;
		}
	}
}

void define(char *__name, ff_u64_t __key, ff_u8_t *__beg, ff_u8_t *__end) {
	defp df = (defp)malloc(sizeof(struct def));
	df->name = strdup(__name);
	df->beg = __beg;
	df->end = __end;
	df->next = (defp)ffly_lat_get(&defines, __key);
	ffly_lat_put(&defines, __key, df);
	printf("key: %lu\n", __key);
}

void prep() {
	ffly_lat_prepare(&defines);
}

void prossess() {
	obuf.p = obuf.bed;
	obuf.end = obuf.bed+OBUFSZ;
	bucketp p;
	while((p = lex()) != NULL) {
		if (p->sort == _percent) {
			if ((p = lex())->sort != _ident) {
				printf("error.\n");
				break;
			}
			ff_u8_t dir;
			if (!strcmp(p->p, "define"))
				dir = _define;
			else if (!strcmp(p->p, "ifdef"))
				dir = _ifdef;
			else if (!strcmp(p->p, "ifndef"))
				dir = _ifndef;
			else {
				printf("error, %s\n", p->p);
				break;
			}
			char buf[128];
			char *bufp = buf;
			sk_space();
			_again:
			if (!(p = lex()))
				break;
			if ((p->val >= 'a' && p->val <= 'z') || (p->val >= '0' && p->val <= '9') || p->val == '_') {
				*(bufp++) = p->val;
				goto _again;
			}
			ulex(p);

			*bufp = '\0';
			ff_u64_t sum = ffly_hash(buf, bufp-buf);
			printf("key: %lu\n", sum);
			if (dir == _ifdef || dir == _ifndef) {
				defp df = (defp)ffly_lat_get(&defines, sum);
				while(df != NULL) {
					if (!strcmp(df->name, buf)) break;
					df = df->next;
				}
				printf("%s, %s\n", buf, !df?"no":"yes");
				if (dir == _ifdef && !df)
					sk_endif();
				else if (dir == _ifndef && df != NULL)
					sk_endif();
				else {
					while((p = lex()) != NULL) {
						if (p->sort == _percent) {
							if (!strcmp(lex()->p, "endif"))
								break;
						}
						oust(p->val);
					}
				}
				continue;
			}

			ff_u8_t *beg, *end;
/*
			beg = getp();
			while((p = lex()) != NULL)
				if (p->val == '\n' || at_eof()) break;
			end = getp();
*/
			define(buf, sum, beg, end);
		} else
			oust(p->val);
	}
/*
	void *cur = ffly_lat_head(&defines);
	while(cur != NULL) {
		defp df = (defp)ffly_lat_getp(cur);	

		free(df->name);
		free(df);

		ffly_lat_fd(&cur);
	}
*/
}

void finish() {
	ffly_lat_free(&defines);
}
