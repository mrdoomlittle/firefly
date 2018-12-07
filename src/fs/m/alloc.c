# include "m.h"
# include "../../memory/mem_alloc.h"
# include "../../memory/mem_free.h"
# include "../../memory/mem_realloc.h"
# include "../../system/io.h"

#define PAGE_SHIFT 4
#define PAGE_SIZE (1<<PAGE_SHIFT)


struct mfs_slab **slabs = NULL;
static ff_u32_t page_c = 0;
static ff_u64_t off = 0;


struct mfs_slab static* slab_alloc(void);
struct mfs_slab* slabat(ff_u32_t);

void mfs_slabs_save(ff_uint_t *__n, ff_u32_t __to) {
	ff_u32_t *slabs;

	slabs = (ff_u32_t*)__ffly_mem_alloc(off*sizeof(ff_u32_t));
	ff_uint_t i;

	ffly_printf("slab save\n");
	i = 0;
	for(;i != off;i++) {
		slabs[i] = slabat(i)->off;
		ffly_printf("slab, save: %u\n", slabs[i]);
	}

	mfs->write(slabs, off*sizeof(ff_u32_t), __to);
	__ffly_mem_free(slabs);
	*__n = off;
}

void mfs_slabs_load(ff_uint_t __n, ff_u32_t __from) {
	ff_uint_t i;

	ff_u32_t *slabs;
	slabs = (ff_u32_t*)__ffly_mem_alloc(__n*sizeof(ff_u32_t));
	mfs->read(slabs, __n*sizeof(ff_u32_t), __from);

	ffly_printf("slab load.\n");

	i = 0;
	while(i != __n) {
		slab_alloc()->off = slabs[i];
		ffly_printf("slab, load: %u\n", slabs[i]);
		i++;
	}

	__ffly_mem_free(slabs);
}

struct mfs_slab*
slabat(ff_u32_t __of) {
	ff_u32_t pg, pg_off;
	pg = __of>>PAGE_SHIFT;
	pg_off = __of-(pg*PAGE_SIZE);
	return (*(slabs+pg))+pg_off;
}

void static
deattach(struct mfs_slab *__sb) {
	if (__sb == mfs->top) {
		if ((mfs->top = __sb->next) != NULL)
			mfs->top->prev = NULL;
		return;
	}

	if (__sb->prev != NULL)
		__sb->prev->next = __sb->next;
	if (__sb->next != NULL)
		__sb->next->prev = __sb->prev;
}

void static
delink(struct mfs_slab *__sb) {
	*__sb->bk = __sb->fd;
	if (__sb->fd != NULL)
		__sb->fd->bk = __sb->bk;
}

ff_u32_t mfs_balloc(void) {
	ff_u32_t r;
	r = mfs->off;
	mfs->off+=(sizeof(struct mfs_bale)+(MFS_SLAB_SIZE-1))>>MFS_SLAB_SHIFT;
	return r;
}

struct mfs_slab* slab_alloc(void) {
	struct mfs_slab *sb;
	ff_u32_t pg, pg_off;

	if (mfs->bin != NULL) {
		sb = mfs->bin; 
		if ((mfs->bin = sb->fd) != NULL)
			mfs->bin->bk = sb->bk;
		goto _done;
	}

	pg = off>>PAGE_SHIFT;
	pg_off = off-(pg*PAGE_SIZE);
	if (!slabs) {
		slabs = (struct mfs_slab**)__ffly_mem_alloc(sizeof(struct mfs_slab*));
		page_c++;
	} else {
		if (pg>=page_c) {
			slabs = (struct mfs_slab**)__ffly_mem_realloc(slabs, (++page_c)*sizeof(struct mfs_slab*));
		} else
			goto _sk;
	}

	*(slabs+pg) = (struct mfs_slab*)__ffly_mem_alloc(PAGE_SIZE*sizeof(struct mfs_slab));
_sk:
	sb = (*(slabs+pg))+pg_off;
	sb->in = off++;

	sb->off = mfs->off++;
  
	sb->prev = NULL;
	sb->next = mfs->top;
	if (mfs->top != NULL)
		mfs->top->prev = sb;
	mfs->top = sb;
	sb->fd = NULL;
	sb->bk = NULL;
	sb->flags = 0x00;
_done:
	sb->flags |= MFS_SLAB_INUSE;
	return sb;
}

void static slab_free(struct mfs_slab *__sb) {
	__sb->fd = mfs->bin;
	__sb->bk = &mfs->bin;
	if (mfs->bin != NULL)
		mfs->bin->bk = &__sb->fd;
	mfs->bin = __sb;
	__sb->flags ^= MFS_SLAB_INUSE;
}

struct mfs_scope* mfs_alloc(ff_uint_t __size) {
	struct mfs_scope *sc;

	sc = (struct mfs_scope*)__ffly_mem_alloc(sizeof(struct mfs_scope));

	ff_uint_t slabs;
	slabs = (__size+(0xffffffffffffffff>>(64-MFS_SLAB_SHIFT)))>>MFS_SLAB_SHIFT;

	sc->slabs = (struct mfs_slab**)__ffly_mem_alloc(slabs*sizeof(struct mfs_slab*));
	ff_uint_t i;
	i = 0;
	for(;i != slabs;i++)
		sc->slabs[i] = slab_alloc();
	sc->slab_c = slabs;
	return sc;
}

struct mfs_scope* mfs_build(ff_u32_t *__slabs, ff_uint_t __n) {
	struct mfs_scope *sc;
	sc = (struct mfs_scope*)__ffly_mem_alloc(sizeof(struct mfs_scope));


	sc->slabs = (struct mfs_slab**)__ffly_mem_alloc(__n*sizeof(struct mfs_slab*));
	ff_uint_t i;
	i = 0;
	for(;i != __n;i++)
		sc->slabs[i] = slabat(__slabs[i]);
	sc->slab_c = __n;
	return sc;
}

void mfs_dmscope(struct mfs_scope *__s) {
	__ffly_mem_free(__s->slabs);
	__ffly_mem_free(__s);
}

void mfs_resize(struct mfs_scope *__sc, ff_uint_t __size) {
	ffly_printf("resize.\n");
	ff_uint_t slabs;
	slabs = (__size+(0xffffffffffffffff>>(64-MFS_SLAB_SHIFT)))>>MFS_SLAB_SHIFT;

	ff_uint_t i;
	if (slabs>__sc->slab_c) {
		__sc->slabs = (struct mfs_slab**)__ffly_mem_realloc(__sc->slabs, slabs*sizeof(struct mfs_slab*));
		i = __sc->slab_c;
		for(;i != slabs;i++)
			__sc->slabs[i] = slab_alloc();
		__sc->slab_c = slabs;
	}
}

void mfs_free(struct mfs_scope *__sc) {
	ff_uint_t i;

	i = 0;
	for(;i != __sc->slab_c;i++)
		slab_free(__sc->slabs[i]);
	__ffly_mem_free(__sc->slabs);
	__ffly_mem_free(__sc);
}
