# ifndef __ffly__allocr__h
# define __ffly__allocr__h
# include "../ffint.h"
# include "../types.h"
/*
	for system/thread.c
	this might be removed later but for now in thread.c
	the thread data is stored in the main process memory 
	until we are finished here.
*/
typedef void const* ffly_potp;

struct ff_ar_info {
	ff_uint_t block_c;
	ff_u64_t dead, used, buried;
};

enum {
// unset pot - reset
	_ar_unset,
// set pot or current process
	_ar_setpot,
// get current pot for process
	_ar_getpot
};

void ff_ar_info(struct ff_ar_info*);
extern void(*ar_erase)(void*, ff_uint_t);
// rename
/*
	tls, etc
*/
void ffly_process_prep(void);
ff_err_t ffly_ar_init(void);
ff_err_t ffly_ar_cleanup(void);
void ffly_arctl(ff_u8_t, ff_u64_t);

/*
	hang pot/subpots | kill them off
*/
void ffly_arhang(void);
void ffly_arstat(void);

# define ffly_alloc(__size) \
	ffly_balloc(__size)
# define ffly_free(__p) \
	ffly_bfree(__p)
# define ffly_realloc(__oldp, __nsz) \
	ffly_brealloc(__oldp, __nsz)

// allocate block
void* ffly_balloc(ff_uint_t);

// free block
void ffly_bfree(void*);

// realloc block
void* ffly_brealloc(void*, ff_uint_t);

/*
	TODO:
		rename arsh argr - why because there for getting rid of small amount
		of space no larger then 16 i think?
*/

/*should only be used for trimming small amount off*/
// non user functions or to be used before using
void* ffly_arsh(void*, ff_uint_t);
void* ffly_argr(void*, ff_uint_t);

/*
	rename, used it for debugging
*/
void ffly_arbl(void*);
# endif /*__ffly__allocr__h*/
