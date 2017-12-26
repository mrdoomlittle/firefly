# ifndef __ffly__vec__h
# define __ffly__vec__h
# define VEC_PAGE_SIZE 32 // dont change
# define VEC_AUTO_RESIZE 0x1
# define VEC_ITR_FD 0x0
# define VEC_ITR_BK 0x1
# define VEC_BLK_CHAIN 0x2
# define VEC_UUU_BLKS 0x4
# include <mdlint.h>
# include "io.h"
# include "../types/err_t.h"
# include "../types/bool_t.h"
# include "flags.h"
# include "err.h"
# include "../types/off_t.h"
# include "../types/size_t.h"
struct ffly_vec_blkd {
	ffly_flag_t flags;
	ffly_off_t prev, next;
};

struct ffly_vec {
	ffly_off_t top, end;
	void *p;
	ffly_flag_t flags;
	ffly_off_t off;
	ffly_size_t size, blk_size;
	mdl_uint_t page_c;
	struct ffly_vec *uu_blks;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_vec_init(struct ffly_vec*, ffly_size_t);
ffly_err_t ffly_vec_push_back(struct ffly_vec*, void**);
ffly_err_t ffly_vec_pop_back(struct ffly_vec*, void*);
ffly_err_t ffly_vec_de_init(struct ffly_vec*);
ffly_err_t ffly_vec_resize(struct ffly_vec*, ffly_size_t);
void* ffly_vec_at(struct ffly_vec*, mdl_uint_t);
void ffly_vec_del(struct ffly_vec*, void*);
void ffly_vec_itr(struct ffly_vec*, void**, mdl_u8_t, mdl_uint_t);
void* ffly_vec_rbegin(struct ffly_vec*);
void* ffly_vec_rend(struct ffly_vec*);
void* ffly_vec_first(struct ffly_vec*);
void* ffly_vec_last(struct ffly_vec*);
void* ffly_vec_begin(struct ffly_vec*);
void* ffly_vec_end(struct ffly_vec*);
ffly_off_t ffly_vec_off(struct ffly_vec*, void*);
mdl_uint_t ffly_vec_blk_off(struct ffly_vec*, void*);
# ifdef __cplusplus
}
# endif
void static __inline__* ffly_vec_p(struct ffly_vec *__vec){return __vec->p;}
ffly_size_t static __inline__ ffly_vec_size(struct ffly_vec *__vec) {return __vec->size;}
ffly_bool_t static __inline__ ffly_vec_empty(struct ffly_vec *__vec) {return !__vec->size;}
void static __inline__ ffly_vec_tog_flag(struct ffly_vec *__vec, ffly_flag_t __flag) {ffly_add_flag(&__vec->flags, __flag, 0);}
void static __inline__ ffly_vec_clear_flags(struct ffly_vec *__vec){__vec->flags = 0x0;}
void static __inline__ ffly_vec_set_flags(struct ffly_vec *__vec, ffly_flag_t __flags) {__vec->flags = __flags;}
# ifdef __cplusplus
# include "../data/mem_swp.h"
# include "errno.h"
namespace mdl {
namespace firefly {
namespace system {
static types::err_t(*vec_init)(struct ffly_vec*, types::size_t) = &ffly_vec_init;
static types::err_t(*vec_push_back)(struct ffly_vec*, void**) = &ffly_vec_push_back;
static types::err_t(*vec_pop_back)(struct ffly_vec*, void*) = &ffly_vec_pop_back;
static types::err_t(*vec_de_init)(struct ffly_vec*) = &ffly_vec_de_init;
static types::err_t(*vec_resize)(struct ffly_vec*, types::size_t) = &ffly_vec_resize;
static void(*vec_del)(struct ffly_vec*, void*) = &ffly_vec_del;
static void*(*vec_first)(struct ffly_vec*) = &ffly_vec_first;
static void*(*vec_last)(struct ffly_vec*) = &ffly_vec_last;
types::size_t static __inline__ vec_size(struct ffly_vec *__vec) {return ffly_vec_size(__vec);}
void static __inline__* vec_begin(struct ffly_vec *__vec) {return ffly_vec_begin(__vec);}
void static __inline__* vec_end(struct ffly_vec *__vec) {return ffly_vec_end(__vec);}
types::bool_t static __inline__ vec_empty(struct ffly_vec *__vec) {return ffly_vec_empty(__vec);}
template<typename _T>
struct vec {
	vec() {
		if (_err(this->init(0)))
			io::fprintf(ffly_err, "vec: failed to init.\n");}
	vec(mdl_u8_t __flags) {
		if (_err(this->init(__flags)))
			io::fprintf(ffly_err, "vec: failed to init.\n");}
	~vec() {
		if (_err(this->de_init()))
			io::fprintf(ffly_err, "vec: failed to de_init.\n");}

	types::err_t init(u8_t __flags) {
		types::err_t err;
		ffly_vec_set_flags(&this->raw, __flags);
		err = vec_init(&this->raw, sizeof(_T));
		return err;
	}

	types::err_t de_init() {return vec_de_init(&this->raw);}
	types::size_t size() {return vec_size(&this->raw);}
	_T* begin() {return static_cast<_T*>(vec_begin(&this->raw));}
	_T* end() {return static_cast<_T*>(vec_end(&this->raw));}
	_T* first() {return static_cast<_T*>(vec_first(&this->raw));}
	_T* last() {return static_cast<_T*>(vec_last(&this->raw));}
	types::bool_t empty() {return vec_empty(&this->raw);}
	_T& operator()(mdl_uint_t __off) {return this->at(__off);}

	types::err_t del(_T *__p) {
		vec_del(&this->raw, static_cast<void*>(__p));
		return FFLY_SUCCESS;
	}

	_T& push_back(types::err_t& __err) {
		_T *p;
		__err = vec_push_back(&this->raw, (void**)&p);
		return *p;
	}

	types::err_t push_back(_T __elem) {
		types::err_t err;
		this->push_back(err) = __elem;
		return err;
	}

	_T pop_back(types::err_t& __err) {
		_T ret;
		__err = vec_pop_back(&this->raw, (void*)&ret);
		return ret;
	}

	types::off_t off(void *__p) {return ffly_vec_off(&this->raw, __p);}
	uint_t blk_off(void *__p){return ffly_vec_blk_off(&this->raw, __p);}
	_T& at_off(types::off_t __off) {return *reinterpret_cast<_T*>(static_cast<u8_t*>(ffly_vec_p(&this->raw))+__off);}
	_T& at(uint_t __off) {return *static_cast<_T*>(ffly_vec_at(&this->raw, __off));}
	_T& operator[](uint_t __off){return this->at(__off);}

	void swp(_T *__p1, uint_t __p2) {
		data::mem_swp<_T>(__p1, ffly_vec_at(&this->raw, __p2));}

	void swp(uint_t __p1, _T *__p2) {
		data::mem_swp<_T>(ffly_vec_at(&this->raw, __p1), __p2);}

	void swp(uint_t __p1, uint_t __p2) {
		data::mem_swp<_T>(ffly_vec_at(&this->raw, __p1), ffly_vec_at(&this->raw, __p2));}

	types::err_t resize(types::size_t __size) {return vec_resize(&this->raw, __size);}
	types::err_t resize(types::size_t __size, i8_t __dir) {
		if (__dir == 1) {return this->resize(this->size()+__size);}
		if (__dir == -1) {return this->resize(this->size()-__size);}}
	struct ffly_vec raw;
};

}
}
}
# endif
# endif /*__ffly__vec__h*/
