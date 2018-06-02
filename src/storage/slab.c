# include "reservoir.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../linux/unistd.h"
# include "../ffly_def.h"
# include "../system/error.h"
ffly_slabp ffly_slab_alloc(ffly_reservoirp __res) {
	ffly_slabp sb;

	if (__res->bin != NULL) {
		sb = __res->bin;
		__res->bin = sb->fd;
	} else {
		sb = (ffly_slabp)__ffly_mem_alloc(sizeof(struct ffly_slab));
		sb->off = __res->off++;
	}
	sb->prev = NULL;
	sb->next = __res->top;
	if (__res->top != NULL)
		__res->top->prev = sb;
	__res->top = sb;
	sb->fd = NULL;
	sb->inuse = 0;
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
	deattach(__res, __sb);
	if (__res->off-1 == __sb->off) {
		ffly_slabp sb = __sb, bk;
		while(sb != NULL) {
			if (!sb->inuse) break;
			bk = sb;
			sb = sb->next;
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

ff_err_t ffly_slab_write(ffly_reservoirp __res, ffly_slabp __slab,
	void *__p, ff_uint_t __offset, ff_uint_t __size)
{
	pwrite(__res->fd, __p, __size, (__slab->off*SLAB_SIZE)+__offset);
	retok;
}

ff_err_t ffly_slab_read(ffly_reservoirp __res, ffly_slabp __slab,
	void *__p, ff_uint_t __offset, ff_uint_t __size)
{
	pread(__res->fd, __p, __size, (__slab->off*SLAB_SIZE)+__offset);
	retok;
}

void ffly_slab_cleanup(ffly_reservoirp __res) {
	ffly_slabp cur;
	if ((cur = __res->bin) != NULL) {
		ffly_slabp bk;
		while(cur != NULL) {
			bk = cur;
			cur = cur->next;
			__ffly_mem_free(bk);
		}
	}
}
