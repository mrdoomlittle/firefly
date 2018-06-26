# include "exec.h"
# include "../dep/str_cmp.h"
# include "../version.h"
# include "../dep/mem_cmp.h"
# include "../system/util/ff6.h"
# include "../stdio.h"
# include "../string.h"
# include "../exec.h"
static char const *help = "commands:\n"
	"	help\n"
	"	exit\n"
	"	ff6 <enc,dec> <value>\n"
	"	exec <file>\n";

void _help();
void _exit();
void _info();
void _ff6();
void _exec();
void *cmds[] = {
	_help,
	_exit,
	_ff6,
	_info,
	_exec
};

void ffsh_cmdput(struct hash *__hash, char const *__ident, ff_uint_t __func) {
	hash_put(__hash, __ident, strlen(__ident), cmds[__func]);
}

void* ffsh_cmdget(struct hash *__hash, char const *__ident) {
	return hash_get(__hash, __ident, strlen(__ident));
}

# define jmpend __asm__("jmp _end")
extern ff_i8_t ffsh_run;
void ffsh_exec_cmd(void *__func, ff_uint_t __n, struct arg_s **__args) {
	char buf[128];

	__asm__("jmp *%0" : : "r"(__func));

	__asm__("_help:\n\t");
		printf("%s", help);
	jmpend;

	__asm__("_exit:\n\t");
		ffsh_run = -1;
	jmpend;

	__asm__("_ff6:\n\t"); {
		if (__n != 2) {
			ffly_printf("2 arguments needed.\n");
		} else {
			struct arg_s *mot = *__args;
			struct arg_s *val = *(__args+1);
			ff_uint_t l;
			if (mot->l == 4) {
				if (!ffly_mem_cmp(mot->p, "enc", mot->l-1)) {
					l = ffly_ff6_enc(val->p, buf, val->l);
					*(buf+l) = '\0';
					printf("enc, '%s':'%s'\n", val->p, buf);
				} else if (!ffly_mem_cmp(mot->p, "dec", mot->l-1)) {
					l = ffly_ff6_dec(val->p, buf, val->l);
					*(buf+l) = '\0';
					printf("dec, '%s':'%s'\n", val->p, buf);
				} else {
					printf("unknown.\n");
				}
			} else
				printf("length error.\n");
		}
	}
	jmpend;
	__asm__("_info:\n\t"); {
		printf("firefly version: %u-%s\n", ffly_version_int, ffly_version_str);
	}
	jmpend;
	__asm__("_exec:\n\t"); {
		if (__n>0) {
			struct arg_s *file = *__args;
			ffexecf(file->p);
		} else
			ffly_printf("please provide file.\n");
	}
	jmpend;
	__asm__("_end:\n\t");
}
