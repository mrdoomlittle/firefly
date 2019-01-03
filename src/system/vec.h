# ifndef __ffly__vec__h
# define __ffly__vec__h
#define VEC_PAGE_SHIFT 5
#define VEC_PAGE_SIZE (1<<VEC_PAGE_SHIFT) // dont change

/*
	if push and under size then will auto resize
*/
#define VEC_AUTO_RESIZE 0x01
#define VEC_ITR_FD 0x00
#define VEC_ITR_BK 0x01

/*
	blocks have header that are linked
*/
#define VEC_BLK_CHAIN 0x02

/*
	VEC_BLK_CHAIN <- must be specified
	blocks get reused
*/
#define VEC_UUU_BLKS 0x04

/*
	use of pages
*/
#define VEC_NONCONTINUOUS 0x08
#define VEC_AS 0x10
# include "../ffint.h"
# include "../types.h"
# include "io.h"
# include "flags.h"
# include "err.h"
#define FF_VEC struct ffly_vec
#define ff_vec struct ffly_vec
#define ___ffly_vec_nonempty(__vec) if (ffly_vec_nonempty(__vec))
#define vec_prev(__vec) ((__vec)->prev)
#define vec_next(__vec) ((__vec)->next)

#define vec_at_deadstop(__p, __end) ((void*)__p>(void*)__end)
#define vec_deadstop(__p, __vec) ((void*)__p>ffly_vec_end(__vec))
#define ffly_vec_beg ffly_vec_begin

#define FF_VEC_SA

/*
    needs testing - 'VEC_NONCONTINUOUS'
	TODO:
	rename begin to just 'beg' e.g. ffly_vec_beg();
*/
struct ffly_vec_blkd {
	ff_flag_t flags;
    ff_u16_t page; // page no
    ff_u8_t off; // offset from page in blocks
	ff_off_t prev, next;
};

typedef struct ffly_vec_blkd* ffly_vec_blkdp;
typedef struct ffly_vec {
	// todo re'move (move)
#ifdef FF_VEC_SA
	void*(*alloc)(long long, ff_uint_t);
	void(*free)(long long, void*);
	void*(*realloc)(long long, void*, ff_uint_t);
	long long arg;
#endif
	ff_off_t top, end;
	void *p;
	ff_flag_t flags;

	/*
		where next block is to be if pushed
	*/
	ff_off_t off;
	/*
		N *blocks
	*/
	ff_size_t size;
	/*
		size of block in full {with header}
	*/
	ff_size_t blk_size;
	ff_uint_t page_c;
	struct ffly_vec *uu_blks;
    struct ffly_vec *prev, *next;
} *ffly_vecp;

# ifdef __cplusplus
extern "C" {
# endif
// de-init and then free
ff_err_t ffly_vec_destroy(ffly_vecp);
// detach and {free - not the same as de-init}
ff_err_t ffly_vec_free(ffly_vecp);
// attach/detach vector to/from list
void ffly_vec_attach(ffly_vecp);
void ffly_vec_detach(ffly_vecp);
void ffly_vec_fd(ffly_vecp, void**);
void ffly_vec_bk(ffly_vecp, void**);
// allocate and then attach and init
ffly_vecp ffly_vec_creat(ff_size_t, ff_flag_t, ff_err_t*);
ffly_vecp ffly_vec(ff_size_t, ff_flag_t, ff_err_t*);
ffly_vecp ffly_vec_list(); //get list of all vectors
ff_err_t ffly_vec_init(ffly_vecp, ff_size_t);
ff_err_t ffly_vec_push_back(ffly_vecp, void**);
ff_err_t ffly_vec_pop_back(ffly_vecp, void*);
ff_err_t ffly_vec_de_init(ffly_vecp);
ff_err_t ffly_vec_resize(ffly_vecp, ff_size_t);
void* ffly_vec_at(ffly_vecp, ff_uint_t);
void ffly_vec_del(ffly_vecp, void*);
void ffly_vec_itr(ffly_vecp, void**, ff_u8_t, ff_uint_t);
void* ffly_vec_rbegin(ffly_vecp);
void* ffly_vec_rend(ffly_vecp);
void* ffly_vec_first(ffly_vecp);
void* ffly_vec_last(ffly_vecp);
void* ffly_vec_begin(ffly_vecp);
void* ffly_vec_end(ffly_vecp);
ff_off_t ffly_vec_off(ffly_vecp, void*);
ff_uint_t ffly_vec_blk_off(ffly_vecp, void*);
# ifdef __cplusplus
}
# endif
void static __inline__* ffly_vec_p(struct ffly_vec *__vec){return __vec->p;}
ff_size_t static __inline__ ffly_vec_size(struct ffly_vec *__vec) {return __vec->size;}
ff_bool_t static __inline__ ffly_vec_empty(struct ffly_vec *__vec) {return !__vec->size;}
ff_bool_t static __inline__ ffly_vec_nonempty(struct ffly_vec *__vec) {return __vec->size>0;}
void static __inline__ ffly_vec_tog_flag(struct ffly_vec *__vec, ff_flag_t __flag) {ffly_add_flag(__vec->flags, __flag);}
void static __inline__ ffly_vec_clear_flags(struct ffly_vec *__vec){__vec->flags = 0x0;} // remove
void static __inline__ ffly_vec_clr_flags(struct ffly_vec *__vec){__vec->flags = 0x0;}
void static __inline__ ffly_vec_set_flags(struct ffly_vec *__vec, ff_flag_t __flags) {__vec->flags = __flags;}
# ifdef __cplusplus
# include "../data/mem_swp.h"
# include "errno.h"
namespace ff {
static err_t(*vec_init)(ffly_vecp, size_t) = &ffly_vec_init;
static err_t(*vec_push_back)(ffly_vecp, void**) = &ffly_vec_push_back;
static err_t(*vec_pop_back)(ffly_vecp, void*) = &ffly_vec_pop_back;
static err_t(*vec_de_init)(ffly_vecp) = &ffly_vec_de_init;
static err_t(*vec_resize)(ffly_vecp, size_t) = &ffly_vec_resize;
static void(*vec_del)(ffly_vecp, void*) = &ffly_vec_del;
static void*(*vec_first)(ffly_vecp) = &ffly_vec_first;
static void*(*vec_last)(ffly_vecp) = &ffly_vec_last;
size_t static __inline__ vec_size(struct ffly_vec *__vec) {return ffly_vec_size(__vec);}
void static __inline__* vec_begin(struct ffly_vec *__vec) {return ffly_vec_begin(__vec);}
void static __inline__* vec_end(struct ffly_vec *__vec) {return ffly_vec_end(__vec);}
bool_t static __inline__ vec_empty(struct ffly_vec *__vec) {return ffly_vec_empty(__vec);}
template<typename _T>
struct vec {
    typedef _T* iterator;
	vec() {
		if (_err(this->init(0)))
			fprintf(ffly_err, "vec: failed to init.\n");}
	vec(ff_u8_t __flags) {
		if (_err(this->init(__flags)))
			fprintf(ffly_err, "vec: failed to init.\n");}
	~vec() {
		if (_err(this->de_init()))
			fprintf(ffly_err, "vec: failed to de_init.\n");}

	err_t init(u8_t __flags) {
		err_t err;
		ffly_vec_set_flags(&this->raw, __flags);
		err = vec_init(&this->raw, sizeof(_T));
		printf("vector init.\n");
		return err;
	}

	err_t de_init() {
		printf("vector de-init.\n");
		return vec_de_init(&this->raw);
	}
	size_t size() {return vec_size(&this->raw);}
	_T* begin() {return static_cast<_T*>(vec_begin(&this->raw));}
	_T* end() {return static_cast<_T*>(vec_end(&this->raw));}
	_T* first() {return static_cast<_T*>(vec_first(&this->raw));}
	_T* last() {return static_cast<_T*>(vec_last(&this->raw));}
	bool_t empty() {return vec_empty(&this->raw);}
	_T& operator()(ff_uint_t __off) {return this->at(__off);}

	err_t del(_T *__p) {
		vec_del(&this->raw, static_cast<void*>(__p));
		return FFLY_SUCCESS;
	}

	_T& push_back(err_t& __err) {
		_T *p;
		__err = vec_push_back(&this->raw, (void**)&p);
		return *p;
	}

	err_t push_back(_T __elem) {
		err_t err;
		this->push_back(err) = __elem;
		return err;
	}

	_T pop_back(err_t& __err) {
		_T ret;
		__err = vec_pop_back(&this->raw, (void*)&ret);
		return ret;
	}

	off_t off(void *__p) {return ffly_vec_off(&this->raw, __p);}
	uint_t blk_off(void *__p){return ffly_vec_blk_off(&this->raw, __p);}
	_T& at_off(off_t __off) {return *reinterpret_cast<_T*>(static_cast<u8_t*>(ffly_vec_p(&this->raw))+__off);}
	_T& at(uint_t __off) {return *static_cast<_T*>(ffly_vec_at(&this->raw, __off));}
	_T& operator[](uint_t __off){return this->at(__off);}

	void swp(_T *__p1, uint_t __p2) {
		mem_swp<_T>(__p1, ffly_vec_at(&this->raw, __p2));}

	void swp(uint_t __p1, _T *__p2) {
		mem_swp<_T>(ffly_vec_at(&this->raw, __p1), __p2);}

	void swp(uint_t __p1, uint_t __p2) {
		mem_swp<_T>(ffly_vec_at(&this->raw, __p1), ffly_vec_at(&this->raw, __p2));}

	err_t resize(ff::size_t __size) {return vec_resize(&this->raw, __size);}
	err_t resize(ff::size_t __size, i8_t __dir) {
		if (__dir == 1) {return this->resize(this->size()+__size);}
		if (__dir == -1) {return this->resize(this->size()-__size);}}
	struct ffly_vec raw;
};
}
# endif
# endif /*__ffly__vec__h*/
