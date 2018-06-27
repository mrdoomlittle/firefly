# ifndef __ffly__allocr__h
# define __ffly__allocr__h
# include "../ffint.h"
# include "../types.h"
typedef void const* ffly_potp;

struct arinfo {
	ff_uint_t block_c;
	ff_uint_t used;
};

enum {
	_ar_unset,
	_ar_setpot,
	_ar_getpot
};

void arinfo(struct arinfo*);
extern void(*ar_erase)(void*, ff_uint_t);
// rename
void ffly_process_prep(void);
ff_err_t ffly_ar_init(void);
ff_err_t ffly_ar_cleanup(void);
void ffly_arctl(ff_u8_t, ff_u64_t);
void ffly_arhang(void);
void ffly_arstat(void);
void* ffly_balloc(ff_uint_t);
void ffly_bfree(void*);
void* ffly_brealloc(void*, ff_uint_t);

/*should only be used for trimming small amount off*/
// non user functions or to be used before using
void* ffly_arsh(void*, ff_uint_t);
void* ffly_argr(void*, ff_uint_t);
void ffly_arbl(void*);
# endif /*__ffly__allocr__h*/
