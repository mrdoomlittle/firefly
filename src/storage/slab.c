# include "reservoir.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../linux/unistd.h"
# include "../ffly_def.h"
# include "../system/error.h"
# include "../dep/mem_cpy.h"
# include "../clock.h"
# include "../system/io.h"
ffly_slabp ffly_slab_alloc(ffly_reservoirp __res) {
	ffly_slabp sb;
	if (__res->bin != NULL) {
		sb = __res->bin;
		__res->bin = sb->fd;
		sb->inuse = 0;
		return sb;
	} else {
		sb = (ffly_slabp)__ffly_mem_alloc(sizeof(struct ffly_slab));
		sb->off = __res->off++;
		ffly_fprintf(ffly_log, "new slab %u\n", sb->off);
	}
	sb->prev = NULL;
	sb->next = __res->top;
	if (__res->top != NULL)
		__res->top->prev = sb;
	__res->top = sb;
	sb->fd = NULL;
	sb->inuse = 0;
	sb->p = NULL;
	sb->link = NULL;
	sb->lock = FFLY_MUTEX_INIT;
	return sb;
}

void static
deattach(ffly_reservoirp __res, ffly_slabp __sb) {
	if (__sb == __res->top) {
		if ((__res->top = __sb->next) != NULL)
			__res->top->prev = NULL;
		return;
	}

	if (__sb->prev != NULL)
		__sb->prev->next = __sb->next;
	if (__sb->next != NULL)
		__sb->next->prev = __sb->prev;
}

ff_err_t ffly_slab_free(ffly_reservoirp __res, ffly_slabp __sb) {
	if (__res->off-1 == __sb->off) {
		ffly_slabp sb = __sb, bk;
		while(sb != NULL) {
			if (!sb->inuse) break;
			bk = sb;
			sb = sb->next;
			deattach(__res, __sb);
			__res->off--;
			__ffly_mem_free(bk);
		}
		reterr;
	}

	__sb->inuse = -1;
	__sb->fd = __res->bin;
	__res->bin = __sb;
	retok;
}

void static
load(ffly_reservoirp __res, ffly_slabp __slab) {
	ffly_fprintf(ffly_log, "loading slab : %u\n", __slab->off);
	__slab->p = __ffly_mem_alloc(SLAB_SIZE);
	pread(__res->fd, __slab->p, SLAB_SIZE, __slab->off*SLAB_SIZE);
	__slab->creation = clock;
	__slab->death = __slab->creation+SLAB_AGE;
	ffly_mutex_lock(&__res->lock);
	__slab->link = __res->open;
	__res->open = __slab;
	ffly_mutex_unlock(&__res->lock);
}

ff_err_t ffly_slab_write(ffly_reservoirp __res, ffly_slabp __slab,
	void *__p, ff_uint_t __offset, ff_uint_t __size)
{
	ffly_fprintf(ffly_log, "writing to slab %u\n", __slab->off);
	ffly_mutex_lock(&__slab->lock);
	if (__slab->p != NULL) {
		__slab->death+=SLAB_AGE;
	_bk:
		ffly_mem_cpy((ff_u8_t*)__slab->p+__offset, __p, __size);
	} else {
		load(__res, __slab);
		goto _bk;
	}
	ffly_mutex_unlock(&__slab->lock);
	//	pwrite(__res->fd, __p, __size, (__slab->off*SLAB_SIZE)+__offset);
	retok;
}

ff_err_t ffly_slab_read(ffly_reservoirp __res, ffly_slabp __slab,
	void *__p, ff_uint_t __offset, ff_uint_t __size)
{
	ffly_fprintf(ffly_log, "reading from slab %u\n", __slab->off);
	ffly_mutex_lock(&__slab->lock);
	if (__slab->p != NULL) {
		__slab->death+=SLAB_AGE;
	_bk:
		ffly_mem_cpy(__p, (ff_u8_t*)__slab->p+__offset, __size);
	} else {
		load(__res, __slab);
		goto _bk;
	}
	ffly_mutex_unlock(&__slab->lock);
	//	pread(__res->fd, __p, __size, (__slab->off*SLAB_SIZE)+__offset);
	retok;
}

void ffly_slab_cleanup(ffly_reservoirp __res) {
	ffly_slabp cur;
	if ((cur = __res->top) != NULL) {
		ffly_slabp bk;
		while(cur != NULL) {
			bk = cur;
			cur = cur->next;
			if (bk->p != NULL)
				__ffly_mem_free(bk->p);
			__ffly_mem_free(bk);
		}
	}
}
