# include "m.h"
# include "../../memory/mem_alloc.h"
# include "../../memory/mem_free.h"
# include "../../memory/mem_realloc.h"
# include "../../system/io.h"

#define PAGE_SHIFT 4
#define PAGE_SIZE (1<<PAGE_SHIFT)

#define attachslab(__sb)\
	__sb->prev = NULL;\
	__sb->next = mfs->top;\
	if (mfs->top != NULL)\
		mfs->top->prev = __sb;\
	mfs->top = __sb;
#define linkslab(__sb)\
	__sb->fd = mfs->bin;\
	__sb->bk = &mfs->bin;\
	if (mfs->bin != NULL)\
		mfs->bin->bk = &__sb->fd;\
	mfs->bin = __sb;

struct mfs_slab **slabs = NULL;
static ff_u32_t page_c = 0;
static ff_u64_t off = 0;
#define DEBUG

struct mfs_slab static* slab_alloc(void);
struct mfs_slab* slabat(ff_u32_t);

#define LKNULL 0xffffffff

ff_u64_t mfs_slabc(void) {
	return off;
}

struct mfs_slab static* __slab(ff_u32_t __s) {
	ff_u32_t pg, pg_off, pg_c;
	struct mfs_slab **pp;

	pg_c = ((__s+1)+(PAGE_SIZE-1))>>PAGE_SHIFT;
	if (!slabs) {
		slabs = (struct mfs_slab**)__ffly_mem_alloc(pg_c*sizeof(struct mfs_slab*));
	} else {
		if (pg_c>page_c) {
			slabs = (struct mfs_slab**)__ffly_mem_realloc(slabs, pg_c*sizeof(struct mfs_slab*));
		} else
			goto _sk;
	}

	pg = page_c;
	page_c = pg_c;
	while(pg != pg_c)
		*(slabs+(pg++)) = NULL;

_sk:
	pg = __s>>PAGE_SHIFT;
	pg_off = __s-(pg*PAGE_SIZE);
	pp = slabs+pg;
	if (!*pp)
		*pp = (struct mfs_slab*)__ffly_mem_alloc(PAGE_SIZE*sizeof(struct mfs_slab));

	return (*pp)+pg_off;
}

void mfs_slabs_save(ff_uint_t *__n, ff_u32_t __to) {
	struct mfs_slab_struc *slabs, *ss;
	struct mfs_slab *s;

	slabs = (struct mfs_slab_struc*)__ffly_mem_alloc(off*sizeof(struct mfs_slab_struc));
	ff_uint_t i;

	ffly_printf("slab save\n");
	i = 0;
	for(;i != off;i++) {
		ss = slabs+i;
		s = slabat(i);
		ss->pv = !s->prev?LKNULL:s->prev->in;
		ss->nx = !s->next?LKNULL:s->next->in;
		ss->in = s->in;
		ss->off = s->off;
		ss->flags = s->flags;
		ffly_printf("slab save: off: %u, in: %u, flags: %u, {%u, %u}, used: %s\n", ss->off, ss->in, ss->flags, ss->pv, ss->nx, !(ss->flags&MFS_SLAB_INUSE)?"no":"yes");
	}

	mfs->write(slabs, off*sizeof(struct mfs_slab_struc), __to);
	__ffly_mem_free(slabs);
	*__n = off;
}

void mfs_slabs_load(ff_uint_t __n, ff_u32_t __from) {
	ff_uint_t i;

	struct mfs_slab_struc *slabs, *ss;
	struct mfs_slab *s;
	slabs = (struct mfs_slab_struc*)__ffly_mem_alloc(__n*sizeof(struct mfs_slab_struc));
	mfs->read(slabs, __n*sizeof(struct mfs_slab_struc), __from);

	ffly_printf("slab load.\n");
	off = __n;

	i = 0;
	while(i != __n) {
		ss = slabs+i;
		s = __slab(ss->in);
		s->in = ss->in;
		s->off = ss->off;
		s->flags = ss->flags;
		if (!(ss->flags&MFS_SLAB_INUSE)) {
			linkslab(s);
		} else {
			s->fd = NULL;
			s->bk = NULL;
		}
		ffly_printf("slab load: off: %u, in: %u, flags: %u, {%u, %u}, used: %s\n", ss->off, ss->in, ss->flags, ss->pv, ss->nx, !(ss->flags&MFS_SLAB_INUSE)?"no":"yes");
		i++;
	}

	i = 0;
	while(i != __n) {
		ss = slabs+i;
		s = slabat(ss->in);
		s->prev = ss->pv == LKNULL?NULL:slabat(ss->pv);
		s->next = ss->nx == LKNULL?NULL:slabat(ss->nx);
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

void mfs_vfree(ff_u32_t __p, ff_uint_t __n) {
	struct mfs_slab *sb;
	ff_u32_t ss, se;
	ss = __p>>MFS_SLAB_SHIFT;
	se = ss+(__n>>MFS_SLAB_SHIFT);
	ff_u32_t i;
	i = ss;
	for(;i != se;i++) {
		sb = __slab(i);
	}
}



ff_u32_t mfs_valloc(ff_uint_t __n) {
	ff_u32_t r;
	r = mfs->off;
	mfs->off+=(__n+(MFS_SLAB_SIZE-1))>>MFS_SLAB_SHIFT;
	return r;
}

ff_u32_t mfs_balloc(void) {
	return mfs_valloc(sizeof(struct mfs_bale));
}

struct mfs_slab* slab_alloc(void) {
	struct mfs_slab *sb;
	if (mfs->bin != NULL) {
		sb = mfs->bin; 
		if ((mfs->bin = sb->fd) != NULL)
			mfs->bin->bk = sb->bk;
		goto _done;
	}

	sb = __slab(off);
#ifdef DEBUG
	if (off>=(page_c*PAGE_SIZE)) {
		ffly_printf("mfs: slab_alloc, what the fuck have you done.\n-- slab~n is above page~n slabs, noff: %u, page_noff: %u.\n", off, page_c*PAGE_SIZE);
		return NULL;
	}
#endif
	sb->in = off++;

	sb->off = mfs->off++;
 
 	attachslab(sb)
	sb->fd = NULL;
	sb->bk = NULL;
	sb->flags = 0x00;
_done:
	sb->flags |= MFS_SLAB_INUSE;
	return sb;
}

void static slab_free(struct mfs_slab *__sb) {
	linkslab(__sb);
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
