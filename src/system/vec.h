# ifndef __vec__h
# define __vec__h
# define VEC_PAGE_SIZE 32
# define VEC_AUTO_RESIZE 0x1
# include <eint_t.h>
# include "../types/err_t.h"
struct vec {
	void *p;
	mdl_u8_t flags;
	mdl_uint_t off;
	mdl_uint_t blk_size, page_c;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_vec_init(struct vec*, mdl_uint_t);
ffly_err_t ffly_vec_push_back(struct vec*, void **);
ffly_err_t ffly_vec_pop_back(struct vec*, void*);
ffly_err_t ffly_vec_de_init(struct vec*);
ffly_err_t ffly_vec_resize(struct vec*, mdl_uint_t);
# ifdef __cplusplus
}
# endif

mdl_uint_t static __inline__ ffly_vec_size(struct vec *__vec) {return __vec->off;}
void static __inline__* ffly_vec_begin(struct vec *__vec) {return __vec->p;}
void static __inline__* ffly_vec_end(struct vec *__vec) {return (mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size);}
mdl_u8_t static __inline__ ffly_vec_empty(struct vec *__vec) {return __vec->off == 0?1:0;}
void static __inline__ ffly_vec_swp(struct vec *__vec) {

}

# ifdef __cplusplus
# include "../data/swp.h"
namespace mdl {
namespace firefly {
namespace system {
static types::err_t (*vec_init)(struct vec*, mdl_uint_t) = &ffly_vec_init;
static types::err_t (*vec_push_back)(struct vec*, void **) = &ffly_vec_push_back;
static types::err_t (*vec_pop_back)(struct vec*, void*) = &ffly_vec_pop_back;
static types::err_t (*vec_de_init)(struct vec*) = &ffly_vec_de_init;
static types::err_t (*vec_resize)(struct vec*, mdl_uint_t) = &ffly_vec_resize;
uint_t static __inline__ vec_size(struct vec *__vec) {return ffly_vec_size(__vec);}
void static __inline__* vec_begin(struct vec *__vec) {ffly_vec_begin(__vec);}
void static __inline__* vec_end(struct vec *__vec) {ffly_vec_end(__vec);}
bool static __inline__ vec_empty(struct vec *__vec) {return ffly_vec_empty(__vec);}
template<typename _T>
struct vec {
	vec() {this->init(0);}
	vec(mdl_u8_t __flags) {this->init(__flags);}
	void init(u8_t __flags) {vec_init(&this->_vec, sizeof(_T)); this->_vec.flags = __flags;}
	~vec() {vec_de_init(&this->_vec);}
	uint_t size() {vec_size(&this->_vec);}
	_T* begin() {static_cast<_T*>(vec_begin(&this->_vec));}
	_T* end() {static_cast<_T*>(vec_end(&this->_vec));}
	_T& operator[](uint_t __off) {return *(begin()+__off);}
	bool empty() {return vec_empty(&this->_vec);}

	_T& push_back() {
		_T *ptr;
		vec_push_back(&this->_vec, (void**)&ptr);
		return *ptr;
	}

	void push_back(_T __element) {
		this->push_back() = __element;}

	_T pop_back() {
		_T ret;
		vec_pop_back(&this->_vec, (void*)&ret);
		return ret;
	}

	void swp(_T *__p1, uint_t __p2) {
		data::swp(__p1, (void*)(this->begin()+__p2), sizeof(_T));}

	void swp(uint_t __p1, _T *__p2) {
		data::swp((void*)(this->begin()+__p1), __p2, sizeof(_T));}

	void swp(uint_t __p1, uint_t __p2) {
		data::swp((void*)(this->begin()+__p1), (void*)(this->begin()+__p2), sizeof(_T));}

	void resize(uint_t __size) {vec_resize(&this->_vec, __size);}
	void resize(uint_t __size, u8_t __dir) {
		if (__dir == 1) {this->resize(this->size()+__size);return;}
		if (__dir == -1) {this->resize(this->size()-__size);return;}
	}
	struct ::vec _vec;
};

}
}
}
# endif
# endif /*__vec__h*/
