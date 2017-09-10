# ifndef __vec__h
# define __vec__h
# define VEC_PAGE_SIZE 32 // dont change
# define VEC_AUTO_RESIZE 0b10000000
# define VEC_ITR_UP 0x0
# define VEC_ITR_DOWN 0x1
# define VEC_BLK_CHAIN 0b01000000
# define VEC_UUU_BLKS 0b00100000
# include <mdlint.h>
# include "io.h"
# include "../types/err_t.h"
# include "../types/bool_t.h"
struct ffly_vec_chain {
	mdl_u8_t state;
	mdl_uint_t above, below;
};

struct ffly_vec {
	mdl_uint_t last_blk, first_blk;
	void *p;
	mdl_u8_t flags;
	mdl_uint_t off, size;
	mdl_uint_t blk_size, page_c;
	struct ffly_vec *uu_blks;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_vec_init(struct ffly_vec*, mdl_uint_t);
ffly_err_t ffly_vec_push_back(struct ffly_vec*, void**);
ffly_err_t ffly_vec_pop_back(struct ffly_vec*, void*);
ffly_err_t ffly_vec_de_init(struct ffly_vec*);
ffly_err_t ffly_vec_resize(struct ffly_vec*, mdl_uint_t);
void ffly_vec_del(struct ffly_vec*, void*);
void ffly_vec_itr(struct ffly_vec*, void**, mdl_u8_t, mdl_uint_t);
void* ffly_vec_rbegin(struct ffly_vec*);
void* ffly_vec_rend(struct ffly_vec*);
void* ffly_vec_first(struct ffly_vec*);
void* ffly_vec_last(struct ffly_vec*);
# ifdef __cplusplus
}
# endif

mdl_uint_t static __inline__ ffly_vec_size(struct ffly_vec *__vec) {return __vec->size;}
void static __inline__* ffly_vec_begin(struct ffly_vec *__vec) {
	return (void*)((mdl_u8_t*)__vec->p+((__vec->flags & VEC_BLK_CHAIN) == VEC_BLK_CHAIN? sizeof(struct ffly_vec_chain):0));
}

void static __inline__* ffly_vec_end(struct ffly_vec *__vec) {
	return (void*)(((mdl_u8_t*)__vec->p-((__vec->flags & VEC_BLK_CHAIN) == VEC_BLK_CHAIN? sizeof(struct ffly_vec_chain):0))+((__vec->off-1)*__vec->blk_size));
}
ffly_bool_t static __inline__ ffly_vec_empty(struct ffly_vec *__vec) {return __vec->off == 0?1:0;}
# ifdef __cplusplus
# include "../data/swp.h"
# include "errno.h"
namespace mdl {
namespace firefly {
namespace system {
static types::err_t(*vec_init)(struct ffly_vec*, uint_t) = &ffly_vec_init;
static types::err_t(*vec_push_back)(struct ffly_vec*, void**) = &ffly_vec_push_back;
static types::err_t(*vec_pop_back)(struct ffly_vec*, void*) = &ffly_vec_pop_back;
static types::err_t(*vec_de_init)(struct ffly_vec*) = &ffly_vec_de_init;
static types::err_t(*vec_resize)(struct ffly_vec*, uint_t) = &ffly_vec_resize;
static void(*vec_del)(struct ffly_vec*, void*) = &ffly_vec_del;
static void*(*vec_first)(struct ffly_vec*) = &ffly_vec_first;
static void*(*vec_last)(struct ffly_vec*) = &ffly_vec_last;
uint_t static __inline__ vec_size(struct ffly_vec *__vec) {return ffly_vec_size(__vec);}
void static __inline__* vec_begin(struct ffly_vec *__vec) {return ffly_vec_begin(__vec);}
void static __inline__* vec_end(struct ffly_vec *__vec) {return ffly_vec_end(__vec);}
types::bool_t static __inline__ vec_empty(struct ffly_vec *__vec) {return ffly_vec_empty(__vec);}
template<typename _T>
struct vec {
	vec() {
		if (this->init(0) != FFLY_SUCCESS)
			io::printf(stderr, "queue: failed to init.\n");}
	vec(mdl_u8_t __flags) {
		if (this->init(__flags) != FFLY_SUCCESS)
			io::printf(stderr, "queue: failed to init.\n");}
	~vec() {
		if (this->de_init() != FFLY_SUCCESS)
			io::printf(stderr, "queue: failed to de_init.\n");}

	types::err_t init(u8_t __flags) {
		types::err_t any_err;
		this->raw_vec.flags = __flags;
		any_err = vec_init(&this->raw_vec, sizeof(_T));
		return any_err;
	}
	types::err_t de_init() {return vec_de_init(&this->raw_vec);}
	uint_t size() {return vec_size(&this->raw_vec);}
	_T* begin() {return static_cast<_T*>(vec_begin(&this->raw_vec));}
	_T* end() {return static_cast<_T*>(vec_end(&this->raw_vec));}
	_T* first() {return static_cast<_T*>(vec_first(&this->raw_vec));}
	_T* last() {return static_cast<_T*>(vec_last(&this->raw_vec));}
	types::bool_t empty() {return vec_empty(&this->raw_vec);}
	_T& operator()(mdl_uint_t __off) {return this->at(__off);}

	void del(_T *__p) {
		vec_del(&this->raw_vec, static_cast<void*>(__p));
	}

	_T& push_back(types::err_t& __any_err) {
		_T *p;
		__any_err = vec_push_back(&this->raw_vec, (void**)&p);
		return *p;
	}

	types::err_t push_back(_T __elem) {
		types::err_t any_err;
		this->push_back(any_err) = __elem;
		return any_err;
	}

	_T pop_back(types::err_t& __any_err) {
		_T ret;
		__any_err = vec_pop_back(&this->raw_vec, (void*)&ret);
		return ret;
	}

	_T& at(uint_t __off) {return *(this->begin()+__off);}
	_T& operator[](uint_t __off){return this->at(__off);}

	void swp(_T *__p1, uint_t __p2) {
		data::swp<_T>(__p1, (void*)(this->begin()+__p2));}

	void swp(uint_t __p1, _T *__p2) {
		data::swp<_T>((void*)(this->begin()+__p1), __p2);}

	void swp(uint_t __p1, uint_t __p2) {
		data::swp<_T>((void*)(this->begin()+__p1), (void*)(this->begin()+__p2));}

	types::err_t resize(uint_t __size) {return vec_resize(&this->raw_vec, __size);}
	types::err_t resize(uint_t __size, u8_t __dir) {
		if (__dir == 1) {return this->resize(this->size()+__size);}
		if (__dir == -1) {return this->resize(this->size()-__size);}}
	struct ffly_vec raw_vec;
};

}
}
}
# endif
# endif /*__vec__h*/
