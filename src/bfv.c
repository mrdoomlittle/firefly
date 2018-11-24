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
# include "brick.h"
#define BRICK_SHIFT _ff_brick_256
#define PAGE_SHIFT (BRICK_SHIFT+2)
#define PAGE_SIZE (1<<PAGE_SHIFT)
struct page {
	void *p;
	ff_uint_t n;
};

/*
	NOT DONE.
*/

static ff_u32_t *bricks;
struct page static **pages = NULL;
static ff_uint_t page_c = 0;
struct page static* getpage(ff_uint_t __num) {
	if ((__num+1)>page_c) {
		ffly_printf("new pages: %u\n", __num+1);
		ff_uint_t prior;
		prior = page_c;
		page_c = __num+1;

		if (!pages)
			pages = (struct page**)__ffly_mem_alloc(page_c*sizeof(struct page*));
		else
			pages = (struct page**)__ffly_mem_realloc(pages, page_c*sizeof(struct page*));

		ff_uint_t pg;
		pg = prior;
		while(pg != page_c)
			*(pages+(pg++)) = NULL;

	}

	struct page *pg, **pp;
	pp = pages+__num;
	if (!(pg = *pp)) {
		pg = (*pp = (struct page*)__ffly_mem_alloc(sizeof(struct page)));
		pg->p = __ffly_mem_alloc(PAGE_SIZE);
		pg->n = __num;
	}
	return pg;	
}

void static 
pg_write(struct page *__page, void *__buf, ff_uint_t __offset, ff_uint_t __size) {
	ffly_mem_cpy(((ff_u8_t*)__page->p)+__offset, __buf, __size);
	ffly_printf("page write(%u): %u ^ %u\n", __page->n, __offset, __size);
}

void static
pg_read(struct page *__page, void *__buf, ff_uint_t __offset, ff_uint_t __size) {
	ffly_mem_cpy(__buf, ((ff_u8_t*)__page->p)+__offset, __size);
	ffly_printf("page read(%u): %u ^ %u\n", __page->n, __offset, __size);
}

void static __write(void *__buf, ff_uint_t __size, ff_u64_t __offset) {
	ff_uint_t pg, pg_off, n;
	pg = __offset>>PAGE_SHIFT;
	pg_off = __offset-(pg<<PAGE_SHIFT);
	struct page *pa;
	
	n = pg+(__size>>PAGE_SHIFT);
	ff_u8_t *p;
	p = (ff_u8_t*)__buf;
	// get rid of offset
	if (pg_off>0 && __size>=PAGE_SIZE) {
		ff_uint_t sz;
		sz = PAGE_SIZE-pg_off;
		pa = getpage(pg);
		pg_write(pa, p, pg_off, sz);
		pg++;
		pg_off = 0;
		p+=sz;
	}

	if (pg_off+__size>PAGE_SIZE && __size<PAGE_SIZE) {
		struct page *p0, *p1;
		p0 = getpage(pg);
		p1 = getpage(pg+1);
		ff_uint_t a0, a1;
		a0 = PAGE_SIZE-pg_off;
		a1 = __size-a0;

		pg_write(p0, p, pg_off, a0);
		pg_write(p1, p+a0, 0, a1);
		return;
	}

	while(pg != n) {
		pa = getpage(pg);
		pg_write(pa, p, 0, PAGE_SIZE);
		pg++;
		p+=PAGE_SIZE;
	}

	ff_uint_t left;
	if ((left = (__size-(p-(ff_u8_t*)__buf)))>0) {	
		pa = getpage(pg);
		pg_write(pa, p, pg_off, left);
	}
}

void static __read(void *__buf, ff_uint_t __size, ff_u64_t __offset) {
	ff_uint_t pg, pg_off, n;
	pg = __offset>>PAGE_SHIFT;
	pg_off = __offset-(pg<<PAGE_SHIFT);
	struct page *pa;
	n = pg+(__size>>PAGE_SHIFT);
	ff_u8_t *p;
	p = (ff_u8_t*)__buf;
	// get rid of offset
	if (pg_off>0 && __size>=PAGE_SIZE) {
		ff_uint_t sz;
		sz = PAGE_SIZE-pg_off;
		pa = getpage(pg);
		pg_read(pa, p, pg_off, sz);
		pg++;
		pg_off = 0;
		p+=sz;
	}

	if (pg_off+__size>PAGE_SIZE && __size<PAGE_SIZE) {
		struct page *p0, *p1;
		p0 = getpage(pg);
		p1 = getpage(pg+1);
		ff_uint_t a0, a1;
		a0 = PAGE_SIZE-pg_off;
		a1 = __size-a0;
		pg_read(p0, p, pg_off, a0);
		pg_read(p1, p+a0, 0, a1);
		return;
	}

	while(pg != n) {
		pa = getpage(pg);
		pg_read(pa, p, 0, PAGE_SIZE);
		pg++;
		p+=PAGE_SIZE;
	}

	ff_uint_t left;
	if ((left = (__size-(p-(ff_u8_t*)__buf)))>0) {
		pa = getpage(pg);
		pg_read(pa, p, pg_off, left);
	}
}

# include "string.h"
# include "fs.h"
# include "dep/str_len.h"
# include "system/util/ff5.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc<3) {
		return;
	}

	ff_uint_t nb;
	nb = ffly_stno(__argv[1]);
	bricks = (ff_u32_t*)__ffly_mem_alloc(nb*sizeof(ff_u32_t));
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

	ff_u8_t buf[(1<<_ff_brick_256)];

	ff_uint_t i;
	i = 0;
	while(i != nb) {
		ffly_printf("reading brick, %u\n", i);
		ff_bh_bread(&bh, *(bricks+i), buf, 1<<_ff_brick_256, 0);
		__write(buf, 1<<_ff_brick_256, i<<_ff_brick_256);
		i++;	
	}
	fs_fin = ffly_fs(_ff_mfs);

//	ff_u32_t f;

//	fs_mkdir("/test");
//	f = fs_open("/test/test.txt", MFS_CREAT);

//	ffly_printf("%u : %u\n", getpage(12)->n, getpage(13)->n);
/*
	char buf[128];
	strcpy(buf, "mrdoomlittle - firefly engine.\n");
	__write(buf, 128, (12<<PAGE_SHIFT)-20);
	memset(buf, 0, 128);
	__read(buf, 128, (12<<PAGE_SHIFT)-20);

	ffly_printf("## '%s'\n", buf);
*/

//	fs_close(f);
	mfs_tree(mfs->root);
	fs_fin();

	i = 0;
	while(i != nb) {
		ffly_printf("writing brick, %u\n", i);
		__read(buf, 1<<_ff_brick_256, i<<_ff_brick_256);
		ff_bh_bwrite(&bh, *(bricks+i), buf, 1<<_ff_brick_256, 0);
		i++;
	}

	ff_bh_disconnect(&bh);
	ff_bh_close(&bh);
	__ffly_mem_free(bricks);

}
