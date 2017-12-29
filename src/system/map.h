# ifndef __ffly__map__h
# define __ffly__map__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "vec.h"
# define FFLY_MAP_SIZE 0xFF
# define MAP_ITR_FD 0x1
# define MAP_ITR_BK 0x1
struct ffly_map {
	struct ffly_vec **table;
    void *begin, *end;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_map_init(struct ffly_map*);
ffly_err_t ffly_map_put(struct ffly_map*, mdl_u8_t const*, mdl_uint_t, void const*);
void const* ffly_map_get(struct ffly_map*, mdl_u8_t const*, mdl_uint_t);
ffly_err_t ffly_map_de_init(struct ffly_map*);
void* ffly_map_begin(struct ffly_map*);
void* ffly_map_end(struct ffly_map*);
void ffly_map_itr(struct ffly_map*, void**, mdl_u8_t);
void const* ffly_map_getp(void*);
# ifdef __cplusplus
}
# include "../memory/mem_alloc.h"
# include "../data/str_len.h"
namespace mdl {
namespace firefly {
namespace system {
template<typename _T>
struct map {
	map() {this->init();}
	~map() {this->de_init();}
	types::err_t put(_T __v, char const *__s) {
		this->put(__v, reinterpret_cast<u8_t const*>(__s), data::str_len(__s));}

	types::err_t put(_T __v, u8_t const *__key, uint_t __bc) {
		_T *p;
		*(p = static_cast<_T*>(memory::mem_alloc(sizeof(_T)))) = __v;
		ffly_map_put(&this->raw, __key, __bc, reinterpret_cast<void const*>(p));
	}

	_T get(char const* __s) {
		return this->get(reinterpret_cast<u8_t const*>(__s), data::str_len(__s));}

	_T get(u8_t const *__key, uint_t __bc) {
		return *static_cast<_T*>(const_cast<void*>(ffly_map_get(&this->raw, __key, __bc)));}

	types::err_t init() {
		return ffly_map_init(&this->raw);}
	types::err_t de_init() {
		return ffly_map_de_init(&this->raw);}
	struct ffly_map raw;
};
}
}
}
# endif
# endif /*__ffly__map__h*/
