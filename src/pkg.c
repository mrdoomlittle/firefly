# include "pkg.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "dep/str_cpy.h"
# include "dep/mem_cpy.h"
# include "linux/limits.h"
# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "linux/stat.h"
# include "system/io.h"
char static dir[PATH_MAX];
#define PLAN_FILE "firefly.plan"
#define PLAN_FNL 13

#define is_space(__c) \
	(__c == ' ' || __c == '\n' || __c == '\t')
void ff_pkc_init(void) {

}

struct link {
	struct link *next;
	struct pkc_ingot *p;
};

static ff_u8_t *text;
static ff_uint_t cur;
static ff_uint_t txsz;

ff_u8_t static at_eof(void) {
	return cur>=txsz;
}

struct pkc_ingot static*
read_ingot(void) {
	ff_u8_t eo;
	while(is_space(*(text+cur)) && !(eo = at_eof()))
		cur++;
	
	if (eo || *(text+cur) == '#')
		return NULL;
	ff_u8_t *p;

	p = text+cur;
	char buf[PATH_MAX];
	char *bufp;

	bufp = buf;
_again:
	if (*p != ',') {
		*(bufp++) = *(p++);
		goto _again;
	}
	p++;
	*bufp = '\0';
	cur=(p-text);

	struct pkc_ingot *i;
	i = (struct pkc_ingot*)__ffly_mem_alloc(
		sizeof(struct pkc_ingot));

	i->plen = (bufp-buf);
	i->path = (char const*)__ffly_mem_alloc(i->plen+1);
	ffly_mem_cpy((void*)i->path, buf, i->plen+1);
	return i;
}

struct pkc_plan*
pkc_get_plan(void) {
	char file[PATH_MAX];
	char *p = file;
	p+=ffly_str_cpy(p, dir);
	ffly_mem_cpy(p, PLAN_FILE, PLAN_FNL);

	int fd;
	fd = open(file, O_RDONLY, 0);
	struct stat st;
	fstat(fd, &st);
	
	struct pkc_plan *plan;
	cur = 0;
	plan = (struct pkc_plan*)__ffly_mem_alloc(sizeof(struct pkc_plan));
	txsz = st.st_size;
	text = (ff_u8_t*)__ffly_mem_alloc(txsz);
	read(fd, text, txsz);
	struct link *l, *top = NULL;
	struct pkc_ingot *i;
	ff_uint_t n;

	n = 0;
_again:
	if (!(i = read_ingot())) {
		goto _end;
	}

	l = (struct link*)__ffly_mem_alloc(sizeof(struct link));

	l->p = i;
	l->next = top;
	top = l;
	n++;
	goto _again;
_end:
	plan->n = n;
	ffly_printf("N: %u\n", n);
	if (n>0) {
		plan->i = (struct pkc_ingot**)__ffly_mem_alloc(n*sizeof(struct pkc_ingot*));
	
		struct link *b;
		struct pkc_ingot **ip;
		ip = plan->i+n;
		l = top;
		while(l != NULL) {
			b = l;
			l = l->next;

			*(--ip) = (i = b->p);

			unsigned int mode;
			stat(i->path, &st);
			switch(st.st_mode&S_IFMT) {
				case S_IFREG:
					i->type = IG_FILE;
				break;
				case S_IFDIR:
					i->type = IG_DIR;
				break;
				default:
					i->type = IG_UNKNOWN;
			}

			__ffly_mem_free(b);

			ffly_printf("ingot: %s, type: %s\n", i->path, i->type == IG_FILE?"file":"directory");
		}
	} else
		plan->i = NULL;
	close(fd);
	return plan;
}

void ff_pkc_de_init(void) {

}

void ff_pkc_construct(char const*, char const*);
ff_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc<1){
		return 0;
	}

	char dc;
	dc = *__argv[1];
	if (dc == 'c') {
		ff_pkc_construct(NULL, "out.dat");
	} else if (dc == 'd') {
		ff_pkc_deconstruct(NULL, "out.dat");
	}
}
