# include "location.h"
# include "ffly_def.h"

/*
	tls not tested;
*/
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

# include "system/mutex.h"
ff_mlock_t static lock = FFLY_MUTEX_INIT;
# include "system/tls.h"

# define MAX 12
static ff_uint_t list_tls;
static ff_uint_t cur_tls;
static ff_uint_t r_tls;
static ff_uint_t pc_tls;
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

void static 
tls_init(void) {
	ffly_tls_set((ff_u64_t)ffly_tls_getp(list_tls), cur_tls);
	ffly_tls_set(0, r_tls);
	ffly_tls_set(0, pc_tls);
}

void ff_location_init(void) {
	list_tls = ffly_tls_alloc(MAX*sizeof(struct region));
	cur_tls = ffly_tls_alloc(sizeof(regionp));
	r_tls = ffly_tls_alloc(sizeof(regionp));
	pc_tls = ffly_tls_alloc(sizeof(ff_uint_t));
	ffly_tls_toinit(tls_init);
	tls_init();
}

void ff_location_list(void) {
	*pd = '\0';
	prrg(ffly_tls_getp(list_tls));
}

void ff_location_show(void) {
}

void ff_location_push(ff_uint_t __loc) {
return;
	regionp list = (regionp)ffly_tls_getp(list_tls);
	regionp cur = (regionp)ffly_tls_get(cur_tls);
	regionp r = (regionp)ffly_tls_get(r_tls);

	if (cur>=list+MAX) {
//		ffly_fprintfs(ffly_err, "error.\n");
		cur = list;
		goto _end;
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
	ffly_tls_set(r, r_tls);
_end:
	ffly_tls_set(cur, cur_tls);
}

void ff_location_pop(void) {
return;
	regionp list = (regionp)ffly_tls_getp(list_tls);
	regionp r = (regionp)ffly_tls_get(r_tls);
	regionp cur = (regionp)ffly_tls_get(cur_tls);
	ff_uint_t pc = ffly_tls_get(pc_tls);
	if ((cur-1)-pc == list) {
		cur = list;
		ffly_tls_set(cur, cur_tls);
		pc = 0;
		r = NULL;
		goto _end;
	}
	r = r->parent;
	pc++;
_end:
	ffly_tls_set(pc, pc_tls);
	ffly_tls_set(r, r_tls);
}
