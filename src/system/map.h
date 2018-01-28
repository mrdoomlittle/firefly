# ifndef __ffly__map__h
# define __ffly__map__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "vec.h"
# define MAP_ITR_FD 0x1
# define MAP_ITR_BK 0x1
struct ffly_map {
	struct ffly_vec **table;
    void *begin, *end;
    mdl_uint_t size;
    struct ffly_map *parent;
};

/*
    TODO: add ffly_map_del
*/

enum {
    _ffly_map_15 = 4,
    _ffly_map_31,
    _ffly_map_63,
    _ffly_map_127,
    _ffly_map_255,
    _ffly_map_511,
    _ffly_map_1023,
    _ffly_map_2047,
    _ffly_map_4095,
    _ffly_map_8191
};

typedef struct ffly_map* ffly_mapp;
# ifdef __cplusplus
extern "C" {
# endif
ffly_mapp ffly_map(mdl_uint_t);
void ffly_map_free(ffly_mapp);
void ffly_map_destroy(ffly_mapp);
void ffly_map_parent(ffly_mapp, ffly_mapp);
ffly_err_t ffly_map_init(ffly_mapp, mdl_uint_t);
ffly_err_t ffly_map_put(ffly_mapp, mdl_u8_t const*, mdl_uint_t, void const*);
void const* ffly_map_get(ffly_mapp, mdl_u8_t const*, mdl_uint_t, ffly_err_t*);
ffly_err_t ffly_map_de_init(ffly_mapp);
void* ffly_map_begin(ffly_mapp);
void* ffly_map_end(ffly_mapp);
void ffly_map_itr(ffly_mapp, void**, mdl_u8_t);
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

	_T get(char const* __s, types::err_t& __err) {
		return this->get(reinterpret_cast<u8_t const*>(__s), data::str_len(__s), &__err);}

	_T get(u8_t const *__key, uint_t __bc, types::err_t& __err) {
		return *static_cast<_T*>(const_cast<void*>(ffly_map_get(&this->raw, __key, __bc, &__err)));}

	types::err_t init() {
		return ffly_map_init(&this->raw);}
	types::err_t de_init() {
        void *itr = ffly_map_begin(&this->raw);
        while(itr != nullptr) {
            void *p = ffly_map_getp(itr);
            if (p != nullptr)
                __ffly_mem_free(p);
            ffly_map_itr(&this->raw, &itr, MAP_ITR_FD);
        }

		return ffly_map_de_init(&this->raw);
    }
	struct ffly_map raw;
};
}
}
}
# endif
# endif /*__ffly__map__h*/
