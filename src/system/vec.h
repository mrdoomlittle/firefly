# ifndef __vec__h
# define __vec__h
# define VEC_PAGE_SIZE 32 // dont change
# define VEC_AUTO_RESIZE 0x1
# include <eint_t.h>
# include "io.h"
# include "../types/err_t.h"
struct ffly_vec {
	void *p;
	mdl_u8_t flags;
	mdl_uint_t off, size;
	mdl_uint_t blk_size, page_c;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_vec_init(struct ffly_vec*, mdl_uint_t);
ffly_err_t ffly_vec_push_back(struct ffly_vec*, void**);
ffly_err_t ffly_vec_pop_back(struct ffly_vec*, void*);
ffly_err_t ffly_vec_de_init(struct ffly_vec*);
ffly_err_t ffly_vec_resize(struct ffly_vec*, mdl_uint_t);
# ifdef __cplusplus
}
# endif

mdl_uint_t static __inline__ ffly_vec_size(struct ffly_vec *__vec) {return __vec->size;}
void static __inline__* ffly_vec_begin(struct ffly_vec *__vec) {return __vec->p;}
void static __inline__* ffly_vec_end(struct ffly_vec *__vec) {return (mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size);}
mdl_u8_t static __inline__ ffly_vec_empty(struct ffly_vec *__vec) {return __vec->off == 0?1:0;}
# ifdef __cplusplus
# include "../data/swp.h"
namespace mdl {
namespace firefly {
namespace system {
static types::err_t (*vec_init)(struct ffly_vec*, uint_t) = &ffly_vec_init;
static types::err_t (*vec_push_back)(struct ffly_vec*, void**) = &ffly_vec_push_back;
static types::err_t (*vec_pop_back)(struct ffly_vec*, void*) = &ffly_vec_pop_back;
static types::err_t (*vec_de_init)(struct ffly_vec*) = &ffly_vec_de_init;
static types::err_t (*vec_resize)(struct ffly_vec*, uint_t) = &ffly_vec_resize;
uint_t static __inline__ vec_size(struct ffly_vec *__vec) {return ffly_vec_size(__vec);}
void static __inline__* vec_begin(struct ffly_vec *__vec) {return ffly_vec_begin(__vec);}
void static __inline__* vec_end(struct ffly_vec *__vec) {return ffly_vec_end(__vec);}
bool static __inline__ vec_empty(struct ffly_vec *__vec) {return ffly_vec_empty(__vec);}
template<typename _T>
struct vec {
	vec() {
		if (this->init(0) != FFLY_SUCCESS)
			io::printf(stderr, "queue: failed to ini.\n");}
	vec(mdl_u8_t __flags) {
		if (this->init(__flags) != FFLY_SUCCESS)
			io::printf(stderr, "queue: failed to ini.\n");}
	~vec() {
		if (this->de_init() != FFLY_SUCCESS)
			io::printf(stderr, "queue: failed to de_init.\n");}

	types::err_t init(u8_t __flags) {
		types::err_t any_err;
		any_err = vec_init(&this->raw_vec, sizeof(_T));
		this->raw_vec.flags = __flags;
		return any_err;
	}
	types::err_t de_init() {return vec_de_init(&this->raw_vec);}
	uint_t size() {vec_size(&this->raw_vec);}
	_T* begin() {static_cast<_T*>(vec_begin(&this->raw_vec));}
	_T* end() {static_cast<_T*>(vec_end(&this->raw_vec));}
	bool empty() {return vec_empty(&this->raw_vec);}

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
