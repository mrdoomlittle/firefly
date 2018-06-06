# include "reservoir.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../ffly_def.h"
# include "../system/io.h"
# include "../system/sched.h"
# include "../clock.h"

# define is_flag(__flags, __flag) \
	((__flags&__flag)==__flag)
struct ffly_reservoir __ffly_reservoir__;
typedef struct region {
	ffly_slabp *slabs;
	ff_uint_t sc;
	struct region **bk, *next;
} *regionp;

ff_u32_t sched_id;

ff_i8_t static
update(void *__arg_p) {
	ffly_reservoirp r = (ffly_reservoirp)__arg_p;
	ffly_mutex_lock(&r->lock);
	ffly_slabp sb = r->open, *bk = &r->open;
	ffly_fprintf(ffly_log, "reservoir update.\n");
	while(sb != NULL) {
		ff_int_t tl = ((ff_int_t)(sb->creation+sb->death))-(ff_int_t)clock;

		ffly_fprintf(ffly_log, "slab : %u, creation: %u, death: %u, time left: %d\n",
			sb->off, sb->creation, sb->creation+sb->death, tl);
		if (clock >= sb->creation+sb->death) {
			if (!sb->p) {
				ffly_printf("error.\n");
			}
			*bk = sb->link;
			ffly_mutex_lock(&sb->lock);
			ffly_fprintf(ffly_log, "saving slab : %u\n", sb->off);
			pwrite(r->fd, sb->p, SLAB_SIZE, sb->off*SLAB_SIZE);
			__ffly_mem_free(sb->p);
			sb->p = NULL;
			sb->link = NULL;
			ffly_mutex_unlock(&sb->lock);
		}
	_sk:
		bk = &sb->link;
		sb = sb->link;
	}
	ffly_mutex_unlock(&r->lock);
	return -1;
}

# include "../corrode.h"
void static
corrode(void *__arg) {
	ffly_printf("reservoir-corrode.\n");
	ffly_reservoir_de_init((ffly_reservoirp)__arg);
}

ff_err_t ffly_reservoir_init(ffly_reservoirp __res, ff_u8_t __flags, char const *__file) {
	if ((__res->fd = open(__file, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1) {

	}
	
	__res->lock = FFLY_MUTEX_INIT;
	__res->open = NULL;
	__res->top = NULL;
	__res->bin = NULL;
	__res->off = 0;
	__res->reg = NULL;
	sched_id = ffly_schedule(update, __res, RESU_RATE);
	if (is_flag(__flags, RESV_CORRODE)) {
		ffly_corrode(corrode, __res);
	}
}

ff_err_t ffly_reservoir_de_init(ffly_reservoirp __res) {
	ffly_sched_rm(sched_id);
	regionp cur = (regionp)__res->reg, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		ffly_reservoir_free(__res, bk);
	}

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

	reg->bk = (regionp*)&__res->reg;
	if (__res->reg != NULL)
		((regionp)__res->reg)->bk = &reg->next;
	reg->next = (regionp)__res->reg;
	__res->reg = reg;
	return reg;
}

ff_err_t ffly_reservoir_free(ffly_reservoirp __res, void *__reg) {
	regionp reg = (regionp)__reg;
	ffly_slabp *p = reg->slabs;
	ffly_slabp *end = p+reg->sc;
	while(p != end)
		ffly_slab_free(__res, *(p++));

	*reg->bk = reg->next;
	if (reg->next != NULL)
		reg->next->bk = reg->bk;
	__ffly_mem_free(reg->slabs);
	__ffly_mem_free(__reg);
}

ff_err_t
ffly_reservoir_write(ffly_reservoirp __res, void *__reg,
	void *__p, ff_uint_t __offset, ff_uint_t __size)
{
	ffly_fprintf(ffly_log, "writing to region.\n");
	regionp reg = (regionp)__reg;
	ffly_slabp *sb = reg->slabs;
	ff_u8_t *p = (ff_u8_t*)__p;
	while(__size-(p-(ff_u8_t*)__p) >= SLAB_SIZE) {
		ffly_slab_write(__res, *(sb++), p, 0, SLAB_SIZE);
		p+=SLAB_SIZE;
	}

	ff_uint_t left = __size-(p-(ff_u8_t*)__p);
	if (left>0)
		ffly_slab_write(__res, *sb, p, 0, left);
}

ff_err_t
ffly_reservoir_read(ffly_reservoirp __res, void *__reg,
	void *__p, ff_uint_t __offset, ff_uint_t __size)
{
	ffly_fprintf(ffly_log, "reading from region.\n");
	regionp reg = (regionp)__reg;
	ffly_slabp *sb = reg->slabs;
	ff_u8_t *p = (ff_u8_t*)__p;
	while(__size-(p-(ff_u8_t*)__p) >= SLAB_SIZE) {
		ffly_slab_read(__res, *(sb++), p, 0, SLAB_SIZE);
		p+=SLAB_SIZE;
	}

	ff_uint_t left = __size-(p-(ff_u8_t*)__p);
	if (left>0)
		ffly_slab_read(__res, *sb, p, 0, left);
}
