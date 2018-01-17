# include "allocr.h"
void* ffly_brk(void*);
# include <sys/resource.h>
# include <sys/mman.h>
# include "../system/util/checksum.h"
ffly_err_t ffly_printf(char*, ...);
# define ALIGN 2
# define align_to(__no, __to)(((__no)+((__to)-1))&((~(__to))+1))
# define is_aligned_to(__no, __align)(((__no&__align) == __align)||!(__no&__align))
# define PAGE_SHIFT 8
# define PAGE_SIZE (1<<PAGE_SHIFT)
typedef mdl_u32_t ar_off_t;
typedef mdl_u32_t ar_size_t;
typedef mdl_u32_t ar_uint_t;

# ifndef NULL
#   define NULL ((void*)0)
# endif

# define BLK_FREE 0x1
# define BLK_USED 0x2
# define FIXED 0x1

# define is_flag(__flags, __flag) \
    ((__flags&__flag)==__flag)

# define is_free(__blk) \
    is_flag((__blk)->flags, BLK_FREE)
# define is_used(__blk) \
    is_flag((__blk)->flags, BLK_USED)

# define get_blk(__pot, __off) \
    ((blkdp)(((mdl_u8_t*)(__pot)->top)+(__off)))

# define next_blk(__pot, __blk) \
    get_blk(__pot, (__blk)->next)
# define prev_blk(__pot, __blk) \
    get_blk(__pot, (__blk)->prev)

# define AR_NULL ((ar_off_t)~0)
# define is_null(__p) (__p==AR_NULL) 
# define not_null(__p) (__p!=AR_NULL)

# define no_bins 255
# define bin_no(__bc) \
    ((__bc)>>24)

# define get_bin(__pot, __bc) \
    *((__pot)->bins+bin_no(__bc))

//# define DEBUG
void static copy(void *__dst, void *__src, mdl_uint_t __bc) {
    mdl_u8_t *p = (mdl_u8_t*)__dst;
    mdl_u8_t *end = p+__bc;
    while(p != end) {
        *p = *((mdl_u8_t*)__src+(p-(mdl_u8_t*)__dst));
        p++;
    }
}

struct pot {
    ar_uint_t page_c;
    ar_off_t off, top_blk, end_blk;
    void *top, *end;
    mdl_u8_t flags;
    ar_off_t bins[no_bins];
};

typedef struct pot* potp;
struct pot main_pot;

struct blkd {
    ar_off_t prev, next, fd, bk;
    ar_size_t size;
    ar_uint_t pad;
    ar_off_t off, end;
    mdl_u8_t flags;
} __attribute__((packed));

typedef struct blkd* blkdp;
# define blkd_size sizeof(struct blkd)
void static unlink(potp __pot, blkdp __blk) {
    if (__blk->off == get_bin(__pot, __blk->size)) {
        *(__pot->bins+bin_no(__blk->size)) = __blk->fd;
        if (not_null(__blk->fd))
            get_blk(__pot, __blk->fd)->bk = AR_NULL; 
    } else {
        if (not_null(__blk->fd))
            get_blk(__pot, __blk->fd)->bk = __blk->bk;
        if (not_null(__blk->bk))
            get_blk(__pot, __blk->bk)->fd = __blk->fd;
    }

    __blk->fd = AR_NULL;
    __blk->bk = AR_NULL;
}

void static recouple(potp __pot, blkdp __blk) {
    if (__pot->top == (void*)__blk)
        __pot->top_blk = __blk->off;
    if (__pot->off == __blk->end)
        __pot->end_blk = __blk->off;

    if (not_null(__blk->next))
        next_blk(__pot, __blk)->prev = __blk->off;
    if (not_null(__blk->prev))
        prev_blk(__pot, __blk)->next = __blk->off; 
}

void static decouple(potp __pot, blkdp __blk) {
    if (__blk->off == __pot->top_blk)
        __pot->top_blk = __blk->next;
    if (__blk->off == __pot->end_blk)
        __pot->end_blk = __blk->prev;

    if (not_null(__blk->next))
        next_blk(__pot, __blk)->prev = __blk->prev;
    if (not_null(__blk->prev))
        prev_blk(__pot, __blk)->next = __blk->next;
}

ffly_err_t init_pot(potp __pot) {
    __pot->top = ffly_brk((void*)0);
    __pot->end = __pot->top;
    __pot->top_blk = AR_NULL;
    __pot->end_blk = AR_NULL;
    __pot->off = 0;
    __pot->page_c = 0;
    __pot->flags = 0;
    ar_off_t *bin = __pot->bins;
    while(bin != __pot->bins+no_bins)
        *(bin++) = AR_NULL;
}

ffly_err_t _ffly_ar_cleanup(potp __pot) {
    ffly_brk(__pot->top);
}

ffly_err_t ffly_ar_cleanup() {
    _ffly_ar_cleanup(&main_pot);
}

void pr() {
    ffly_printf("\nblocks:\n");
    if (is_null(main_pot.top_blk)) return;
    blkdp blk = get_blk(&main_pot, main_pot.top_blk);
    _next:

    ffly_printf("|-----------------------|\n");
    ffly_printf("| size: %u\t\t| 0x%x\n", blk->size, blk->off);
    if (not_null(blk->next)) {
        blk = get_blk(&main_pot, blk->next);
        goto _next;
    }
    ffly_printf("|-----------------------|\n");
    ffly_printf("used: %u\n", main_pot.off);
}

void pf() {
    ffly_printf("\nfree blocks: 0x%x\n", main_pot.off);
    ar_off_t *bin = main_pot.bins;
    blkdp blk;
    _next:
    if (is_null(*bin)) goto _sk;
    blk = get_blk(&main_pot, *bin);
    _fd:

    ffly_printf("|-----------------------| bin: %u\n", bin-main_pot.bins);
    ffly_printf("| size: %u\t\t| {0x%x, 0x%x}, fd: 0x%x, bk: 0x%x\n", blk->size, blk->off, blk->end, blk->fd, blk->bk);
    if (not_null(blk->fd)) {
        blk = get_blk(&main_pot, blk->fd);
        goto _fd;
    }

    _sk:
    if (bin < main_pot.bins+(no_bins-1)) {
        bin++;
        goto _next;
    }

    ffly_printf("|-----------------------|\n");
}

void _ffly_free(potp, void*);
void* _ffly_alloc(potp __pot, mdl_uint_t __bc) {
    ar_off_t bin;
    if (not_null((bin = get_bin(__pot, __bc)))) {
        while(not_null(bin)) {
            blkdp blk = get_blk(__pot, bin);
            ar_uint_t bc;
            if (blk->size >= (bc = align_to(__bc, ALIGN))) {
                unlink(__pot, blk);
                blk->pad = blk->size-__bc;
                blk->flags = (blk->flags&~BLK_FREE)|BLK_USED;

                ar_uint_t junk;
                if ((junk = (blk->size-bc)) > blkd_size) {
                    ar_off_t off = blk->off+blkd_size+bc;
                    blkdp p = (blkdp)((mdl_u8_t*)__pot->top+off); 
                    *p = (struct blkd){
                        .prev = blk->off, .next = blk->next,
                        .size = junk-blkd_size, .off = off, .end = blk->end,
                        .fd = AR_NULL, .bk = AR_NULL,
                        .flags = BLK_FREE
                    };

                    blk->size = bc;
                    blk->next = p->off;
                    blk->end = off;
                    if (__pot->off == p->end) {
                        __pot->end_blk = p->off;
                    }

                    if (not_null(p->next))
                        get_blk(__pot, p->next)->prev = p->off;
                    _ffly_free(__pot, (void*)((mdl_u8_t*)p+blkd_size));
                }
                return (void*)((mdl_u8_t*)blk+blkd_size);
            } 
           
            bin = blk->fd;
        }
    }

    mdl_uint_t size = align_to(blkd_size+__bc, ALIGN);
    mdl_uint_t page_c;
    ar_off_t end = __pot->off+size;
    if ((page_c = ((end<<PAGE_SHIFT)+((end-((end<<PAGE_SHIFT)*PAGE_SIZE))>0))) >= __pot->page_c) {
        ffly_brk(__pot->end = (__pot->top+((__pot->page_c = page_c)*PAGE_SIZE)));
    }

    blkdp blk = (blkdp)((mdl_u8_t*)__pot->top+__pot->off);
    *blk = (struct blkd) {
        .prev = __pot->end_blk, .next = AR_NULL,
        .size = size-blkd_size, .off = __pot->off, .end = end,
        .fd = AR_NULL, .bk = AR_NULL,
        .flags = BLK_USED
    };

    blk->pad = blk->size-__bc;
    if (is_null(__pot->top_blk))
        __pot->top_blk = __pot->off;
        
    if (not_null(__pot->end_blk))
        get_blk(__pot, __pot->end_blk)->next = __pot->off;
    __pot->end_blk = __pot->off;
    __pot->off = end;
    return (void*)((mdl_u8_t*)blk+blkd_size);
}

mdl_u8_t inited = 0;
void* ffly_alloc(mdl_uint_t __bc) {
    if (!inited) {
        init_pot(&main_pot);
        inited = 1;
    }

    return _ffly_alloc(&main_pot, __bc);
}

void _ffly_free(potp __pot, void *__p) {
    blkdp blk = (blkdp)((mdl_u8_t*)__p-blkd_size);
    decouple(__pot, blk);
# ifdef DEBUG
    ffly_printf("to free: %u\n", blk->size);
# endif
    blkdp prev, next, top = NULL, end = NULL;
    if (not_null(blk->prev)) {
        prev = prev_blk(__pot, blk);
        while(is_free(prev)) {
# ifdef DEBUG
            ffly_printf("found free space above, %u\n", prev->size);
# endif
            unlink(__pot, prev);
            decouple(__pot, prev);
# ifdef DEBUG
            ffly_printf("total freed: %u\n", prev->end-prev->off); 
# endif
            top = prev;
            if (is_null(prev->prev)) break;
            prev = prev_blk(__pot, prev);
        }
    }

    if (not_null(blk->next)) {
        next = next_blk(__pot, blk);
        while(is_free(next)) {
# ifdef DEBUG
            ffly_printf("found free space below, %u\n", next->size);
# endif
            unlink(__pot, next);
            decouple(__pot, next);
# ifdef DEBUG
            ffly_printf("total freed: %u\n", next->end-next->off);
# endif
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

    if (blk->end == __pot->off) {
        __pot->off = blk->off;
        return;
    }

    recouple(__pot, blk);
    blk->flags = (blk->flags&~BLK_USED)|BLK_FREE;
    ar_off_t bin;
    if (not_null((bin = get_bin(__pot, blk->size)))) {
        get_blk(__pot, bin)->bk = blk->off;
        blk->fd = bin;
    }
    *(__pot->bins+bin_no(blk->size)) = blk->off;
}

void ffly_free(void *__p) {
    if (!__p) return;
    _ffly_free(&main_pot, __p);
}

void *ffly_realloc(void *__p, mdl_uint_t __bc) {
    blkdp blk = (blkdp)((mdl_u8_t*)__p-blkd_size);
    void *p = ffly_alloc(__bc);

    ar_uint_t size = blk->size-blk->pad;
   // ffly_printf("%u\n", size);
    copy(p, __p, size<__bc?size:__bc);
    ffly_free(__p);
    return p;
}

