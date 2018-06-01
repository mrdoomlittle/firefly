# include "location.h"
# include "ffly_def.h"
// my parent, my child
typedef struct region {
	struct region *parent;
	struct region *child, *fd;
	ff_uint_t loc;
} *regionp;

struct loc_info {
	char const *label;
	char const *file;
};

# define get_info(__n) \
	(info+(__n))
struct loc_info info[] = {
	{"mem_alloc", "src/memory/mem_alloc.c"},
	{"mem_free", "src/memory/mem_free.c"},
	{"mem_realloc", "src/memory/mem_realloc.c"},
	{"stores_connect", "src/stores.c"},
	{"stores_login", "src/stores.c"},
	{"stores_logout", "src/stores.c"},
	{"stores_disconnect", "src/stores.c"},
	{"db_ctr", "src/db/connect.c"},
	{"net_creat", "src/net.c"},
	{"net_connect", "src/net.c"},
	{"db_ctr_login", "src/db/connect.c"}
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
	struct loc_info *inf = get_info(__reg->loc);
	ffly_printf("%s` %s : %s\n", pad, inf->label, inf->file);

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
