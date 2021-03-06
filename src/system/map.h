# ifndef __ffly__map__h
# define __ffly__map__h
# include "../ffint.h"
# include "../types.h"
//#define FF_MAP_SA
#define MAP_AS 0x01
#define MAP_ITR_FD	0x00
#define MAP_ITR_BK	0x01
#define FF_MAP struct ffly_map
#define ff_map struct ffly_map
#define FF_MGF_MA	0x00
#define FF_MGF_MF	0x01
#define FF_MGF_MR	0x02
struct ffly_map_maf {
	void*(*alloc)(long long, ff_uint_t);
	void(*free)(long long, void*);
	void*(*realloc)(long long, void*, ff_uint_t);
};

typedef struct ffly_map {
	struct ffly_map_maf maf;
	long long ma_arg;
	void **table;
    void *begin, *end;
    ff_uint_t size;
    struct ffly_map *parent;
} *ffly_mapp;

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

# ifdef __cplusplus
extern "C" {
# endif
#define ffly_map_creat(__size)\
	_ffly_map_creat(__size, NULL)
#define ffly_map(__size)\
	_ffly_map(__size, NULL)
#define ffly_map_init(__map, __size)\
	_ffly_map_init(__map, __size, NULL)
ffly_mapp _ffly_map_creat(ff_uint_t, void(*)(long long, ff_u8_t));
ffly_mapp _ffly_map(ff_uint_t, void(*)(long long, ff_u8_t));
void ffly_map_free(ffly_mapp);
void ffly_map_destroy(ffly_mapp);
void ffly_map_parent(ffly_mapp, ffly_mapp);
ff_err_t _ffly_map_init(ffly_mapp, ff_uint_t, void(*)(long long, ff_u8_t));
void ffly_map_del(ffly_mapp, void const*);
ff_err_t ffly_map_put(ffly_mapp, ff_u8_t const*, ff_uint_t, void *const);
void *const ffly_map_get(ffly_mapp, ff_u8_t const*, ff_uint_t, ff_err_t*);
ff_err_t ffly_map_de_init(ffly_mapp);
void const* ffly_map_begin(ffly_mapp);
void const* ffly_map_beg(ffly_mapp);
void const* ffly_map_end(ffly_mapp);
void ffly_map_fd(ffly_mapp, void const**);
void ffly_map_bk(ffly_mapp, void const**);
void ffly_map_itr(ffly_mapp, void const**, ff_u8_t);
// fetch entry
void const* ffly_map_fetch(ffly_mapp, ff_u8_t const*, ff_uint_t);
void *const ffly_map_getp(void const*);
# ifdef __cplusplus
}
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../data/str_len.h"
namespace mdl {
namespace firefly {
namespace system {
template<typename _T>
struct map {
	map() {this->init();}
	~map() {this->de_init();}
	ff:err_t put(_T __v, char const *__s) {
		this->put(__v, reinterpret_cast<u8_t const*>(__s), data::str_len(__s));}

	ff:err_t put(_T __v, u8_t const *__key, uint_t __bc) {
		_T *p;
		*(p = static_cast<_T*>(memory::mem_alloc(sizeof(_T)))) = __v;
		ffly_map_put(&this->raw, __key, __bc, reinterpret_cast<void const*>(p));
	}

	_T get(char const *__s, ff:err_t& __err) {
		return this->get(reinterpret_cast<u8_t const*>(__s), data::str_len(__s), __err);}

	_T get(u8_t const *__key, uint_t __bc, ff:err_t& __err) {
		return *static_cast<_T*>(const_cast<void*>(ffly_map_get(&this->raw, __key, __bc, &__err)));}

	void del(char const *__s, ff:err_t& __err) {
		this->del(reinterpret_cast<u8_t const*>(__s), data::str_len(__s), __err);}

	void del(u8_t const *__key, uint_t __bc, ff:err_t& __err) {
		ffly_map_del(&this->raw, ffly_map_fetch(&this->raw, __key, __bc));}

	ff:err_t init() {
		return ffly_map_init(&this->raw, _ffly_map_127);}
	ff:err_t de_init() {
        void *itr = ffly_map_begin(&this->raw);
        while(itr != nullptr) {
            void *const p = ffly_map_getp(itr);
            if (p != nullptr)
             	memory::mem_free((void*)p);
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
