# include "location.h"
# include "ffly_def.h"
typedef struct region {
	struct region *parent;
	struct region *child, *fd;
	ff_uint_t loc;
} *regionp;

struct loc_info {
	char const *label;
};

# define get_info(__n) \
	(info+(__n))
struct loc_info info[] = {
	{"mem_alloc"},
	{"mem_free"},
	{"mem_realloc"},
	{"stores_connect"},
	{"stores_login"},
	{"stores_logout"},
	{"stores_disconnect"},
	{"db_ctr"},
	{"net_creat"},
	{"net_connect"},
	{"db_ctr_login"}
};

# define MAX 46
struct region static list[MAX];
struct region static *cur = list;

static struct region *r = NULL;

char static pad[128];
char static *pd = pad;

# include "system/io.h"
# include "system/nanosleep.h"
void static prrg(regionp __reg) {
	ffly_nanosleep(0, 100000000);
	ffly_printf("%s, %s\n", pad, get_info(__reg->loc)->label);

	if (__reg->child != NULL) {
		*(pd++) = ' ';
		*pd = '\0';
		regionp p = __reg->child;
		while(p != NULL) {
			prrg(p);
			p = p->fd;
		}
		*(--pd) = '\0';
	}
}

void ff_location_list(void) {
	*pd = '\0';
	prrg(list);
}

void ff_location_show(void) {
}

void ff_location_push(ff_uint_t __loc) {
	if (cur>=list+MAX) {
		ffly_printf("error.\n");
		cur = list;
		return;
	}

	cur->parent = r;
	cur->child = NULL;
	cur->fd = NULL;
	if (r != NULL) {
		cur->fd = r->child;
		r->child = cur;
	}
	r = cur;
	cur->loc = __loc;
	cur++;
}

ff_uint_t static pc = 0;
void ff_location_pop(void) { 
	if ((cur-1)-pc == list) {
		cur = list;
		pc = 0;
		r = NULL;
		return;
	}
	r = r->parent;
	pc++;
}
