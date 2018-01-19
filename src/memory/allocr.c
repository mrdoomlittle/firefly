# include "allocr.h"
void* ffly_brk(void*);
# include <sys/resource.h>
# include <sys/mman.h>
# include "../system/util/checksum.h"
# include "../system/thread.h"
# include "../system/mutex.h"
ffly_err_t ffly_printf(char*, ...);
# define ALIGN 8
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
# define MMAPED 0x2

# define is_flag(__flags, __flag) \
    ((__flags&__flag)==__flag)

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
# define is_null(__p) (__p==AR_NULL) 
# define not_null(__p) (__p!=AR_NULL)
# define POT_SIZE 0xfff

# define lock_pot(__pot) \
    ffly_mutex_lock(&(__pot)->lock)
# define unlock_pot(__pot) \
    ffly_mutex_unlock(&(__pot)->lock)

# define no_bins 80
# define bin_no(__bc) \
    (((mdl_u32_t)(__bc))>>3 > 16?16+(((mdl_u32_t)(__bc))>>7 > 16?16+(((mdl_u32_t)(__bc))>>11 > 16?16+(((mdl_u32_t)(__bc))>>15 > 16?16+(((mdl_u32_t)(__bc))>>19 > 16?16: \
    (((mdl_u32_t)(__bc))>>19)):(((mdl_u32_t)(__bc))>>15)):(((mdl_u32_t)(__bc))>>11)):(((mdl_u32_t)(__bc))>>7)):(((mdl_u32_t)(__bc))>>3))

# define get_bin(__pot, __bc) \
    *((__pot)->bins+bin_no(__bc))

//# define DEBUG
void static copy(void *__dst, void *__src, mdl_uint_t __bc) {
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

struct pot {
    ar_uint_t page_c;
    ar_off_t off, top_blk, end_blk;
    void *top, *end;
    mdl_u8_t flags;
    ar_off_t bins[no_bins];
    struct pot *next;
    ffly_mutex_t lock;
};

typedef struct pot* potp;
struct pot main_pot;

__thread struct pot arena;
__thread mdl_u8_t inited = 0;

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
    __pot->end = ffly_brk((void*)0);
    __pot->top = __pot->end;
    __pot->top_blk = AR_NULL;
    __pot->end_blk = AR_NULL;
    __pot->off = 0;
    __pot->page_c = 0;
    __pot->flags = 0;
    __pot->next = NULL;
    __pot->flags = 0x0;
    __pot->lock = FFLY_MUTEX_INIT;
    ar_off_t *bin = __pot->bins;
    while(bin != __pot->bins+no_bins)
        *(bin++) = AR_NULL;
}

ffly_err_t ffly_ar_init() {
    init_pot(&main_pot);
}

ffly_err_t _ffly_ar_cleanup(potp __pot) {
    ffly_brk(__pot->end);
}

ffly_err_t ffly_ar_cleanup() {
    _ffly_ar_cleanup(&main_pot);
}

void pr() {
    struct pot *p = &main_pot;
    mdl_uint_t no = 0;
    blkdp blk;
    _next_pot:
    ffly_printf("****pot %u\n", no++);
    ffly_printf("\nblocks:\n");
    if (is_null(p->top_blk)) return;
    blk = get_blk(p, p->top_blk);
    _next:

    ffly_printf("|-----------------------|\n");
    ffly_printf("| size: %u\t\t| 0x%x\n", blk->size, blk->off);
    if (not_null(blk->next)) {
        blk = get_blk(p, blk->next);
        goto _next;
    }
    ffly_printf("|-----------------------|\n");
    ffly_printf("used: %u\n", p->off);
    if (p->next != NULL) {
        p = p->next;
        goto _next_pot;
    }
}

void pf() {
    struct pot *p = &main_pot;
    mdl_uint_t no = 0;
    ar_off_t *bin;

    _next_pot:
    ffly_printf("****pot %u\n", no++);
    ffly_printf("\nfree blocks: 0x%x\n", p->off);
    bin = p->bins;
    blkdp blk;
    _next:
    if (is_null(*bin)) goto _sk;
    blk = get_blk(p, *bin);
    _fd:

    ffly_printf("|-----------------------| bin: %u\n", bin-p->bins);
    ffly_printf("| size: %u\t\t| {0x%x, 0x%x}, fd: 0x%x, bk: 0x%x\n", blk->size, blk->off, blk->end, blk->fd, blk->bk);
    if (not_null(blk->fd)) {
        blk = get_blk(p, blk->fd);
        goto _fd;
    }

    _sk:
    if (bin < p->bins+(no_bins-1)) {
        bin++;
        goto _next;
    }

    ffly_printf("|-----------------------|\n");
    if (p->next != NULL) {
        p = p->next;
        goto _next_pot;
    }
}

/*
void static*
shrink_blk(potp __pot, blkdp __blk, ar_uint_t __size) {
    blkdp prev = blk_prev(__pot, __blk);
    blkdp next = blk_next(__pot, __blk);
    blkdp fwd = get_blk(__pot, __blk->fd);
    blkdp bck = get_blk(__pot, __blk->bk);
    struct blkd blk;

    ar_unit_t dif = (__blk->size-__blk->pad)-__size;
    ar_off_t off;
    void *p;
    if (not_null(__blk->prev)) {
        if (is_free(prev) && prev->size > dif<<1) {
            off = __blk->off-dif;
            __blk->off = off;
            prev->next = off;
            next->prev = off;
            if (not_null(__blk->fd))
                fwd->bk = off;
            if (not_null(__blk->bk))
                bck->fd = off;
            blk->size-=dif;
            copy(&blk, __blk, blkd_size);
            __blk = (blkdp)((mdl_u8_t*)__blk-dif);
            copy(__blk, &blk, blkd_size);
            return (void*)((mdl_u8_t*)__blk+blkd_size);
        }
    }

    if (not_null(__blk->next)) {
        if (is_free(next)) {
             
            return p;
        }
    } 
    return NULL;
}

void static*
grow_blk(potp __pot, blkdp __blk, ar_uint_t __size) {

}
*/

void* _ffly_alloc(potp, mdl_uint_t);
void _ffly_free(potp, void*);
/*
struct pot* alloc_pot(mdl_uint_t __size) {
    struct pot *p = _ffly_alloc(&main_pot, sizeof(struct pot));
    init_pot(p);
    p->flags |= FIXED;
    if (__size>ALLOC_THRESH) {
        p->top = mmap(NULL, __size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
        p->flags |= MMAPED;
    } else {
        p->top = _ffly_alloc(&main_pot, __size);
    }
    p->end = (void*)((mdl_u8_t*)p->top+__size);
    return p;
}

void free_pot(struct pot *__pot) {
    if (is_flag(__pot->flags, MMAPED)) {
        munmap(__pot->top, __pot->end-__pot->pot);
    } else {
        _ffly_free(&main_pot, __pot->p);
    }
    _ffly_free(&main_pot, __pot);
}
*/
void* _ffly_alloc(potp __pot, mdl_uint_t __bc) {
    lock_pot(__pot);
    ar_off_t bin;
    if (not_null((bin = get_bin(__pot, __bc)))) {
        while(not_null(bin)) {
            blkdp blk = get_blk(__pot, bin);
            if (blk->size >= __bc) {
                unlink(__pot, blk);
                blk->pad = blk->size-__bc;
                blk->flags = (blk->flags&~BLK_FREE)|BLK_USED;

                ar_uint_t junk;
                if ((junk = (blk->size-__bc)) > blkd_size) {
                    ar_off_t off = blk->off+blkd_size+__bc;
                    blkdp p = (blkdp)((mdl_u8_t*)__pot->end+off); 
                    *p = (struct blkd){
                        .prev = blk->off, .next = blk->next,
                        .size = junk-blkd_size, .off = off, .end = blk->end,
                        .fd = AR_NULL, .bk = AR_NULL,
                        .flags = BLK_FREE
                    };

                    blk->size = __bc;
                    blk->next = p->off;
                    blk->end = off;
                    if (__pot->off == p->end) {
                        __pot->end_blk = p->off;
                    }

                    if (not_null(p->next))
                        get_blk(__pot, p->next)->prev = p->off;
                    _ffly_free(__pot, (void*)((mdl_u8_t*)p+blkd_size));
                }
                unlock_pot(__pot);
                return (void*)((mdl_u8_t*)blk+blkd_size);
            } 
           
            bin = blk->fd;
        }
    }

    mdl_uint_t size = align_to(blkd_size+__bc, ALIGN);
    ar_off_t top = __pot->off+size;
    if (!is_flag(__pot->flags, FIXED)) {
        mdl_uint_t page_c;
        if ((page_c = ((top<<PAGE_SHIFT)+((top-((top<<PAGE_SHIFT)*PAGE_SIZE))>0))) >= __pot->page_c) {
            ffly_brk(__pot->top = (void*)((mdl_u8_t*)__pot->end+((__pot->page_c = page_c)*PAGE_SIZE)));
        }
    }

    blkdp blk = (blkdp)((mdl_u8_t*)__pot->end+__pot->off);
    *blk = (struct blkd) {
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
    unlock_pot(__pot);
    return (void*)((mdl_u8_t*)blk+blkd_size);
}

void* ffly_alloc(mdl_uint_t __bc) {
    if (!inited) {
        inited = 1;
        init_pot(&arena);

        arena.flags |= FIXED;
        arena.end = _ffly_alloc(&main_pot, POT_SIZE);
        arena.top = (mdl_u8_t*)arena.top+POT_SIZE;
        if (main_pot.next != NULL)
            arena.next = main_pot.next;
        main_pot.next = &arena;
    }
    return _ffly_alloc(&arena, __bc);
}

void _ffly_free(potp __pot, void *__p) {
    lock_pot(__pot);
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
        mdl_uint_t page_c;
        if ((page_c = ((__pot->off<<PAGE_SHIFT)+((__pot->off-((__pot->off<<PAGE_SHIFT)*PAGE_SIZE))>0))) < __pot->page_c) {
            ffly_brk(__pot->top = (void*)((mdl_u8_t*)__pot->end+((__pot->page_c = page_c)*PAGE_SIZE)));
        }
        unlock_pot(__pot);
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
    unlock_pot(__pot);
}

void ffly_free(void *__p) {
    if (!__p) return;
    _ffly_free(&arena, __p);
}

void *ffly_realloc(void *__p, mdl_uint_t __bc) {
    blkdp blk = (blkdp)((mdl_u8_t*)__p-blkd_size);
    void *p = ffly_alloc(__bc);

    ar_uint_t size = blk->size-blk->pad;
    ffly_printf("prev size: %u\n", size);
    copy(p, __p, size<__bc?size:__bc);
    ffly_free(__p);
    return p;
}

