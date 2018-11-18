# include "m.h"
# include "../../memory/mem_alloc.h"
# include "../../memory/mem_free.h"
# include "../../memory/mem_realloc.h"
static struct mfs_file **files = NULL;
static ff_u32_t off = 0;
struct mfs_file* mfs_file_new(void) {
	struct mfs_file *f;
	if (!files) {
		files = (struct mfs_file**)__ffly_mem_alloc(sizeof(struct mfs_file*));
		off++;
	} else {
		files = (struct mfs_file**)__ffly_mem_realloc(files, (++off)*sizeof(struct mfs_file*));
	}

	f = (*(files+(off-1)) = (struct mfs_file*)__ffly_mem_alloc(sizeof(struct mfs_file)));
	f->i = off-1;
	return f;
}

struct mfs_file* mfs_file_get(ff_u32_t __i) {
	return *(files+__i);
}

void mfs_file_destroy(struct mfs_file *__f) {
	__ffly_mem_free(__f);
}
