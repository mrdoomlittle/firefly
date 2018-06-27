# include "allocr.h"
# include "../linux/mman.h"
# include "../linux/unistd.h"
# include "../linux/signal.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../system/util/checksum.h"
# include "../system/thread.h"
# include "../system/mutex.h"
# include "../mode.h"
# include "../rat.h"
# include "../system/tls.h"
# include "../system/string.h"
# include "../dep/str_len.h"
# include "../dep/str_cpy.h"
# include "../dep/mem_set.h"
# include "../system/io.h"
//	might want to keep track of mmaps

/*
	not using this until finished
	as it may hide issues.
*/
# define ALIGN 1
# define align_to(__no, __to)(((__no)+((__to)-1))&((~(__to))+1))
# define is_aligned_to(__no, __align)(((__no&__align) == __align)||!(__no&__align))
# define PAGE_SHIFT 8
# define PAGE_SIZE (1<<PAGE_SHIFT)
typedef ff_u32_t ar_off_t;
typedef ff_u32_t ar_size_t;
typedef ff_u32_t ar_uint_t;
typedef ff_s32_t ar_int_t;

/* recycle old spots
*/
# ifndef NULL
#	define NULL ((void*)0)
# endif

// block flags
# define BLK_FREE 0x1
# define BLK_USED 0x2
# define MMAPED 0x4
# define BLK_STATIC 0x8
# define BLK_LINKED 0x10
// pot flag
# define USE_BRK 0x1

/*
	min about that can be trimed from oversized block
*/
# define TRIM_MIN 0x13

// MAX SHRINK
# define MAX_SH 0xf

// MAX GROW
# define MAX_GR 0xf

# define get_pot(__b) \
	*(potp*)(((ff_u8_t*)(__b)-(__b)->off)-sizeof(potp*))
# define is_flag(__flags, __flag) \
	(((__flags)&(__flag))==(__flag))

# define is_free(__b) \
	is_flag((__b)->flags, BLK_FREE)
# define is_used(__b) \
	is_flag((__b)->flags, BLK_USED)
# define is_static(__b) \
	is_flag((__b)->flags, BLK_STATIC)

# define get_block(__pot, __off) \
	((blockp)(((ff_u8_t*)(__pot)->end)+(__off)))

# define block_next(__pot, __b) \
	get_block(__pot, (__b)->next)
# define block_prev(__pot, __b) \
	get_block(__pot, (__b)->prev)

# define AR_NULL ((ar_off_t)~0)
# define is_null(__p) ((__p)==AR_NULL) 
# define not_null(__p) ((__p)!=AR_NULL)

# define POT_SIZE 0x986f
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
	(((ff_u32_t)(__bc))>>3 > 16?16+(((ff_u32_t)(__bc))>>7 > 16?16+(((ff_u32_t)(__bc))>>11 > 16?16+(((ff_u32_t)(__bc))>>15 > 16?16+(((ff_u32_t)(__bc))>>19 > 16?16: \
	(((ff_u32_t)(__bc))>>19)):(((ff_u32_t)(__bc))>>15)):(((ff_u32_t)(__bc))>>11)):(((ff_u32_t)(__bc))>>7)):(((ff_u32_t)(__bc))>>3))

# define get_bin(__pot, __bc) \
	(*((__pot)->bins+bin_no(__bc)))
# define bin_at(__pot, __bc) \
	((__pot)->bins+bin_no(__bc))

/*
	erase block when freed
*/
# define ERASE

// erase block contents
void(*ar_erase)(void*, ff_uint_t) = NULL;

# include "../system/err.h"
# include <stdarg.h>
//# define DEBUG
/* not the best but will do */
void static
copy(void *__dst, void *__src, ff_uint_t __bc) {
	ff_u8_t *p = (ff_u8_t*)__dst;
	ff_u8_t *end = p+__bc;
	ff_uint_t left;
	while(p != end) {
		left = __bc-(p-(ff_u8_t*)__dst);
		if (left>>3 > 0) {
			*(ff_u64_t*)p = *(ff_u64_t*)((ff_u8_t*)__src+(p-(ff_u8_t*)__dst));
			p+=sizeof(ff_u64_t);
		} else {
			*p = *((ff_u8_t*)__src+(p-(ff_u8_t*)__dst));
			p++;
		}
	}
}

/*
	using real one will only take more time and cause more obstacles in the future
*/
int static arout;
void static
out(char const *__format, va_list __args) {
	char buf[2048];
	char *bufp = buf;
	char const *p = __format;
	char *end = p+ffly_str_len(__format);
	while(p != end) {
		if (*(p++) == '%') {
			if (*p == 'u') {
				ff_u32_t v = va_arg(__args, ff_u32_t);
				bufp+=ffly_nots(v, bufp);
			} else if (*p == 'x') {
				ff_u32_t v = va_arg(__args, ff_u32_t);
				bufp+=ffly_noths((ff_u64_t)v, bufp);
			} else if (*p == 'p') { 
				void *v = va_arg(__args, void*);
				*(bufp++) = '0';
				*(bufp++) = 'x';
				bufp+=ffly_noths((ff_u64_t)v, bufp);
			} else if (*p == 's') {
				char *s = va_arg(__args, char*);
				bufp+=ffly_str_cpy(bufp, s);
			}
			p++;
		} else
			*(bufp++) = *(p-1);
	}
	*bufp = '\0';
	write(arout, buf, bufp-buf);
}

void static
printf(char const *__format, ...) {
	va_list args;
	va_start(args, __format);
	out(__format, args);
	va_end(args);
}

typedef struct rod *rodp;
typedef struct pot {
	rodp r;
	ar_uint_t page_c, blk_c;

	ar_off_t off, top_blk, end_blk;
	void *top, *end;
	ff_u8_t flags;
	ar_off_t bins[no_bins];
	struct pot *next, *fd, *bk;

	struct pot *previous;
	ar_uint_t buried, total;
	ff_mlock_t lock;
} *potp;

typedef struct rod {
	struct rod *next;
	ff_mlock_t lock;
    potp p;
	ff_u8_t no;
} *rodp;

# define rodno(__p) \
	(((((ff_u64_t)(__p))&0xff)^(((ff_u64_t)(__p))>>8&0xff)^(((ff_u64_t)(__p))>>16&0xff)^(((ff_u64_t)(__p))>>24&0xff)^\
	(((ff_u64_t)(__p))>>32&0xff)^(((ff_u64_t)(__p))>>40&0xff)^(((ff_u64_t)(__p))>>48&0xff)^(((ff_u64_t)(__p))>>56&0xff))>>2)
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

// rename
static void *spot[30];

static ff_uint_t arena_tls;
static ff_uint_t temp_tls;
/*
	dont use pointers when we can use offsets to where the block is located 
	as it will take of less space in the header.
*/

// keep packed as we want the header to be as small as possible
typedef struct block {
	ar_off_t prev, next, fd, bk;
	ar_size_t size; // total size, without block
	/*
		junk part of block that wont be used.
		but can be used.

		zeroed when freed.
	*/
	ar_uint_t pad;
	ar_off_t off, end;
	ff_u8_t flags;
} __attribute__((packed)) *blockp;

# define pot_size sizeof(struct pot)
# define block_size sizeof(struct block)
void static* _ffly_balloc(potp, ff_uint_t);
void static _ffly_bfree(potp, void*);

# define cur_arena \
	((potp)*(spot+ffly_tls_get(arena_tls)))
void static
abort(void) {
	printf("ar, abort.\n");
	exit(SIGKILL);
}

ff_uint_t static spot_id = 0;

ff_mlock_t static plock = FFLY_MUTEX_INIT;
void ffly_process_prep(void) {
	ffly_mutex_lock(&plock);
	ffly_tls_set(spot_id++, arena_tls);
	ffly_tls_set(spot_id++, temp_tls);
	*(spot+(spot_id-1)) = NULL;
	*(spot+(spot_id-2)) = NULL;
	ffly_mutex_unlock(&plock);
}

void ffly_arctl(ff_u8_t __req, ff_u64_t __val) {
	void **arena_spot = spot+ffly_tls_get(arena_tls);
	void **temp_spot = spot+ffly_tls_get(temp_tls);
	potp arena = (potp)*arena_spot;
	potp temp = (potp)*temp_spot;
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

	*arena_spot = arena;
	*temp_spot = temp;
}

void arinfo(struct arinfo *__p) {
	__p->block_c = main_pot.blk_c;
	__p->used = (ff_u8_t*)main_pot.top-(ff_u8_t*)main_pot.end;
}

// add to rod
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

// remove from rod
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
detatch(potp __pot, blockp __b) {
	ar_off_t *bin = bin_at(__pot, __b->size);
	ar_off_t fwd = __b->fd;
	ar_off_t bck = __b->bk;
# ifdef __ffly_debug
	if (is_null(*bin))
		ff_rat(_ff_rat_0, out, "bin at this location is dead.\n");
# endif
	if (__b->off == *bin) {
# ifdef __ffly_debug
		if (not_null(bck))
			printf("block seems to have a back link.\n");
# endif
		if (not_null(*bin = fwd))
			get_block(__pot, fwd)->bk = AR_NULL; 
	} else {
		if (not_null(fwd))
			get_block(__pot, fwd)->bk = bck;
		if (not_null(bck))
			get_block(__pot, bck)->fd = fwd;
	}

	// clear
	__b->fd = AR_NULL;
	__b->bk = AR_NULL;
}

void static
recouple(potp __pot, blockp __b) {
	ar_off_t *top = &__pot->top_blk;
	ar_off_t *end = &__pot->end_blk;
	ar_off_t off = __b->off;
	if (not_null(*top)) {
		if (off < get_block(__pot, *top)->off)
			*top = off;
	} else if (__pot->end == (void*)__b)
		*top = off;

	if (not_null(*end)) {
		if (off > get_block(__pot, *end)->off)
			*end = off;
	} else if (__pot->off == __b->end)
		*end = off;

	if (not_null(__b->next))
		block_next(__pot, __b)->prev = off;
	if (not_null(__b->prev))
		block_prev(__pot, __b)->next = off; 
}

void static
decouple(potp __pot, blockp __b) {
	ar_off_t *top = &__pot->top_blk;
	ar_off_t *end = &__pot->end_blk;
	ar_off_t off = __b->off;

	ar_off_t rr = __b->prev;
	ar_off_t ft = __b->next;

	if (off == *top) {
		if (not_null(*top = ft))
			get_block(__pot, *top)->prev = AR_NULL;
		else //make sure
			*end = AR_NULL;
		return;
	}

	if (off == *end) {
		if (not_null(*end = rr))
			get_block(__pot, *end)->next = AR_NULL;
		else
			*top = AR_NULL;
		return;
	}

	if (not_null(ft))
		block_next(__pot, __b)->prev = rr;
	if (not_null(rr))
		block_prev(__pot, __b)->next = ft;
}

/* dead memory */
/*
	memory that hasent been touched yet
*/
ff_u64_t static
ffly_ardead(potp __pot) {
	return __pot->top-(__pot->end+__pot->off);
}

/* used memory */
ff_u64_t static
ffly_arused(potp __pot) {
	return __pot->off-((__pot->blk_c*block_size)+__pot->buried);
}

/* buried memory */
/*
	memory that is occupied but not being used at this moment
*/
ff_u64_t static
ffly_arburied(potp __pot) {
	return __pot->buried;
}

# include "../system/nanosleep.h"
void ffly_arstat(void) {
	printf("potsize: %u, blocksize: %u\n", pot_size, block_size);
	potp arena = cur_arena;
	ff_uint_t no = 0;
	rodp r = *rods;
	potp cur = &main_pot;
_next:
	if (cur != NULL) {
		printf("potno: %u, rodno: %u - %s, off{%u}, no mans land{from: 0x%x, to: 0x%x}, pages{%u}, blocks{%u}, used{%u}, buried{%u}, dead{%u}\n",
			no++, !cur->r?0:cur->r->no, !cur->r?"bad":"ok", cur->off, cur->off, cur->top-cur->end, cur->page_c, cur->blk_c, ffly_arused(cur), ffly_arburied(cur), ffly_ardead(cur));

		if ((cur = cur->next) != NULL)
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

ff_err_t init_pot(potp __pot) {
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

void static tls_init(void) {
	ffly_tls_set(0, arena_tls);
	ffly_tls_set(0, temp_tls);
	printf("tls init.\n");
}

void static
_erase(void *__p, ff_uint_t __size) {
	ffly_mem_set(__p, 0xff, __size);
}

ff_err_t
ffly_ar_init(void) {
	ar_erase = _erase;
# ifndef __tty
	arout = open("arout", O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
# else
	arout = open("/dev/tty", O_WRONLY, 0);
# endif
	if (arout == -1) {
		reterr;
	}

	arena_tls = ffly_tls_alloc(sizeof(ff_uint_t));
	temp_tls = ffly_tls_alloc(sizeof(ff_uint_t));
	ffly_tls_toinit(tls_init);
	tls_init();
	ffly_process_prep();
	init_pot(&main_pot);
	main_pot.end = brk((void*)0); // should check for error
	main_pot.top = main_pot.end;
	main_pot.flags |= USE_BRK;

	rodp r;
	rodp *p = rods;
	while(p != rods+64) {
		r = (rodp)_ffly_balloc(&main_pot, sizeof(struct rod));
		if (!r) {
			reterr;
		}

		*(*(p++) = r) =
			(struct rod){.lock=FFLY_MUTEX_INIT,.p=NULL, .no=(p-rods)-1};
	}

	while(p != rods+1)
		(*--p)->next = *(p-1);
	(*rods)->next = *(rods+63);
	retok;
}

ff_err_t _ffly_ar_cleanup(potp __pot) {
	brk(__pot->end);
}

ff_err_t ffly_ar_cleanup(void) {
	printf("brk depth: %u\n", main_pot.top-main_pot.end);
	close(arout);
	_ffly_ar_cleanup(&main_pot);
}

void pot_pr(potp __pot) {
	potp p = __pot;
	blockp blk = get_block(p, p->top_blk);
	ff_uint_t static depth = 0;
	if (is_null(p->top_blk)) return;
	if (p->fd != NULL) {
		printf("\ndepth, %u\n", depth++);
		pot_pr(p->fd);
		printf("**end\n");
	}
_next:
	printf("/-----------------------\\\n");
	printf("| size: %u, off: %x, pad: %u, inuse: %s\n",
		blk->size, blk->off, blk->pad, is_used(blk)?"yes":"no");

	printf("| prev: %x{%s}, next: %x{%s}, fd: %x{%s}, bk: %x{%s}\n",
		is_null(blk->prev)?0:blk->prev, is_null(blk->prev)?"dead":"alive", is_null(blk->next)?0:blk->next, is_null(blk->next)?"dead":"alive",
		is_null(blk->fd)?0:blk->fd, is_null(blk->fd)?"dead":"alive", is_null(blk->bk)?0:blk->bk, is_null(blk->bk)?"dead":"alive");

	printf("\\----------------------/\n");
	if (not_null(blk->next)) {
		blk = get_block(p, blk->next);
		goto _next;		   
	}
}
# include "../maths/abs.h"
void pot_pf(potp __pot) {
	potp p = __pot;
	ar_off_t *bin = p->bins;
	blockp blk, bk;
	ff_uint_t static depth = 0;
	if (p->fd != NULL) {
		if (!depth)
			printf("**start\n");
		printf("\ndepth, %u, %u\n", depth++, p->fd->off);
		pot_pf(p->fd);
		printf("**end\n");
	}

_next:
	bk = NULL;
	if (is_null(*bin)) goto _sk;
	if (*bin >= p->off) {
		printf("bin is messed up, at: %u, off: %u\n", bin-p->bins, *bin);
		goto _sk;
	}

	blk = get_block(p, *bin);
_fwd:
	if (bk != NULL) {
		printf("\\\n");
		printf(" > %u-bytes\n", ffly_abs((ff_int_t)bk->off-(ff_int_t)blk->off));
		printf("/\n");
	}
	printf("/-----------------------\\\n");
	printf("| size: %u\t\t| 0x%x, pad: %u\n", blk->size, blk->off, blk->pad);
	printf("\\----------------------/\n");
	if (not_null(blk->fd)) {
		bk = blk;
		blk = get_block(p, blk->fd);
		goto _fwd;
	}

_sk:
	if (bin != p->bins+(no_bins-1)) {
		bin++;
		goto _next;
	}
}

void ffly_arbl(void *__p) {
	blockp blk = (blockp)((ff_u8_t*)__p-block_size);
	printf("block: off: %x, size: %u, pad: %u\n", blk->off, blk->size, blk->pad);
}

void pr(void) {
	potp arena = cur_arena;
	printf("\n**** all ****\n");
	rodp r = *rods;
	potp p = &main_pot;
	ff_uint_t no = 0;
_next:
	if (p != NULL) {
		if (p == &main_pot)
			printf("~: main pot, no{%u}\n", no);
		printf("\npot, %u, used: %u, buried: %u, dead: %u, off: %u, pages: %u, blocks: %u, total: %u\n",
			no++, ffly_arused(p), ffly_arburied(p), ffly_ardead(p), p->off, p->page_c, p->blk_c, p->total);
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

void pf(void) {
	potp arena = cur_arena;
	printf("\n**** free ****\n");
	rodp r = *rods;
	potp p = &main_pot;
	ff_uint_t no = 0;
_next:
	if (p != NULL) {
		printf("\npot, %u, used: %u, buried: %u, dead: %u, off: %u, pages: %u, blocks: %u, total: %u\n",
			no++, ffly_arused(p), ffly_arburied(p), ffly_ardead(p), p->off, p->page_c, p->blk_c, p->total);
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
void static dispose(potp, ar_off_t, ff_uint_t);

/*
	hang the pots, dead parts will drop off
*/
void ffly_arhang(void) {
	void **arena_spot = spot+ffly_tls_get(arena_tls);
	potp arena = (potp)*arena_spot;
	potp p;
	if (!(p = arena)) {
# ifdef __ffly_debug
		printf("ar, pot has already been axed, or has been like this from the start.\n");
# endif
		return;
	}

	potp bk;
	while(p != NULL) {
		bk = p;
		lkpot(p);
		p = p->fd;

		if (bk->fd != NULL)
			bk->fd->bk = bk->bk;
		if (bk->bk != NULL)
			bk->bk->fd = bk->fd;
		
		bk->fd = NULL;
		bk->bk = NULL;
		if (!bk->off) {
			rfr(bk);
			free_pot(bk);
		} else {
			dispose(bk, bk->off, (bk->top-bk->end)-bk->off);
			ulpot(bk);
		}
	}

	*arena_spot = NULL;
}

/*
	has not been finished
	TODO:
*/

void static*
shrink_blk(potp __pot, blockp __b, ar_uint_t __size) {
	if (__b->size <= __size || is_free(__b)) {
# ifdef __ffly_debug
		printf("forbidden alteration.\n");
# endif
		return NULL;
	}

	ar_uint_t size;

	size = __b->size-__b->pad;
	void *p;
	if (!(p = ffly_balloc(__size))) {
		//error
	}

	lkpot(__pot);
	blockp rr = block_prev(__pot, __b); //rear
	blockp ft = block_next(__pot, __b); //front
//	blockp fwd = get_block(__pot, __b->fd);
//	blockp bck = get_block(__pot, __b->bk);
	struct block blk;

	void *ret = NULL;

	// how much to shrink
	ar_uint_t dif = (__b->size-__b->pad)-__size;
	ar_off_t *off = &__b->off;
	ff_u8_t freed, inuse;
# ifdef __ffly_debug
	__ffmod_debug
		ff_rat(_ff_rat_1, out, "attempting to shrink block by %u bytes.\n", dif);
# endif
	if (dif>MAX_SH) {
# ifdef __ffly_debug
		__ffmod_debug
			ff_rat(_ff_rat_1, out, "can't strink block, %u bytes is too much to cutoff.\n", dif);
# endif
		goto _r;	
	}

	if (dif <= 0x4 && __b->pad < 0x4) {
		__b->pad+=dif;
		ret = (void*)((ff_u8_t*)__b+block_size);
		goto _r;
	}

# ifdef __ffly_debug
		__ffmod_debug
			ff_rat(_ff_rat_1, out, "cant add as padding, skipping...\n");
# endif
	if (not_null(__b->prev)) {
		if ((freed = is_free(rr)) || ((inuse = is_used(rr)) && rr->size <= 0xf)) {
			if (freed) {
				__pot->buried+=dif;
				if (is_flag(rr->flags, BLK_LINKED))
					detatch(__pot, rr);
			}

			copy(p, (ff_u8_t*)__b+block_size, __size);
			*off+=dif;
			if (*off-dif == __pot->end_blk)
				__pot->end_blk = *off;
			rr->next = *off;
			if (not_null(__b->next))
				ft->prev = *off;
			__b->size-=dif;
			if (inuse)
				rr->pad+=dif;

			rr->size+=dif;
			rr->end+=dif;
			copy(&blk, __b, block_size);
			__b = (blockp)((ff_u8_t*)__b+dif);
			copy(__b, &blk, block_size);
			ret = (void*)((ff_u8_t*)__b+block_size);
			copy(ret, p, __size);
			if (freed) {
				ar_off_t *bin;
				if (not_null(rr->fd = *(bin = bin_at(__pot, rr->size)))) {
					get_block(__pot, *bin)->bk = rr->off;		
				}

				*bin = rr->off;
				rr->bk = AR_NULL;
			}
			goto _r;
		}
	}
# ifdef __ffly_debug
	ff_rat(_ff_rat_1, out, "can't shrink block.\n");
# endif
_r:
	ulpot(__pot);
	ffly_bfree(p);
	return ret;
}

void static*
grow_blk(potp __pot, blockp __b, ar_uint_t __size) {
	if (__b->size-__b->pad >= __size || is_free(__b)) {
# ifdef __ffly_debug
		printf("forbidden alteration.\n");
# endif
		return NULL;
	}

	ar_uint_t size;
	void *p;
	if (!(p = ffly_balloc(size = (__b->size-__b->pad)))) {
		//error
	}

	lkpot(__pot);
	blockp rr = block_prev(__pot, __b); //rear
	blockp ft = block_next(__pot, __b); //front
	struct block blk;

	void *ret = NULL;

	ar_uint_t dif = __size-(__b->size-__b->pad);
	ar_off_t *off = &__b->off;
	ff_u8_t freed, inuse;
# ifdef __ffly_debug
	__ffmod_debug
		ff_rat(_ff_rat_1, out, "attempting to grow block by %u bytes.\n", dif);
# endif
	if (dif>MAX_GR) {
# ifdef __ffly_debug
		__ffmod_debug
			ff_rat(_ff_rat_1, out, "can't grow block, %u bytes is to much to add on.\n", dif);
# endif
		goto _r;
	}

	if (__b->pad >= dif) {
		__b->pad-=dif;
		ret = (void*)((ff_u8_t*)__b+block_size);
		goto _r;
	}

	if (not_null(__b->prev)) {
		if (((freed = is_free(rr)) && rr->size > dif<<1) || ((inuse = is_used(rr)) && rr->pad >= dif)) {
			if (freed) {
				__pot->buried-=dif;
				if (is_flag(rr->flags, BLK_LINKED))
					detatch(__pot, rr);
			}
			copy(p, (ff_u8_t*)__b+block_size, size);
			*off-=dif;
			if (*off+dif == __pot->end_blk)
				__pot->end_blk = *off;
			rr->next = *off;
			if (not_null(__b->next))
				ft->prev = *off;
			__b->size+=dif;
			rr->size-=dif;
			rr->end-=dif;
			if (inuse)
				rr->pad-=dif;
			copy(&blk, __b, block_size);
			__b = (blockp)((ff_u8_t*)__b-dif);
			copy(__b, &blk, block_size);
			ret = (void*)((ff_u8_t*)__b+block_size);
			copy(ret, p, size);
			if (freed) {
				ar_off_t *bin;
				if (not_null(rr->fd = *(bin = bin_at(__pot, rr->size)))) {
					get_block(__pot, *bin)->bk = rr->off;
				}

				*bin = rr->off;
				rr->bk = AR_NULL;
			}
			goto _r;
		}
	}

	if (not_null(__b->next)) {
		if (is_free(ft)) {
			
		}
	}
# ifdef __ffly_debug
	ff_rat(_ff_rat_1, out, "can't grow block.\n");
# endif
_r:
	ulpot(__pot);
	ffly_bfree(p);
	return ret;
}

// user level
void*
ffly_arsh(void *__p, ff_uint_t __to) {
	potp arena = cur_arena;
	blockp b = (blockp)((ff_u8_t*)__p-block_size);
	potp p;

	p = get_pot(b);
	void *ret;
	
	ret = shrink_blk(p, b, __to);
	return ret;
}

void*
ffly_argr(void *__p, ff_uint_t __to) {
	potp arena = cur_arena;
	blockp b = (blockp)((ff_u8_t*)__p-block_size);
	potp p;

	p = get_pot(b);
	void *ret; 
	
	ret = grow_blk(p, b, __to);
	return ret;
}

potp alloc_pot(ff_uint_t __size) {
	potp p;
	if (!(p = (potp)_ffly_balloc(&main_pot, pot_size))) {
		// err
	}

	__size+=sizeof(potp*);
	init_pot(p);
	p->page_c = (__size>>PAGE_SHIFT)+((__size&((~(ff_u64_t)0)>>(64-PAGE_SHIFT)))>0);
	ff_uint_t size = p->page_c*PAGE_SIZE;
	void *pp = _ffly_balloc(&main_pot, size);
	*(potp*)pp = p;
	p->end = (ff_u8_t*)pp+sizeof(potp*);
	p->top = (void*)((ff_u8_t*)pp+size);
	p->total = p->top-p->end;
	return p;
}

void free_pot(potp __pot) {
	_ffly_bfree(&main_pot, (ff_u8_t*)__pot->end-sizeof(rodp*));
	_ffly_bfree(&main_pot, __pot);
}

void*
_ffly_balloc(potp __pot, ff_uint_t __bc) {
	lkpot(__pot);
	ar_off_t *bin;
	bin = bin_at(__pot, __bc);
_bk:
	if (not_null(*bin)) {
		ar_off_t cur = *bin;
		while(not_null(cur)) {
			blockp blk;
			if ((blk = get_block(__pot, cur))->size >= __bc) {
				detatch(__pot, blk);
				blk->pad = blk->size-__bc;
				blk->flags = (blk->flags&~BLK_FREE)|BLK_USED|BLK_LINKED;
				__pot->buried-=blk->size;

				/*
				* if block exceeds size then trim it down and split the block into two parts.
				*/
				ar_uint_t junk;
				if ((junk = (blk->size-__bc)) > block_size+TRIM_MIN) {
					__pot->blk_c++;

					ar_off_t off = blk->off+block_size+__bc;
					blockp p; 
					*(p = (blockp)((ff_u8_t*)__pot->end+off)) = (struct block){
						.prev = blk->off, .next = blk->next,
						.size = junk-block_size, .off = off, .end = blk->end,
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
						get_block(__pot, p->next)->prev = p->off;
					ulpot(__pot);
					_ffly_bfree(__pot, (void*)((ff_u8_t*)p+block_size));
					goto _sk;
				}
				ulpot(__pot);
			_sk:
				return (void*)((ff_u8_t*)blk+block_size);
			}

			blockp rr = block_prev(__pot, blk);
			blockp ft = block_next(__pot, blk);
			ff_uint_t canadd = 0;
			ff_u8_t flags = 0x0;
			if (not_null(blk->prev)) {
				if (is_free(rr)) {
					canadd+=rr->size+block_size;
					flags |= 0x1;
				}
			}
			if (not_null(blk->next)) {
				if (is_free(ft)) {
					canadd+=ft->size+block_size;
					flags |= 0x2;
				}
			}

			if (blk->size+canadd >= __bc) {
				detatch(__pot, blk);
				if (flags&0x1) {
					__pot->blk_c--;
					__pot->buried+=block_size;
					if (is_flag(rr->flags, BLK_LINKED))
						detatch(__pot, rr);
					decouple(__pot, blk);
					rr->size+=blk->size+block_size;
					rr->end = blk->end;
					rr->next = blk->next;
					blk = rr;
				}

				if (flags>>1&0x1) {
					if (is_flag(ft->flags, BLK_LINKED))
						detatch(__pot, ft);
					__pot->blk_c--;
					__pot->buried+=block_size;
					decouple(__pot, ft);
					blk->size+=ft->size+block_size;
					blk->end = ft->end;
					blk->next = ft->next;
				}
		
				blk->pad = blk->size-__bc;
				blk->flags = (blk->flags&~BLK_FREE)|BLK_USED|BLK_LINKED; 
				ulpot(__pot);
				return (void*)((ff_u8_t*)blk+block_size);
			}
			cur = blk->fd;
		}
	}

	// for now
	if (bin != __pot->bins+(no_bins-1)) {
		bin++;
		goto _bk;
	}

	ar_uint_t size = align_to(block_size+__bc, ALIGN);
	ar_off_t top = __pot->off+size;
	if (is_flag(__pot->flags, USE_BRK)) {
		ar_uint_t page_c;
		if ((page_c = ((top>>PAGE_SHIFT)+((top&((~(ff_u64_t)0)>>(64-PAGE_SHIFT)))>0))) > __pot->page_c) {
		//if ((page_c = ((top>>PAGE_SHIFT)+((top-((top>>PAGE_SHIFT)*PAGE_SIZE))>0))) > __pot->page_c) {
			if (brk(__pot->top = (void*)((ff_u8_t*)__pot->end+((__pot->page_c = page_c)*PAGE_SIZE))) == (void*)-1) {
# ifdef __ffly_debug
				printf("error: brk.\n");
# endif
				abort();
			}
		}
	} else {
		if (top >= __pot->top-__pot->end) {
			ulpot(__pot);
			return NULL;
		}
	}

	blockp blk;
	*(blk = (blockp)((ff_u8_t*)__pot->end+__pot->off)) = (struct block) {
		.prev = __pot->end_blk, .next = AR_NULL,
		.size = size-block_size, .off = __pot->off, .end = top,
		.fd = AR_NULL, .bk = AR_NULL,
		.flags = BLK_USED
	};

	blk->pad = blk->size-__bc;
	if (is_null(__pot->top_blk))
		__pot->top_blk = __pot->off;
		
	if (not_null(__pot->end_blk))
		get_block(__pot, __pot->end_blk)->next = __pot->off;
	__pot->end_blk = __pot->off;
	__pot->off = top;

	__pot->blk_c++;
	ulpot(__pot);
	return (void*)((ff_u8_t*)blk+block_size);
}

# include "../dep/bzero.h"
void*
ffly_balloc(ff_uint_t __bc) {
	void **arena_spot = spot+ffly_tls_get(arena_tls);
	potp arena = (potp)*arena_spot;
	if (!__bc) return NULL;
	if (__bc+block_size >= POT_SIZE) {
		void *p;
		if ((p = mmap(NULL, block_size+__bc, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0)) == (void*)-1) {
			// failure
			abort();
		}
		blockp blk = (blockp)p;
		blk->size = __bc;
		blk->flags = MMAPED;
		return (void*)((ff_u8_t*)p+block_size);
	}

	if (!arena) {
		printf("new arena, %u\n", ffly_tls_get(arena_tls));
		arena = alloc_pot(POT_SIZE);
		lkpot(arena);
		atr(arena, *rod_at(arena->end));
		ulpot(arena);
	}

	potp p = arena, t;
	void *ret;
_again:
	if (!(ret = _ffly_balloc(p, __bc))) {
		lkpot(p);
		if (p->fd != NULL){
			t = p->fd;
			ulpot(p);
			p = t;
			goto _again;
		}
		ulpot(p);
	}

	p = arena;
	if (!ret) {
		printf("new pot alloc, %u\n", ffly_tls_get(arena_tls));
		if (!(t = alloc_pot(POT_SIZE))) {
			abort();
		}

		atr(t, *rod_at(t->end));

		lkpot(p);
		lkpot(t);
		potp ft = p->fd;
		t->fd = ft;
		t->bk = p;
		p->fd = t;
		if (ft != NULL) {
			lkpot(ft);
			ft->bk = t;
			ulpot(ft);
		}

		ulpot(t);
		ulpot(p);
		p = t;
		goto _again;  
	}	

	*arena_spot = arena;
	return ret;
}

void
_ffly_bfree(potp __pot, void *__p) {
	blockp blk = (blockp)((ff_u8_t*)__p-block_size);
# ifdef ERASE
	ar_erase(__p, blk->size-blk->pad);
# endif
	lkpot(__pot);
	if (is_free(blk)) {
# ifdef __ffly_debug
		printf("error: block has already been freed, at: %p\n", __p);
# endif
		goto _end;
	}

	if (blk->size<16) {
		if (not_null(blk->prev) && not_null(blk->next)) {
			blockp ft = block_next(__pot, blk);
			blockp rr = block_prev(__pot, blk);

			// used-free or free-used
			if (is_flag(ft->flags|rr->flags, BLK_USED|BLK_FREE)) {
				blk->flags = (blk->flags&~(BLK_USED|BLK_LINKED))|BLK_FREE;
				ulpot(__pot);
# ifdef __ffly_debug
				__ffmod_debug
					ff_rat(_ff_rat_0, out, "speed free\n");
# endif
				__pot->buried+=blk->size;
				return;
			}
		}
	}

	decouple(__pot, blk);
# ifdef __ffly_debug
	__ffmod_debug
		ff_rat(_ff_rat_0, out, "to free: %u\n", blk->size);
# endif
	blockp prev, next, top = NULL, end = NULL;
	if (not_null(blk->prev)) {
		prev = block_prev(__pot, blk);
		while(is_free(prev) && !is_static(prev)) {
			__pot->blk_c--;
			__pot->buried-=prev->size;
# ifdef __ffly_debug
			__ffmod_debug
				ff_rat(_ff_rat_0, out, "found free space above, %u\n", prev->size);
# endif
			if (is_flag(prev->flags, BLK_LINKED))
				detatch(__pot, prev);
			decouple(__pot, prev);
# ifdef __ffly_debug
			__ffmod_debug
				ff_rat(_ff_rat_0, out, "total freed: %u\n", prev->end-prev->off); 
# endif
			top = prev;
			if (is_null(prev->prev)) break;
			prev = block_prev(__pot, prev);
		}
	}

	if (not_null(blk->next)) {
		next = block_next(__pot, blk);
		while(is_free(next) && !is_static(next)) {
			__pot->blk_c--;
			__pot->buried-=next->size;
# ifdef __ffly_debug
			__ffmod_debug
				ff_rat(_ff_rat_0, out, "found free space below, %u\n", next->size);
# endif
			if (is_flag(next->flags, BLK_LINKED))
				detatch(__pot, next);
			decouple(__pot, next);
# ifdef __ffly_debug
			__ffmod_debug
				ff_rat(_ff_rat_0, out, "total freed: %u\n", next->end-next->off);
# endif
			end = next;
			if (is_null(next->next)) break;
			next = block_next(__pot, next);
		}
	}

	if (top != NULL) {
		top->size = ((top->end = blk->end)-top->off)-block_size;
		top->next = blk->next;
		blk = top;
	}

	if (end != NULL) {
		blk->size = ((blk->end = end->end)-blk->off)-block_size;
		blk->next = end->next;
	}

	// only used when used
	blk->pad = 0;
# ifdef __ffly_debug
	__ffmod_debug
		ff_rat(_ff_rat_0, out, "freed: %u, %u\n", blk->size, blk->off);
# endif
	if (blk->off>=__pot->off) {
# ifdef __ffly_debug
		printf("somthing is wong.\n");
# endif
	}

	if (blk->end == __pot->off) {
		__pot->blk_c--;
		__pot->off = blk->off;
		if (is_flag(__pot->flags, USE_BRK)) { 
			ff_uint_t page_c;
			if ((page_c = ((__pot->off>>PAGE_SHIFT)+((__pot->off&((~(ff_u64_t)0)>>(64-PAGE_SHIFT)))>0))) < __pot->page_c) {
//			if ((page_c = ((__pot->off>>PAGE_SHIFT)+((__pot->off-((__pot->off>>PAGE_SHIFT)*PAGE_SIZE))>0))) < __pot->page_c) {
				if (brk(__pot->top = (void*)((ff_u8_t*)__pot->end+((__pot->page_c = page_c)*PAGE_SIZE))) == (void*)-1) {
# ifdef __ffly_debug
					printf("error: brk.\n");
# endif
					abort();
				}
			}
		}
		goto _end;
	}

	__pot->buried+=blk->size;

	recouple(__pot, blk);
	blk->flags = (blk->flags&~BLK_USED)|BLK_FREE|BLK_LINKED;
	ar_off_t bin;
	if (not_null(bin = get_bin(__pot, blk->size))) {
		get_block(__pot, bin)->bk = blk->off;
		blk->fd = bin;
	}
	*(__pot->bins+bin_no(blk->size)) = blk->off;

_end:
	ulpot(__pot);
}

/*
	find pot by pointer
*/
potp static
lookup(void *__p) {
	potp p, bk;
	rodp r, rr, beg;

	if ((p = (potp)*(spot+ffly_tls_get(arena_tls))) != NULL) {
		lkpot(p);
		while(!(__p >= p->end && __p < p->top)) {
			bk = p;
			ulpot(bk);
			if (!(p = p->fd)) {
				goto _sk;				
			}
			lkpot(p);
		}
		ulpot(p);
		return p;
	}
_sk:
	r = *rod_at(__p);
	beg = r;
	lkrod(r);
	while(!(p = (rr = r)->p)) {
		if ((r = r->next) == beg) {
# ifdef __ffly_debug
			printf("looked at all rods but found nothing.\n");
# endif
			ulrod(rr);
			abort();
		}
		ulrod(rr);
		lkrod(r);
	}
	ulrod(r);

	lkpot(p);
	while(!(__p >= p->end && __p < p->top)) {
		bk = p;
		p = p->next;
		ulpot(bk);
		if (!p) {
			rodp bk;
			lkrod(r);
			while(!(p = (r = (bk = r)->next)->p) && r != beg) {
				ulrod(bk);
				lkrod(r);
			}
			ulrod(bk);

			if (r == beg) {
# ifdef __ffly_debug
				printf("error: could not find pot associated with pointer.\n");
# endif
				abort();
			}
		}
		lkpot(p);
	}
	ulpot(p);
	return p;
}

void
ffly_bfree(void *__p) {
	void **arena_spot = spot+ffly_tls_get(arena_tls);
	potp arena = (potp)*arena_spot;
	if (!__p) {
# ifdef __ffly_debug
		printf("error: got null ptr.\n");
# endif
		return;
	}
	blockp blk = (blockp)((ff_u8_t*)__p-block_size);
	if (is_flag(blk->flags, MMAPED)) {
		munmap((void*)blk, block_size+blk->size);
		return;
	}

	potp p, bk;
	rodp r;

	p = get_pot(blk);

	lkpot(p);
	while(!(__p >= p->end && __p < p->top)) {
		bk = p;
		if (!(p = p->fd)) {
			ulpot(bk);
			printf("block not located.\n");
			return;
		}
		ulpot(bk);
	}
	ulpot(p);

	_ffly_bfree(p, __p);
	// free pot as we don't need it
	lkpot(p);
	if (!p->off && p != arena) {
		potp ft = p->fd;
		potp rr = p->bk;
		if (ft != NULL) {
			lkpot(ft);
			ft->bk = rr;
		}

		if (rr != NULL) {
			lkpot(rr);
			rr->fd = ft;
		}

		if (ft != NULL) {
			ulpot(ft);
		}
		if (rr != NULL) {
			ulpot(rr);
		}
		rfr(p);
		ulpot(p);
		free_pot(p);
		goto _end;
	}
	ulpot(p);
_end:
	*arena_spot = arena;
}

void*
ffly_brealloc(void *__p, ff_uint_t __bc) {
	if (!__p) {
# ifdef __ffly_debug
		printf("error: got null ptr.\n");
# endif
		return NULL;
	}

	blockp blk = (blockp)((ff_u8_t*)__p-block_size);
	ar_uint_t size = blk->size-blk->pad;
	ar_int_t dif = (ar_int_t)size-(ar_int_t)__bc;
	void *p;
	if (!is_flag(blk->flags, MMAPED)) {
		if (dif>0) {
# ifdef __ffly_debug
			__ffmod_debug
				ff_rat(_ff_rat_1, out, "shrink.\n");
# endif
			if ((p = ffly_arsh(__p, __bc)) != NULL) {
				ff_rat(_ff_rat_1, out, "shrunk %u bytes.\n", dif);
				return p;
			}
# ifdef __ffly_debug
			__ffmod_debug
				ff_rat(_ff_rat_1, out, "can't shrink block.\n");
# endif
		} else if (dif<0) {
# ifdef __ffly_debug
			__ffmod_debug
				ff_rat(_ff_rat_1, out, "grow.\n");
# endif
			if ((p = ffly_argr(__p, __bc)) != NULL) {
				ff_rat(_ff_rat_1, out, "grew %u bytes.\n", -dif);
				return p;
			}
# ifdef __ffly_debug
			__ffmod_debug
				ff_rat(_ff_rat_1, out, "can't grow block.\n");
# endif
		}
	}

	if (!(p = ffly_balloc(__bc))) {
		// failure
	}

# ifdef __ffly_debug
	__ffmod_debug
		ff_rat(_ff_rat_1, out, "prev size: %u, new size: %u, copysize: %u\n", size, __bc, size<__bc?size:__bc);
# endif
	copy(p, __p, size<__bc?size:__bc);
	ffly_bfree(__p);
	return p;
}

// migrate to diffrent pot
void static
migrate(potp __spt, potp __dpt, blockp __src) {
	void *src = (void*)((ff_u8_t*)__src+block_size);
	void *dst = _ffly_balloc(__dpt, __src->size);
	copy(dst, src, __src->size);
	_ffly_bfree(__spt, src);
}

void static
reloc(potp __pot, blockp __p, ar_off_t __off) {
	
}

void static
ffly_reloc(void *__p, ar_off_t __off) {
	
}

void static
atb(potp __pot, blockp __b) {
	ar_off_t *bin;
	if (not_null(__b->fd = *(bin = bin_at(__pot, __b->size)))) {
		get_block(__pot, *bin)->bk = __b->off;
	}

	*bin = __b->off;
	__b->bk = AR_NULL;
}

// must be apart of main pot
void
dispose(potp __pot, ar_off_t __start, ff_uint_t __span) {
	lkpot(__pot);
	blockp b = (blockp)((ff_u8_t*)__pot->end-(block_size+sizeof(potp*)));
	blockp ft = block_next(&main_pot, b);
	blockp rr = block_prev(&main_pot, b);

	ar_off_t end = __start+__span;
	struct block tmp;
	printf("%u, %u\n", end, (__pot->top-__pot->end));

	if (!__start) {
		if (not_null(b->prev)) {
			if (is_free(rr)) {
				if (is_flag(rr->flags, BLK_LINKED))
					detatch(&main_pot, rr);
				else
					rr->flags |= BLK_LINKED;
				rr->size+=__span;
				rr->end+=__span;

				rr->next+=__span;

				if (not_null(b->next))
					ft->prev+=__span;

				copy(&tmp, b, block_size);
				b = (blockp)((ff_u8_t*)b+__span);
				copy(b, &tmp, block_size);
				b->off+=__span;
				b->size-=__span;
				atb(&main_pot, rr);		
				__pot->end = (void*)((ff_u8_t*)__pot->end+__span);
				*(potp*)((ff_u8_t*)__pot->end-sizeof(potp*)) = __pot;
				goto _end;
			}
		}
	}

	if (end == (__pot->top-__pot->end)) {
		if (not_null(b->next)) {
			if (is_free(ft)) {
				if (is_flag(ft->flags, BLK_LINKED))
					detatch(&main_pot, ft);
				else
					ft->flags |= BLK_LINKED;
				copy(&tmp, ft, block_size);
				ft = (blockp)((ff_u8_t*)ft-__span);
				copy(ft, &tmp, block_size);

				if (not_null(ft->next))
					block_next(&main_pot, ft)->prev-=__span;
				ft->off-=__span;
				b->next-=__span;
				ft->size+=__span;

				atb(&main_pot, ft);
				__pot->top = (void*)((ff_u8_t*)__pot->top-__span);
				goto _end;
			}
		}

		if (__span>block_size) {
			lkpot(&main_pot);
			main_pot.blk_c++;
			blockp p;
			*(p = (blockp)((ff_u8_t*)__pot->end+__start)) = (struct block) {
				.prev = b->off, .next = b->next,
				.size = __span-block_size, .off = b->off+__start+sizeof(potp*)+block_size, .end = b->off+__start+__span+block_size+sizeof(potp*),
				.fd = AR_NULL, .bk = AR_NULL,
				.flags = BLK_FREE,
				.pad = 0
			};
			printf("block: %x\n", p->off);
			__pot->top = (void*)((ff_u8_t*)__pot->top-__span);
			__pot->total-=__span;
			b->size-=__span;
			b->end-=__span;

			if (not_null(b->next))
				ft->prev = p->off;
			b->next = p->off;

			if (main_pot.end_blk == b->off)
				main_pot.end_blk = p->off;
			atb(&main_pot, p);
			ulpot(&main_pot);
			goto _end;
		}
	}

	if (__span>block_size) {
		lkpot(&main_pot);
		main_pot.blk_c++;
		blockp p;
		*(p = (blockp)((ff_u8_t*)__pot->end+__start)) = (struct block) {
			.prev = b->off, .next = b->next,
			.size = __span-block_size, .off = b->off+__start+sizeof(potp*)+block_size, .end = b->off+__start+__span+block_size+sizeof(potp*),
			.fd = AR_NULL, .bk = AR_NULL,
			.flags = BLK_FREE|BLK_STATIC,
			.pad = 0
		};

		printf("block: %x\n", p->off);
		if (not_null(b->next))
			ft->prev = p->off;
		b->next = p->off;

		if (main_pot.end_blk == b->off)
			main_pot.end_blk = p->off;
		atb(&main_pot, p);
		ulpot(&main_pot);
		goto _end;
	}

	printf("failed.\n");
_end:
	ulpot(__pot);
	return;
}

void ffly_dispose(void *__p, ff_uint_t __span) {
//	void *p0 = _ffly_balloc(&main_pot, 20);
//	void *p1 = _ffly_balloc(&main_pot, 20);
//	_ffly_bfree(&main_pot, p0);
	potp p = lookup(__p);
	if(!p) {
		printf("pot is dead.\n");
		return;
	}
	dispose(p, (ff_u8_t*)__p-(ff_u8_t*)p->end, __span);
}

