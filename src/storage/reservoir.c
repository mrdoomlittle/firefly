# include "reservoir.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../ffly_def.h"
# include "../system/io.h"
typedef struct region {
	ffly_slabp *slabs;
	ff_uint_t sc;
} *regionp;


ff_err_t ffly_reservoir_init(ffly_reservoirp __res, char const *__file) {
	if ((__res->fd = open(__file, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1) {

	}

	__res->top = NULL;
	__res->bin = NULL;
	__res->off = 0;
}

ff_err_t ffly_reservoir_de_init(ffly_reservoirp __res) {
	ffly_slab_cleanup(__res);
	close(__res->fd);
}

void ffly_reservoir_info(ffly_reservoirp __res) {
	ffly_slabp cur = __res->top;
	while(cur != NULL) {
		ffly_printf("slab, off: %u\n", cur->off*SLAB_SIZE);
		cur = cur->next;
	}
}

void* ffly_reservoir_alloc(ffly_reservoirp __res, ff_uint_t __size) {
	regionp reg = (regionp)__ffly_mem_alloc(sizeof(struct region));
	ff_uint_t sc = (__size>>SLAB_SHIFT)+((__size&((~(ff_u64_t)0)>>(64-SLAB_SHIFT)))>0);
	ffly_slabp *p = (ffly_slabp*)__ffly_mem_alloc(sc*sizeof(ffly_slabp));
	reg->slabs = p;
	ffly_slabp *end = p+sc;
	while(p != end)
		*(p++) = ffly_slab_alloc(__res);
	reg->sc = sc;
	return reg;
}

ff_err_t ffly_reservoir_free(ffly_reservoirp __res, void *__reg) {
	regionp reg = (regionp)__reg;
	ffly_slabp *p = reg->slabs;
	ffly_slabp *end = p+reg->sc;
	while(p != end)
		ffly_slab_free(__res, *(p++));
	__ffly_mem_free(reg->slabs);
	__ffly_mem_free(__reg);
}

ff_err_t
ffly_reservoir_write(ffly_reservoirp __res, void *__reg,
	void *__p, ff_uint_t __offset, ff_uint_t __size)
{
	regionp reg = (regionp)__reg;
	ffly_slabp *sb = reg->slabs;
	ff_u8_t *p = (ff_u8_t*)__p;
	ff_u8_t *end = p+__size;
	while(p != end && p-(ff_u8_t*)__p >= SLAB_SIZE) {
		ffly_slab_write(__res, *(sb++), p, 0, SLAB_SIZE);
		p+=SLAB_SIZE;
	}

	ff_uint_t left = p-(ff_u8_t*)__p;
	if (left>0)
		ffly_slab_write(__res, *sb, p, 0, left);
}

ff_err_t
ffly_reservoir_read(ffly_reservoirp __res, void *__reg,
	void *__p, ff_uint_t __offset, ff_uint_t __size)
{
	regionp reg = (regionp)__reg;
	ffly_slabp *sb = reg->slabs;
	ff_u8_t *p = (ff_u8_t*)__p;
	ff_u8_t *end = p+__size;
	while(p != end && p-(ff_u8_t*)__p >= SLAB_SIZE) {
		ffly_slab_read(__res, *(sb++), p, 0, SLAB_SIZE);
		p+=SLAB_SIZE;
	}

	ff_uint_t left = p-(ff_u8_t*)__p;
	if (left>0)
		ffly_slab_read(__res, *sb, p, 0, left);
}
