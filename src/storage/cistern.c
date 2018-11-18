# include "cistern.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../system/io.h"
# include "../system/errno.h"
# define SLAB_SHIFT 6
# define SLAB_SIZE (1<<SLAB_SHIFT)
typedef struct region {
	ffly_cis_slabp *slabs;
	ff_uint_t sc;
} *regionp;

void static
deattach(ffly_cisternp __cis, ffly_cis_slabp __sb) {
	if (__sb == __cis->top) {
		if ((__cis->top = __sb->next) != NULL)
			__cis->top->prev = NULL;
		return;
	}

	if (__sb->prev != NULL)
		__sb->prev->next = __sb->next;
	if (__sb->next != NULL)
		__sb->next->prev = __sb->prev;
}

void static
delink(ffly_cisternp __cis, ffly_cis_slabp __sb) {
	*__sb->bk = __sb->fd;
	if (__sb->fd != NULL)
		__sb->fd->bk = __sb->bk;
}

ffly_cis_slabp static
slab_alloc(ffly_cisternp __cis) {
	ffly_cis_slabp sb;

	if (__cis->bin != NULL) {
		sb = __cis->bin;
		if ((__cis->bin = sb->fd) != NULL)
			__cis->bin->bk = sb->bk;
	} else {
		sb = (ffly_cis_slabp)__ffly_mem_alloc(sizeof(struct ffly_cis_slab));
		sb->off = __cis->off++;
	}

	sb->prev = NULL;
	sb->next = __cis->top;
	if (__cis->top != NULL)
		__cis->top->prev = sb;
	__cis->top = sb;
	sb->fd = NULL;
	sb->bk = NULL;
	sb->inuse = 0;
	return sb;
}

void static
slab_free(ffly_cisternp __cis, ffly_cis_slabp __sb) {
	if (__sb->off == __cis->off-1) {
		ffly_cis_slabp cur, sb;
		__cis->off--;
		cur = __sb->next;
		deattach(__cis, __sb);
		__ffly_mem_free(__sb);

		while(cur != NULL) {
			sb = cur;
			cur = cur->next;
			if (!sb->inuse) break;
			delink(__cis, sb);
			deattach(__cis, sb);
			__cis->off--;
			__ffly_mem_free(sb);
		}
		return;
	}

	__sb->inuse = -1;
	__sb->fd = __cis->bin;
	__sb->bk = &__cis->bin;
	if (__cis->bin != NULL)
		__cis->bin->bk = &__sb->fd;
	__cis->bin = __sb;	
}

void static
slab_cleanup(ffly_cisternp __cis) {
	ffly_cis_slabp cur, bk;
	cur = __cis->top;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		__ffly_mem_free(bk);
	}
}

void ffly_cistern_init(ffly_cisternp __cis, char const *__file) {
	if ((__cis->fd = open(__file, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1) {

	}
	__cis->off = 0;
	__cis->top = NULL;
	__cis->bin = NULL;
}

void ffly_cistern_de_init(ffly_cisternp __cis) {
	slab_cleanup(__cis);
	close(__cis->fd);
}

void* ffly_cistern_alloc(ffly_cisternp __cis, ff_uint_t __size) {
	regionp r;
	ff_uint_t sc;
	r = (regionp)__ffly_mem_alloc(sizeof(struct region));
	sc = (__size+(0xffffffffffffffff>>(64-SLAB_SHIFT)))>>SLAB_SHIFT;
	r->sc = sc;
	ffly_cis_slabp *s, *end;
	s = (ffly_cis_slabp*)__ffly_mem_alloc(sc*sizeof(struct ffly_cis_slab));
	end = s+sc;
	r->slabs = s;
	while(s != end) 
		*(s++) = slab_alloc(__cis);
	ffly_printf("slabs: %u\n", sc);
	return r;
}
 
void ffly_cistern_free(ffly_cisternp __cis, void *__r) {
	regionp r;

	r = (regionp)__r;
	ffly_cis_slabp *s, *end;
	s = r->slabs;
	end = s+r->sc;
	while(s != end)
		slab_free(__cis, *(s++));
	__ffly_mem_free(r->slabs);
	__ffly_mem_free(__r);
}

void ffly_cistern_write(ffly_cisternp __cis, void *__r, void *__p,
	ff_uint_t __offset, ff_uint_t __size)
{
	regionp r;

	r = (regionp)__r;

	ff_uint_t ss, offset;
	ss = (__offset>>SLAB_SHIFT);
	ffly_cis_slabp *s, sb;
	s = r->slabs+ss;
	offset = __offset-(ss*SLAB_SIZE);

	ffly_cis_slabp *end;
	end = s+(__size>>SLAB_SHIFT);
	ff_u8_t *p;

	p = (ff_u8_t*)__p;
	if (offset>0 && __size>=SLAB_SIZE) {
		ff_uint_t sz;
		pwrite(__cis->fd, p, sz = (SLAB_SIZE-offset), ((*s)->off*SLAB_SIZE)+offset);
		s++;
		p+=sz;
		offset = 0;
	}

	if (offset+__size>SLAB_SIZE && __size<SLAB_SIZE) {
		ff_uint_t a0, a1;
		a0 = SLAB_SIZE-offset;
		a1 = __size-a0;
		pwrite(__cis->fd, p, a0, ((*s)->off*SLAB_SIZE)+offset);
		pwrite(__cis->fd, p+a0, a1, (*(s+1))->off*SLAB_SIZE);
		return;
	}

	while(s<end) {
		sb = *(s++);
		pwrite(__cis->fd, p, SLAB_SIZE, sb->off*SLAB_SIZE);
		p+=SLAB_SIZE;
	}

	ff_uint_t left;
	left = __size-(p-(ff_u8_t*)__p);
	if (left>0) {
		ffly_printf("write, left: %u, %u, file: %u, slab: %u : %p\n", left, offset, __cis->fd, (*s)->off, *s);
		if (pwrite(__cis->fd, p, left, ((*s)->off*SLAB_SIZE)+offset) == -1) {
			ffly_printf("write failure, %s\n", strerror(errno));
		}
	}
}

void ffly_cistern_read(ffly_cisternp __cis, void *__r, void *__p,
	ff_uint_t __offset, ff_uint_t __size)
{
	regionp r;

	r = (regionp)__r;

	ff_uint_t ss, offset;
	ss = (__offset>>SLAB_SHIFT);
	ffly_cis_slabp *s, sb;
	s = r->slabs+ss;
	offset = __offset-(ss*SLAB_SIZE);

	ffly_cis_slabp *end;
	end = s+(__size>>SLAB_SHIFT);
	ff_u8_t *p;

	p = (ff_u8_t*)__p;
	if (offset>0 && __size>=SLAB_SIZE) {
		ff_uint_t sz;
		pread(__cis->fd, p, sz = (SLAB_SIZE-offset), ((*s)->off*SLAB_SIZE)+offset);
		s++;
		p+=sz;
		offset = 0;
	}

	if (offset+__size>SLAB_SIZE && __size<SLAB_SIZE) {
		ff_uint_t a0, a1;
		a0 = SLAB_SIZE-offset;
		a1 = __size-a0;
		pread(__cis->fd, p, a0, ((*s)->off*SLAB_SIZE)+offset);
		pread(__cis->fd, p+a0, a1, (*(s+1))->off*SLAB_SIZE);
		return;
	}

	while(s<end) {
		sb = *(s++);
		pread(__cis->fd, p, SLAB_SIZE, sb->off*SLAB_SIZE);
		p+=SLAB_SIZE;
	}

	ff_uint_t left;
	left = __size-(p-(ff_u8_t*)__p);
	if (left>0) {
		ffly_printf("read, left: %u, %u, file: %u, slab: %u : %p\n", left, offset, __cis->fd, (*s)->off, *s);
		if (pread(__cis->fd, p, left, ((*s)->off*SLAB_SIZE)+offset) == -1) {
			ffly_printf("read failure, %s\n", strerror(errno));
		}
	}
}
