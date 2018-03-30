# include "allocr.h"
# include "../linux/mman.h"
# include "../linux/unistd.h"
# include "../linux/signal.h"
# include "../system/util/checksum.h"
# include "../system/thread.h"
# include "../system/mutex.h"
# include "../mode.h"
ffly_err_t ffly_printf(char*, ...);
/*
	not using this until finished
	as it may hide issues.
*/
# define ALIGN 1
# define align_to(__no, __to)(((__no)+((__to)-1))&((~(__to))+1))
# define is_aligned_to(__no, __align)(((__no&__align) == __align)||!(__no&__align))
# define PAGE_SHIFT 8
# define PAGE_SIZE (1<<PAGE_SHIFT)
typedef mdl_u32_t ar_off_t;
typedef mdl_u32_t ar_size_t;
typedef mdl_u32_t ar_uint_t;
typedef mdl_s32_t ar_int_t;

# ifndef NULL
#	define NULL ((void*)0)
# endif

# define BLK_FREE 0x1
# define BLK_USED 0x2
# define USE_BRK 0x1
# define MMAPED 0x4

# define TRIM_MIN 0xf

# define is_flag(__flags, __flag) \
	(((__flags)&(__flag))==(__flag))

# define is_free(__blk) \
	is_flag((__blk)->flags, BLK_FREE)
# define is_used(__blk) \
	is_flag((__blk)->flags, BLK_USED)

# define get_blk(__pot, __off) \
	((blkdp)(((mdl_u8_t*)(__pot)->end)+(__off)))

# define next_blk(__pot, __blk) \
	get_blk(__pot, (__blk)->next)
# define prev_blk(__pot, __blk) \
	get_blk(__pot, (__blk)->prev)

# define AR_NULL ((ar_off_t)~0)
# define is_null(__p) ((__p)==AR_NULL) 
# define not_null(__p) ((__p)!=AR_NULL)

// aligned to page size
# define POT_SIZE 0xfff

# define lock_pot(__pot) \
	ffly_mutex_lock(&(__pot)->lock)
# define unlock_pot(__pot) \
	ffly_mutex_unlock(&(__pot)->lock)
# define lkpot(__pot) \
	ffly_mutex_lock(&(__pot)->lock)
# define ulpot(__pot) \
	ffly_mutex_unlock(&(__pot)->lock)

# define lkrod(__rod) \
	ffly_mutex_lock(&(__rod)->lock)
# define ulrod(__rod) \
	ffly_mutex_unlock(&(__rod)->lock)

# define no_bins 81
# define bin_no(__bc) \
	(((mdl_u32_t)(__bc))>>3 > 16?16+(((mdl_u32_t)(__bc))>>7 > 16?16+(((mdl_u32_t)(__bc))>>11 > 16?16+(((mdl_u32_t)(__bc))>>15 > 16?16+(((mdl_u32_t)(__bc))>>19 > 16?16: \
	(((mdl_u32_t)(__bc))>>19)):(((mdl_u32_t)(__bc))>>15)):(((mdl_u32_t)(__bc))>>11)):(((mdl_u32_t)(__bc))>>7)):(((mdl_u32_t)(__bc))>>3))

# define get_bin(__pot, __bc) \
	(*((__pot)->bins+bin_no(__bc)))
# define bin_at(__pot, __bc) \
	((__pot)->bins+bin_no(__bc))
# include "../system/err.h"
//# define DEBUG
void static
copy(void *__dst, void *__src, mdl_uint_t __bc) {
	mdl_u8_t *p = (mdl_u8_t*)__dst;
	mdl_u8_t *end = p+__bc;
	mdl_uint_t left;
	while(p != end) {
		left = __bc-(p-(mdl_u8_t*)__dst);
		if (left>>3 > 0) {
			*(mdl_u64_t*)p = *(mdl_u64_t*)((mdl_u8_t*)__src+(p-(mdl_u8_t*)__dst));
			p+=sizeof(mdl_u64_t);
		} else {
			*p = *((mdl_u8_t*)__src+(p-(mdl_u8_t*)__dst));
			p++;
		}
	}
}

typedef struct rod *rodp;
typedef struct pot {
	rodp r;
	ar_uint_t page_c, blk_c;

	ar_off_t off, top_blk, end_blk;
	void *top, *end;
	mdl_u8_t flags;
	ar_off_t bins[no_bins];
	struct pot *next, *fd, *bk;

	struct pot *previous;
	ar_uint_t buried, total;
	ffly_mutex_t lock;
} *potp;

typedef struct rod {
	struct rod *next;
	ffly_mutex_t lock;
    potp p;
	mdl_u8_t no;
} *rodp;

# define rodno(__p) \
	(((((mdl_u64_t)(__p))&0xff)^(((mdl_u64_t)(__p))>>8&0xff)^(((mdl_u64_t)(__p))>>16&0xff)^(((mdl_u64_t)(__p))>>24&0xff)^\
	(((mdl_u64_t)(__p))>>32&0xff)^(((mdl_u64_t)(__p))>>40&0xff)^(((mdl_u64_t)(__p))>>48&0xff)^(((mdl_u64_t)(__p))>>56&0xff))>>4&0x3f)
# define rod_at(__p) \
	(rods+rodno(__p))
rodp rods[64] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

static struct pot main_pot;

static __thread potp arena = NULL;
static __thread potp temp = NULL;
/*
	dont use pointers when we can use offsets to where the block is located 
	as it will take of less space in the header.
*/


# define PAD 8
// keep packed as we want the header to be as small as possible
typedef struct blkd {
	mdl_u8_t __pad0[PAD];
	ar_off_t prev, next, fd, bk;
	ar_size_t size; // total size, without blkd
	/*
		junk part of block that wont be used.
		but can be used.

		zeroed when freed.
	*/
	ar_uint_t pad;
	ar_off_t off, end;
	mdl_u8_t flags;
	mdl_u8_t __pad1[PAD];
} __attribute__((packed)) *blkdp;

# define pot_size sizeof(struct pot)
# define blkd_size sizeof(struct blkd)
void static* _ffly_alloc(potp, mdl_uint_t);
void static _ffly_free(potp, void*);

void static
abort() {
	ffly_printf("ar, abort.\n");
	exit(SIGKILL);
}

void ffly_arctl(mdl_u8_t __req, mdl_u64_t __val) {
	switch(__req) {
		case _ar_unset:
			arena = temp;
		break;
		case _ar_setpot:
			temp = arena;
			arena = (potp)__val;			
		break;
		case _ar_getpot:
			*(potp*)__val = arena;
		break;
	}
}

void static
atr(potp __pot, rodp __rod) {
	lkrod(__rod);
	__pot->r = __rod;
	__pot->next = __rod->p;
	if (__rod->p != NULL) {
		lkpot(__rod->p);
		__rod->p->previous = __pot;
		ulpot(__rod->p);
	}
	__rod->p = __pot;
	ulrod(__rod);
}

void static
rfr(potp __pot) {
	rodp r = __pot->r;

	potp ft = __pot->next;
	potp rr = __pot->previous;
	lkrod(r);
	if (__pot == r->p) {
		if ((r->p = __pot->next) != NULL) {
			lkpot(r->p);
			r->p->previous = NULL;
			ulpot(r->p);
		}
	} else {
		if (ft != NULL) {
			lkpot(ft);
			ft->previous = rr;
		}
		if (rr != NULL) {
			lkpot(rr);
			rr->next = ft;
		}

		if (ft != NULL)
			ulpot(ft);
		if (rr != NULL)
			ulpot(rr);
	}
	ulrod(r);
}

void static
detatch(potp __pot, blkdp __blk) {
	ar_off_t *bin = bin_at(__pot, __blk->size);
	ar_off_t fwd = __blk->fd;
	ar_off_t bck = __blk->bk;

	if (is_null(*bin))
		ffly_errmsg("bin at this location is dead.\n");

	if (__blk->off == *bin) {
		if (not_null(bck))
			ffly_errmsg("block seems to have a back link.\n");
		if (not_null(*bin = fwd))
			get_blk(__pot, fwd)->bk = AR_NULL; 
	} else {
		if (not_null(fwd))
			get_blk(__pot, fwd)->bk = bck;
		if (not_null(bck))
			get_blk(__pot, bck)->fd = fwd;
	}

	// clear
	__blk->fd = AR_NULL;
	__blk->bk = AR_NULL;
}

void static
recouple(potp __pot, blkdp __blk) {
	ar_off_t *top = &__pot->top_blk;
	ar_off_t *end = &__pot->end_blk;
	ar_off_t off = __blk->off;
	if (not_null(*top))
		if (off < get_blk(__pot, *top)->off)
			*top = off;
	else if (__pot->end == (void*)__blk)
		*top = off;

	if (not_null(*end))
		if (off > get_blk(__pot, *end)->off)
			*end = off;
	else if (__pot->off == __blk->end)
		*end = off;

	if (not_null(__blk->next))
		next_blk(__pot, __blk)->prev = off;
	if (not_null(__blk->prev))
		prev_blk(__pot, __blk)->next = off; 
}

void static
decouple(potp __pot, blkdp __blk) {
	ar_off_t *top = &__pot->top_blk;
	ar_off_t *end = &__pot->end_blk;
	ar_off_t off = __blk->off;

	ar_off_t rr = __blk->prev;
	ar_off_t ft = __blk->next;

	if (off == *top) {
		if (not_null(*top = ft))
			get_blk(__pot, *top)->prev = AR_NULL;
		else //make sure
			*end = AR_NULL;
		return;
	}

	if (off == *end) {
		if (not_null(*end = rr))
			get_blk(__pot, *end)->next = AR_NULL;
		return;
	}

	if (not_null(ft))
		next_blk(__pot, __blk)->prev = rr;
	if (not_null(rr))
		prev_blk(__pot, __blk)->next = ft;
}

/* dead memory */
/*
	memory that hasent been touched yet
*/
mdl_u64_t static
ffly_ardead(potp __pot) {
	return __pot->top-(__pot->end+__pot->off);
}

/* does not include block header */
/* used memory */
mdl_u64_t static
ffly_arused(potp __pot) {
	return __pot->off-((__pot->blk_c*blkd_size)+__pot->buried);
}

/* buried memory */
/*
	memory that is occupied but not being used at this moment
*/
mdl_u64_t static
ffly_arburied(potp __pot) {
	return __pot->buried;
}

# include "../system/nanosleep.h"
void ffly_arstat() {
	mdl_uint_t no = 0;
	rodp r = *rods;
	potp cur = &main_pot;
	_next:
	if (cur != NULL) {
		ffly_printf("potno: %u, rodno: %u - %s, off{%u}, no mans land{from: 0x%x, to: 0x%x}, pages{%u}, blocks{%u}, used{%u}, buried{%u}, dead{%u}\n",
			no++, !cur->r?0:cur->r->no, !cur->r?"bad":"ok", cur->off, cur->off, cur->top-cur->end, cur->page_c, cur->blk_c, ffly_arused(cur), ffly_arburied(cur), ffly_ardead(cur));

		if ((cur = cur->next) != NULL);	
			goto _next;
	}

	if (cur == &main_pot) {
		cur = r->p;
		goto _next;
	}

	if ((r = r->next) != *rods) {
		cur = r->p;
		goto _next;
	}
}

ffly_err_t init_pot(potp __pot) {
	__pot->top_blk = AR_NULL;
	__pot->end_blk = AR_NULL;
	__pot->off = 0;
	__pot->r = NULL;
	__pot->page_c = 0;
	__pot->blk_c = 0;
	__pot->flags = 0;
	__pot->next = NULL;
	__pot->previous = NULL;
	__pot->flags = 0x0;
	__pot->fd = NULL;
	__pot->bk = NULL;
	__pot->buried = 0;
	__pot->lock = FFLY_MUTEX_INIT;
	ar_off_t *bin = __pot->bins;
	while(bin != __pot->bins+no_bins)
		*(bin++) = AR_NULL;
}

ffly_err_t ffly_ar_init() {
	init_pot(&main_pot);
	main_pot.end = brk((void*)0);
	main_pot.top = main_pot.end;
	main_pot.flags |= USE_BRK;

	rodp *p = rods;
	while(p != rods+64) {
		*(*(p++) = (rodp)_ffly_alloc(&main_pot, sizeof(struct rod))) =
			(struct rod){.lock=FFLY_MUTEX_INIT,.p=NULL,.no=((p-1)==rods)?0:((*(p-1))->no+1)};
	}

	while(p != rods+1)
		(*--p)->next = *(p-1);
	(*rods)->next = *(rods+63);
}

ffly_err_t _ffly_ar_cleanup(potp __pot) {
	// need to track mmaps

	brk(__pot->end);
}

ffly_err_t ffly_ar_cleanup() {
	_ffly_ar_cleanup(&main_pot);
}

void pot_pr(potp __pot) {
	potp p = __pot;
	blkdp blk = get_blk(p, p->top_blk);
	mdl_uint_t static depth = 0;
	if (is_null(p->top_blk)) return;
	if (p->fd != NULL) {
		ffly_printf("\ndepth, %u\n", depth++);
		pot_pr(p->fd);
		ffly_printf("**end\n");
	}
	_next:
	ffly_printf("/-----------------------\\\n");
	ffly_printf("| size: %u, off: %x, pad: %u, inuse: %s\n", blk->size, blk->off, blk->pad, is_used(blk)?"yes":"no");
	ffly_printf("| prev: %x{%s}, next: %x{%s}, fd: %x{%s}, bk: %x{%s}\n", is_null(blk->prev)?0:blk->prev, is_null(blk->prev)?"dead":"alive", is_null(blk->next)?0:blk->next, is_null(blk->next)?"dead":"alive",
		is_null(blk->fd)?0:blk->fd, is_null(blk->fd)?"dead":"alive", is_null(blk->bk)?0:blk->bk, is_null(blk->bk)?"dead":"alive");
	ffly_printf("\\----------------------/\n");
	if (not_null(blk->next)) {
		blk = get_blk(p, blk->next);
		goto _next;		   
	}
}
# include "../maths/abs.h"
void pot_pf(potp __pot) {
	potp p = __pot;
	ar_off_t *bin = p->bins;
	blkdp blk, bk;
	mdl_uint_t static depth = 0;
	if (p->fd != NULL) {
		ffly_printf("\ndepth, %u, %u\n", depth++, p->fd->off);
		pot_pf(p->fd);
		ffly_printf("**end\n");
	}

	_next:
	bk = NULL;
	if (is_null(*bin)) goto _sk;
	if (*bin >= p->off) {
		ffly_errmsg("bin is messed up, at: %u, off: %u\n", bin-p->bins, *bin);
		goto _sk;
	}

	blk = get_blk(p, *bin);
	_fwd:
	if (bk != NULL) {
		ffly_printf("\\\n");
		ffly_printf(" > %u-bytes\n", ffly_abs((mdl_int_t)bk->off-(mdl_int_t)blk->off));
		ffly_printf("/\n");
	}
	ffly_printf("/-----------------------\\\n");
	ffly_printf("| size: %u\t\t| 0x%x, pad: %u\n", blk->size, blk->off, blk->pad);
	ffly_printf("\\----------------------/\n");
	if (not_null(blk->fd)) {
		bk = blk;
		blk = get_blk(p, blk->fd);
		goto _fwd;
	}

	_sk:
	if (bin != p->bins+(no_bins-1)) {
		bin++;
		goto _next;
	}
}

void ffly_arbl(void *__p) {
	blkdp blk = (blkdp)((mdl_u8_t*)__p-blkd_size);
	ffly_printf("block: off: %u, size: %u, pad: %u\n", blk->off, blk->size, blk->pad);
}

void pr() {
	ffly_printf("\n**** all ****\n");
	rodp r = *rods;
	potp p = &main_pot;
	mdl_uint_t no = 0;
	_next:
	if (p != NULL) {
		if (p == &main_pot)
			ffly_printf("~: main pot, no{%u}\n", no);
		ffly_printf("\npot, %u, used: %u, buried: %u, dead: %u, off: %u, pages: %u, blocks: %u\n", no++, ffly_arused(p), ffly_arburied(p), ffly_ardead(p), p->off, p->page_c, p->blk_c);
		pot_pr(p);
		if ((p = p->next) != NULL)
			goto _next;
	}

	if (p == &main_pot) {
		p = r->p;
		goto _next;
	}

	if ((r = r->next) != *rods) {
		p = r->p;
		goto _next;
	}
}

void pf() {
	ffly_printf("\n**** free ****\n");
	rodp r = *rods;
	potp p = &main_pot;
	mdl_uint_t no = 0;
	_next:
	if (p != NULL) {
		ffly_printf("\npot, %u, used: %u, buried: %u, dead: %u, off: %u\n", no++, ffly_arused(p), ffly_arburied(p), ffly_ardead(p), p->off);
		pot_pf(p);

		if ((p = p->next) != NULL)
			goto _next;
	}

	if (p == &main_pot) {
		p = r->p;		
		goto _next;
	}

	if ((r = r->next) != *rods) {
		p = r->p;
		goto _next;
	}
}


void free_pot(potp);
void ffly_araxe() {
	potp p;
	if (!(p = arena)) {
		ffly_printf("ar, pot has already been axed, or has been like this from the start.\n");
		return;
	}
	rfr(p);
	while(p != NULL) {
		potp bk = p;
		p = p->fd;
		rfr(bk);
		free_pot(bk);
	}
	arena = NULL;
}

/*
	has not been finished
	TODO:
*/

void static*
shrink_blk(potp __pot, blkdp __blk, ar_uint_t __size) {
	if (__blk->size <= __size || is_free(__blk)) {
		ffly_printf("forbidden alteration.\n");
		return NULL;
	}

	ar_uint_t size;
	void *p;
	if (!(p = ffly_alloc(size = (__blk->size-__blk->pad)))) {
		//error
	}

	lkpot(__pot);
	blkdp rr = prev_blk(__pot, __blk); //rear
	blkdp ft = next_blk(__pot, __blk); //front
//	blkdp fwd = get_blk(__pot, __blk->fd);
//	blkdp bck = get_blk(__pot, __blk->bk);
	struct blkd blk;

	void *ret = NULL;

	// how much to shrink
	ar_uint_t dif = (__blk->size-__blk->pad)-__size;
	ar_off_t *off = &__blk->off;
	mdl_u8_t freed, inuse;

	if (dif <= 0x4 && __blk->pad < 0x4) {
		__blk->pad+=dif;
		ret = (void*)((mdl_u8_t*)__blk+blkd_size);
		goto _r;
	}

	if (not_null(__blk->prev)) {
		if ((freed = is_free(rr)) || ((inuse = is_used(rr)) && rr->size <= 0xf)) {
			if (freed) {
				__pot->buried+=dif;
				detatch(__pot, rr);
			}

			copy(p, (mdl_u8_t*)__blk+blkd_size, size);
			*off+=dif;
			if (*off-dif == __pot->end_blk)
				__pot->end_blk = *off;
			rr->next = *off;
			if (not_null(__blk->next))
				ft->prev = *off;
			__blk->size-=dif;
			if (inuse)
				rr->pad+=dif;

			rr->size+=dif;
			rr->end+=dif;
			copy(&blk, __blk, blkd_size);
			__blk = (blkdp)((mdl_u8_t*)__blk+dif);
			copy(__blk, &blk, blkd_size);
			ret = (void*)((mdl_u8_t*)__blk+blkd_size);
			copy(ret, p, size-dif);
			if (freed) {
				ar_off_t *bin;
				if (not_null(rr->fd = *(bin = bin_at(__pot, rr->size)))) {
					get_blk(__pot, *bin)->bk = rr->off;		
				}

				*bin = rr->off;
			}
			goto _r;
		}
	}

	_r:
	ulpot(__pot);
	ffly_free(p);
	return ret;
}

void static*
grow_blk(potp __pot, blkdp __blk, ar_uint_t __size) {
	if (__blk->size-__blk->pad >= __size || is_free(__blk)) {
		ffly_printf("forbidden alteration.\n");
		return NULL;
	}

	ar_uint_t size;
	void *p;
	if (!(p = ffly_alloc(size = (__blk->size-__blk->pad)))) {
		//error
	}

	lkpot(__pot);
	blkdp rr = prev_blk(__pot, __blk); //rear
	blkdp ft = next_blk(__pot, __blk); //front
	struct blkd blk;

	void *ret = NULL;

	ar_uint_t dif = __size-(__blk->size-__blk->pad);
	ar_off_t *off = &__blk->off;

	if (__blk->pad >= dif) {
		__blk->pad-=dif;
		ret = (void*)((mdl_u8_t*)__blk+blkd_size);
		goto _r;
	}

	if (not_null(__blk->prev)) {
		if (is_free(rr) && rr->size > dif<<1) {
			__pot->buried-=dif;
			detatch(__pot, rr);
			copy(p, (mdl_u8_t*)__blk+blkd_size, size);
			*off-=dif;
			if (*off+dif == __pot->end_blk)
				__pot->end_blk = *off;
			rr->next = *off;
			if (not_null(__blk->next))
				ft->prev = *off;
			__blk->size+=dif;
			rr->size-=dif;
			rr->end-=dif;
			copy(&blk, __blk, blkd_size);
			__blk = (blkdp)((mdl_u8_t*)__blk-dif);
			copy(__blk, &blk, blkd_size);
			ret = (void*)((mdl_u8_t*)__blk+blkd_size);
			copy(ret, p, size);
			ar_off_t *bin;
			if (not_null(rr->fd = *(bin = bin_at(__pot, rr->size)))) {
				get_blk(__pot, *bin)->bk = rr->off;
			}

			*bin = rr->off;
			goto _r;
		}
	}

	_r:
	ulpot(__pot);
	ffly_free(p);
	return ret;
}


// user level
void*
ffly_arsh(void *__p, mdl_uint_t __to) {
	blkdp blk = (blkdp)((mdl_u8_t*)__p-blkd_size);
	potp p = arena, bk;
	lkpot(p);
	while(!(__p >= p->end && __p < p->top)) {
		bk = p;
		p = p->fd;
		ulpot(bk);
		if (!p) {
			ffly_errmsg("ar, address '%p' can't be found.\n", __p);
			return NULL;
		}
		lkpot(p);
	}
	ulpot(p);
	return shrink_blk(p, blk, __to);
}

void*
ffly_argr(void *__p, mdl_uint_t __to) {
	blkdp blk = (blkdp)((mdl_u8_t*)__p-blkd_size);
	potp p = arena, bk;
	lkpot(p);
	while(!(__p >= p->end && __p < p->top)) {
		bk = p;
		p = p->fd;
		ulpot(bk);	
		if (!p) {
			ffly_errmsg("ar, address '%p' can't be found.\n", __p);
			return NULL;
		}
		lkpot(p);
	}
	ulpot(p);
	return grow_blk(p, blk, __to);
}

potp alloc_pot(mdl_uint_t __size) {
	potp p;
	if (!(p = (potp)_ffly_alloc(&main_pot, pot_size))) {
		// err
	}

	init_pot(p);
	p->page_c = (__size>>PAGE_SHIFT)+((__size&((~(mdl_u64_t)0)>>(64-PAGE_SHIFT)))>0);

	mdl_uint_t size = p->page_c*PAGE_SIZE;
	p->end = _ffly_alloc(&main_pot, size);
	p->top = (void*)((mdl_u8_t*)p->end+size);
	p->total = p->top-p->end;
	return p;
}

void free_pot(potp __pot) {
	_ffly_free(&main_pot, __pot->end);
	_ffly_free(&main_pot, __pot);
}

void*
_ffly_alloc(potp __pot, mdl_uint_t __bc) {
	lock_pot(__pot);
	ar_off_t bin;
	if (not_null((bin = get_bin(__pot, __bc)))) {
		while(not_null(bin)) {
			blkdp blk;
			if ((blk = get_blk(__pot, bin))->size >= __bc) {
				detatch(__pot, blk);
				blk->pad = blk->size-__bc;
				blk->flags = (blk->flags&~BLK_FREE)|BLK_USED;
				__pot->buried-=blk->size;

				/*
				* if block exceeds size then trim it down and split the block into two parts.
				*/
				ar_uint_t junk;
				if ((junk = (blk->size-__bc)) > blkd_size+TRIM_MIN) {
					__pot->blk_c++;

					ar_off_t off = blk->off+blkd_size+__bc;
					blkdp p; 
					*(p = (blkdp)((mdl_u8_t*)__pot->end+off)) = (struct blkd){
						.prev = blk->off, .next = blk->next,
						.size = junk-blkd_size, .off = off, .end = blk->end,
						.fd = AR_NULL, .bk = AR_NULL,
						.flags = BLK_USED,
						.pad = 0
					};

					blk->pad = 0;
					blk->size = __bc;
					blk->next = p->off;
					blk->end = off;

					if (__pot->off == p->end)
						__pot->end_blk = p->off;
			  
					if (not_null(p->next))
						get_blk(__pot, p->next)->prev = p->off;
					unlock_pot(__pot);
					_ffly_free(__pot, (void*)((mdl_u8_t*)p+blkd_size));
					goto _sk;
				}
				unlock_pot(__pot);
				_sk:
				return (void*)((mdl_u8_t*)blk+blkd_size);
			}
			bin = blk->fd;
		}
	}

	ar_uint_t size = align_to(blkd_size+__bc, ALIGN);
	ar_off_t top = __pot->off+size;
	if (is_flag(__pot->flags, USE_BRK)) {
		ar_uint_t page_c;
		if ((page_c = ((top>>PAGE_SHIFT)+((top&((~(mdl_u64_t)0)>>(64-PAGE_SHIFT)))>0))) > __pot->page_c) {
		//if ((page_c = ((top>>PAGE_SHIFT)+((top-((top>>PAGE_SHIFT)*PAGE_SIZE))>0))) > __pot->page_c) {
			if (brk(__pot->top = (void*)((mdl_u8_t*)__pot->end+((__pot->page_c = page_c)*PAGE_SIZE))) == (void*)-1) {
				ffly_errmsg("error: brk.\n");
			}
		}
	} else {
		if (top >= __pot->top-__pot->end) {
			unlock_pot(__pot);
			return NULL;
		}
	}

	blkdp blk;
	*(blk = (blkdp)((mdl_u8_t*)__pot->end+__pot->off)) = (struct blkd) {
		.prev = __pot->end_blk, .next = AR_NULL,
		.size = size-blkd_size, .off = __pot->off, .end = top,
		.fd = AR_NULL, .bk = AR_NULL,
		.flags = BLK_USED
	};

	blk->pad = blk->size-__bc;
	if (is_null(__pot->top_blk))
		__pot->top_blk = __pot->off;
		
	if (not_null(__pot->end_blk))
		get_blk(__pot, __pot->end_blk)->next = __pot->off;
	__pot->end_blk = __pot->off;
	__pot->off = top;

	__pot->blk_c++;
	unlock_pot(__pot);
	return (void*)((mdl_u8_t*)blk+blkd_size);
}
# include "../data/bzero.h"
void*
ffly_alloc(mdl_uint_t __bc) {
	if (!__bc) return NULL;
	if (__bc+blkd_size >= POT_SIZE) {
		void *p;
		if ((p = mmap(NULL, blkd_size+__bc, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0)) == (void*)-1) {
			// failure
		}
		blkdp blk = (blkdp)p;
		blk->size = __bc;
		blk->flags = MMAPED;
		return (void*)((mdl_u8_t*)p+blkd_size);
	}

	if (!arena) {
		arena = alloc_pot(POT_SIZE);
		atr(arena, *rod_at(arena->end));
	}

	potp p = arena, t;
	void *ret;
	_again:
	if (!(ret = _ffly_alloc(p, __bc))) {
		lkpot(p);
		if (p->fd != NULL){
			t = p->fd;
			ulpot(p);
			p = t;
			goto _again;
		}
		ulpot(p);
	}

	if (!ret) {
		if (!(t = alloc_pot(POT_SIZE))) {
			// failure
		}

		atr(t, *rod_at(t->end));

		lkpot(p);
		p->fd = t;
		lkpot(p->fd);
		p->fd->bk = p;
		ulpot(p->fd);
		t = p->fd;
		ulpot(p);
		p = t;
		goto _again;  
	}	

	return ret;
}

void
_ffly_free(potp __pot, void *__p) {
	lock_pot(__pot);
	{
	blkdp blk = (blkdp)((mdl_u8_t*)__p-blkd_size);
	if (is_free(blk)) {
		ffly_errmsg("error: block has already been freed, at: %p\n", __p);
		goto _end;
	}

	decouple(__pot, blk);
	__ffmod_debug
		ffly_printf("to free: %u\n", blk->size);

	blkdp prev, next, top = NULL, end = NULL;
	if (not_null(blk->prev)) {
		prev = prev_blk(__pot, blk);
		while(is_free(prev)) {
			__pot->blk_c--;
			__pot->buried-=prev->size;
			__ffmod_debug
				ffly_printf("found free space above, %u\n", prev->size);

			detatch(__pot, prev);
			decouple(__pot, prev);

			__ffmod_debug
				ffly_printf("total freed: %u\n", prev->end-prev->off); 

			top = prev;
			if (is_null(prev->prev)) break;
			prev = prev_blk(__pot, prev);
		}
	}

	if (not_null(blk->next)) {
		next = next_blk(__pot, blk);
		while(is_free(next)) {
			__pot->blk_c--;
			__pot->buried-=next->size;
			__ffmod_debug
				ffly_printf("found free space below, %u\n", next->size);

			detatch(__pot, next);
			decouple(__pot, next);

			__ffmod_debug
				ffly_printf("total freed: %u\n", next->end-next->off);

			end = next;
			if (is_null(next->next)) break;
			next = next_blk(__pot, next);
		}
	}

	if (top != NULL) {
		top->size = ((top->end = blk->end)-top->off)-blkd_size;
		top->next = blk->next;
		blk = top;
	}

	if (end != NULL) {
		blk->size = ((blk->end = end->end)-blk->off)-blkd_size;
		blk->next = end->next;
	}

	// only used when used
	blk->pad = 0;

	__ffmod_debug
		ffly_printf("freed: %u, %u\n", blk->size, blk->off);

	if (blk->off>=__pot->off) {
		ffly_errmsg("somthing is wong.\n");
	}

	if (blk->end == __pot->off) {
		__pot->blk_c--;
		__pot->off = blk->off;
		if (is_flag(__pot->flags, USE_BRK)) { 
			mdl_uint_t page_c;
			if ((page_c = ((__pot->off>>PAGE_SHIFT)+((__pot->off&((~(mdl_u64_t)0)>>(64-PAGE_SHIFT)))>0))) < __pot->page_c) {
//			if ((page_c = ((__pot->off>>PAGE_SHIFT)+((__pot->off-((__pot->off>>PAGE_SHIFT)*PAGE_SIZE))>0))) < __pot->page_c) {
				if (brk(__pot->top = (void*)((mdl_u8_t*)__pot->end+((__pot->page_c = page_c)*PAGE_SIZE))) == (void*)-1) {
					ffly_errmsg("error: brk.\n");
				}
			}
		}
		goto _end;
	}

	__pot->buried+=blk->size;

	recouple(__pot, blk);
	blk->flags = (blk->flags&~BLK_USED)|BLK_FREE;
	ar_off_t bin;
	if (not_null(bin = get_bin(__pot, blk->size))) {
		get_blk(__pot, bin)->bk = blk->off;
		blk->fd = bin;
	}
	*(__pot->bins+bin_no(blk->size)) = blk->off;
	}
	_end:
	unlock_pot(__pot);
}

void
ffly_free(void *__p) {
	if (!__p) {
		ffly_errmsg("error: got null ptr.\n");
		return;
	}
	blkdp blk = (blkdp)((mdl_u8_t*)__p-blkd_size);
	if (is_flag(blk->flags, MMAPED)) {
		munmap((void*)blk, blkd_size+blk->size);
		return;
	}
	potp p = arena, bk;
	rodp r = NULL, beg;
	_bk:
	if (!p) {
		r = *rod_at(__p);
		beg = r;
		lkrod(r);
		rodp bk;
		while(!(bk = r)->p) {
			if ((r = r->next) == beg) {
				ffly_errmsg("error.\n");
				ulrod(bk);
				abort();
			}
			ulrod(bk);
			lkrod(r);
		}
	
		if (!(p = r->p)) {
			ffly_errmsg("error.\n");
			ulrod(r);
			abort();
		}

		ulrod(r);
	}

	// look for pot associated with pointer
	lkpot(p);
	while(!(__p >= p->end && __p < p->top)) {
		bk = p;
		p = !r?p->fd:p->next;
		ulpot(bk);
		if (!p) {
			if (r != NULL) {
				rodp bk;
				lkrod(r);
				while(!(p = (r = (bk = r)->next)->p) && r != beg) {
					ulrod(bk);
					lkrod(r);
				}
				ulrod(bk);

				if (r == beg) {
					ffly_errmsg("error: could not find pot associated with pointer.\n");
					abort();
				}
			}
			if (!p) {
				if (!r)
					goto _bk;
				else {
					ffly_errmsg("error.\n");
					return;
				}
			}
		}

		lkpot(p);
	}
	ulpot(p);

	_ffly_free(p, __p);
	// free pot as we don't need it
	lkpot(p);
	if (!p->off && p != arena && !r) {
		lkpot(p->bk);
		p->bk->fd = p->fd;
		ulpot(p->bk);
		if (p->fd != NULL) {
			lkpot(p->fd);
			p->fd->bk = p->bk;
			ulpot(p->fd);
		}
		ulpot(p);

		rfr(p);
		free_pot(p);
		return;
	}
	ulpot(p);
}

void*
ffly_realloc(void *__p, mdl_uint_t __bc) {
	if (!__p) {
		ffly_errmsg("error: got null ptr.\n");
		return NULL;
	}

	blkdp blk = (blkdp)((mdl_u8_t*)__p-blkd_size);
	ar_uint_t size = blk->size-blk->pad;
	ar_int_t dif = (ar_int_t)size-(ar_int_t)__bc;
	void *p;
	if (!is_flag(blk->flags, MMAPED)) {
		if (dif>0) {
			__ffmod_debug
				ffly_printf("shrink.\n");
//			if ((p = ffly_arsh(__p, __bc)) != NULL)
//				return p;
			__ffmod_debug
				ffly_printf("can't shrink block.\n");
		} else if (dif<0) {
			__ffmod_debug
				ffly_printf("grow.\n");
//			if ((p = ffly_argr(__p, __bc)) != NULL)
//				return p;
			__ffmod_debug
				ffly_printf("can't grow block.\n");
		}
	}

	if (!(p = ffly_alloc(__bc))) {
		// failure
	}
# ifdef DEBUG
	ffly_printf("prev size: %u, new size: %u, copysize: %u\n", size, __bc, size<__bc?size:__bc);
# endif
	copy(p, __p, size<__bc?size:__bc);
	ffly_free(__p);
	return p;
}

