# include "ffint.h"
# include "types.h"
# include "system/io.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "dep/str_dup.h"
# include "dep/str_len.h"
# include "system/map.h"
# include "dep/str_cmp.h"
# include "version.h"
# include "dep/mem_cmp.h"
# include "system/util/ff6.h"
struct param_s {
	char const *s;
	ff_u8_t l;
};

typedef struct dct {
	char const *ident;
	struct param_s params[20];
	ff_u8_t n;
} *dctp;

void static
skip_space(char **__p) {
	char *p = *__p;
	while(*p == ' ') p++;
	*__p = p;
}

dctp static
read_dct(char *__line) {
	dctp d = (dctp)__ffly_mem_alloc(sizeof(struct dct));
	char buf[128];
	char *bufp = buf;
	char *p = __line;
	while(*p != ' ' && *p != '\0')
		*(bufp++) = *(p++);
	*bufp = '\0';
	d->ident = ffly_str_dup(buf);
	struct param_s *param = d->params;
_again:
	if (*p != '\0') {
		skip_space(&p);
		bufp = buf;
		if (*p == '"') {
			p++;
			while(*p != '"' && *p != '\0')
				*(bufp++) = *(p++);
			p++;
		} else {
			while(*p != ' ' && *p != '\0')
				*(bufp++) = *(p++);
		}
		*bufp = '\0';
		param->s = ffly_str_dup(buf);
		param->l = bufp-buf;
		param++;
		goto _again;
	}
	d->n = param-d->params;
	return d;
}

void static
dct_free(dctp __dct) {
	__ffly_mem_free(__dct->ident);
	struct param_s *cur = __dct->params;
	struct param_s *end = cur+__dct->n;
	while(cur != end)
		__ffly_mem_free((cur++)->s);
	__ffly_mem_free(__dct);
}

static char const *help = "commands:\n"
	"	help\n"
	"	exit\n"
	"	ff6 <enc,dec> <value>\n";

void _help();
void _exit();
void _info();
void _ff6();
struct cmd {
	char const *ident;
	void(*p)();
};

struct cmd *cmdl[] = {
	&(struct cmd){"help", _help},
	&(struct cmd){"exit", _exit},
	&(struct cmd){"info", _info},
	&(struct cmd){"ff6", _ff6},
	NULL
};

void lddct(ffly_mapp __map) {
	struct cmd **cur = cmdl;
	while(*cur != NULL) {
		struct cmd *p = *cur;
		ffly_map_put(__map, p->ident, ffly_str_len(p->ident), p->p);
		cur++;
	}
}

char const *notice = "FF, ...\n";
void static exec_cmd(void*, ff_uint_t, struct param_s*);

ff_i8_t static run = 0;
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_printf("%s", notice);
	char line[256];
	char buf[128];
	dctp d;
	
	ff_err_t err;

	FF_MAP map;
	ffly_map_init(&map, _ffly_map_127);
	lddct(&map);
_again:

	ffly_printf("~: ");
	ffly_fdrain(ffly_out);
	ffly_rdline(line, 256, ffly_in);
	d = read_dct(line);
	void *to;
	to = ffly_map_get(&map, d->ident, ffly_str_len(d->ident), &err);
	if (!to || _err(err)) {
		ffly_printf("command none existent.\n");
		goto _again;	
	}

	exec_cmd(to, d->n, d->params);
	dct_free(d);
	if (!run)
		goto _again;

	ffly_map_de_init(&map);
}

# define jmpend __asm__("jmp _end")
void
exec_cmd(void *__func, ff_uint_t __n,  struct param_s *__params) {
	char buf[128];

	__asm__("jmp *%0" : : "r"(__func));

	__asm__("_help:\n\t");
		ffly_printf("%s", help);
	jmpend;

	__asm__("_exit:\n\t");
		run = -1;
	jmpend;

	__asm__("_ff6:\n\t"); {
		if (__n != 2) {
			ffly_printf("2 arguments needed.\n");
		} else {
			struct param_s *mot = __params;
			struct param_s *val = __params+1;
			ff_uint_t l;
			if (mot->l != 2) {
				if (!ffly_mem_cmp(mot->s, "enc", mot->l)) {
					l = ffly_ff6_enc(val->s, buf, val->l);
					*(buf+l) = '\0';
					ffly_printf("enc, '%s':'%s'\n", val->s, buf);
				} else if (!ffly_mem_cmp(mot->s, "dec", mot->l)) {
					l = ffly_ff6_dec(val->s, buf, val->l);
					*(buf+l) = '\0';
					ffly_printf("dec, '%s':'%s'\n", val->s, buf);
				} else {
					ffly_printf("unknown.\n");
				}
			} else
				ffly_printf("length error.\n");
		}
	}
	jmpend;
	__asm__("_info:\n\t"); {
		ffly_printf("version: %u\n", ffly_version_int);
	}
	jmpend;

	__asm__("_end:\n\t");
	return;
}
