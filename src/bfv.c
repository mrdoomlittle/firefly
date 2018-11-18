# include "bh.h"
# include "fs/m/m.h"
# include "ffly_def.h"
# include "system/io.h"
# include "dep/mem_cpy.h"
# include "dep/mem_set.h"
# include "system/nanosleep.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "system/string.h"
#define BRICK_SHIFT _ff_brick_256
#define PAGE_SHIFT (BRICK_SHIFT+2)
#define PAGE_SIZE (1<<PAGE_SHIFT)
struct page {
	void *p;
};

/*
	NOT DONE.
*/

static ff_u32_t *bricks;
struct page static **pages = NULL;
static ff_uint_t page_c = 0;
struct page static* getpage(ff_uint_t __num) {
	if (__num>=page_c) {
		ff_unit_t prior;
		prior = page_c;
		page_c = __num+1;

		if (!pages)
			pages = (struct page**)__ffly_mem_alloc(page_c*sizeof(struct page*));
		else
			pages = (struct page**)__ffly_mem_alloc(page_c*sizeof(struct page*));

		ff_uint_t pg;
		pg = prior;
		while(pg != page_c)
			*(pages+(pg++)) = NULL;
	}

	struct page *pg, **pp;
	pp = pages+__num;
	if (!(pg = *pp))
		*pp = (struct page*)__ffly_mem_alloc(struct page);
	pg->p = __ffly_mem_alloc(PAGE_SIZE);


	return pg;	
}

void static __write(void *__buf, ff_uint_t __size, ff_u64_t __offset) {
}

void static __read(void *__buf, ff_uint_t __size, ff_u64_t __offset) {
  
}

# include "fs.h"
# include "dep/str_len.h"
# include "../system/util/ff5.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc<3) {
		return;
	}

	bricks = (ff_u32_t*)__ffly_mem_alloc(ffly_stno(__argv[1])*sizeof(ff_u32_t));
	ffly_ff5_dec(__argv[2], bricks, ffly_str_len(__argv[2]));

	ff_bh_open(&bh);
	ff_bh_connect(&bh, "192.168.0.111", 40960);

	struct ffly_mfs _mfs = {
		.write = __write,
		.read = __read,
		.top = NULL, .bin = NULL,
		.off = 0
	};
	mfs = &_mfs;

	void(*fs_fin)(void);
	fs_fin = ffly_fs(_ff_mfs);

	ff_u32_t f;

//	fs_mkdir("/test");
//	f = fs_open("/test/test.txt", MFS_CREAT);



//	fs_close(f);
	mfs_tree(mfs->root);
	fs_fin();

	ff_bh_disconnect(&bh);
	ff_bh_close(&bh);
	__ffly_mem_free(bricks);
}
