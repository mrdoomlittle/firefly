# include "types/err_t.h"
# include "linux/unistd.h"

# include "memory/allocr.h"
# include "system/io.h"
# include "system/thread.h"
# include "system/config.h"
# include "arcs.h"
# include "memory/alloca.h"
# include "dep/str_cmp.h"
# include "dep/mem_dup.h"
/*
extern void*(*ffly_allocp)(mdl_uint_t);
extern void(*ffly_freep)(void*);
extern void*(*ffly_reallocp)(void*, mdl_uint_t);
*/

typedef struct _ffopt {
	char const *name, *val;
	struct _ffopt *next;
} ffopt, *ffoptp;

# define ffopt_size sizeof(struct _ffopt)

ffopt static *optbed = NULL;
void evalopts(char const *__s) {
	char const *p = __s;
	char buf[128];
	char *bufp;
	ffoptp opt, end = NULL;
	_again:
	bufp = buf;
	opt = (ffoptp)__ffly_mem_alloc(ffopt_size);
	if (!optbed) 
		optbed = opt;
	if (end != NULL)
		end->next = opt;
	end = opt;
	while(*p == ' ') p++;
	while((*p >= 'a' && *p <= 'z') || *p == '-')
		*(bufp++) = *(p++);
	*bufp = '\0';
	ffly_mem_dup((void**)&opt->name, buf, (bufp-buf)+1);
	bufp = buf;

	if (*p == ':') {
		p++;
		while(*p == ' ') p++;
		while(*p != ',' && *p != '\0')
			*(bufp++) = *(p++);
		*bufp = '\0';
		ffly_mem_dup((void**)&opt->val, buf, (bufp-buf)+1);
		if (*p == ',') {
			p++;
			goto _again;
		}
	} else
		opt->val = NULL;
	end->next = NULL;
}

ffly_err_t ffmain(int, char const**);
char const static *by = "mrdoomlittle";
# include "data/str_cpy.h"
void _start(void) {
	int long argc;
	char const **argv;
	__asm__("mov 8(%%rbp), %0\t\n"
			"mov %%rbp, %%rax\n\t"
			"add $16, %%rax\n\t"
			"mov %%rax, %1" : "=r"(argc), "=r"(argv) : : "rax");
	char const **argp = argv;
	char const **end = argp+argc;
	void *frame;
	ffly_ar_init();
	frame = ffly_frame();
	void **p = ffly_alloca(sizeof(void*), NULL);
 	ffly_io_init();
	char const **argl = ffly_alloca(argc*sizeof(char const*), NULL);
	char const **arg = argl;
	*(arg++) = *(argp++); 

	mdl_i8_t conf = -1;
	if (argc > 1) {
		if (!ffly_str_cmp(*argp, "-proc")) {
			evalopts(*(++argp));
			argp++;
			ffly_trim(2*sizeof(char const*)); // alloca stack trim
		}

		ffoptp cur = optbed;
		while(cur != NULL) {
			ffly_printf("name: %s, val: %s\n", cur->name, !cur->val?"null":cur->val);
			if (!ffly_str_cmp(cur->name, "-sysconf")) {
				if (!cur->val) {
					ffly_printf("error.\n");
				}
				ffly_ld_sysconf(cur->val);
				ffly_printf("loaded sysconfig.\n");
				ffly_printf("max-threads: %u\n", __ffly_sysconf__.max_threads);
				conf = 0;
			}
			cur = cur->next;
		}
		//ffly_printf("please provide sysconf.\n");
		//goto _end;
	}

	while(argp != end)
		*(arg++) = *(argp++);

	ffly_arcs_init();

	*p = (void*)by;
	ffly_arcs_creatarc("info");
	ffly_arcs_tun("info");
	ffly_arcs_creatrec("created-by", p, _ffly_rec_def, 0);
	ffly_arcs_bk();

	ffmain(arg-argl, argl);
	ffly_printf("\n\n");

	if (!conf)
		ffly_free_sysconf();
	_end:
	{
		ffoptp cur = optbed, bk;
		while(cur != NULL) {
			bk = cur;
			cur = cur->next;
			__ffly_mem_free(bk->name);
			if (bk->val != NULL)
				__ffly_mem_free(bk->val);
			__ffly_mem_free(bk);
		}
	}

	ffly_arstat();
	ffly_arcs_de_init();
	ffly_io_closeup();
	ffly_collapse(frame);
	ffly_alloca_cleanup();
    ffly_ar_cleanup();
	exit(0);
}
