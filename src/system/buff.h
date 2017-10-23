# ifndef __buff__h
# define __buff__h
# include "../types/err_t.h"
# include "io.h"
# include <mdlint.h>
# include "mutex.h"
# include "../types/bool_t.h"

struct ffly_buff {
	ffly_mutex_t m;
	mdl_u8_t *p;
	mdl_uint_t off;
	mdl_uint_t rs_blk_c;
	mdl_uint_t blk_c, blk_size;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_buff_init(struct ffly_buff*, mdl_uint_t, mdl_uint_t);
ffly_err_t ffly_buff_put(struct ffly_buff*, void*);
ffly_err_t ffly_buff_get(struct ffly_buff*, void*);
ffly_err_t ffly_buff_resize(struct ffly_buff*, mdl_uint_t);
ffly_err_t ffly_buff_de_init(struct ffly_buff*);
ffly_err_t ffly_buff_incr(struct ffly_buff*);
ffly_err_t ffly_buff_decr(struct ffly_buff*);
ffly_err_t ffly_buff_reset(struct ffly_buff*);
# ifdef __cplusplus
}
# endif
mdl_uint_t static __inline__ ffly_buff_ublk_c(struct ffly_buff *__buff){return __buff->off;}
void static __inline__ ffly_buff_off_reset(struct ffly_buff *__buff) {__buff->off = 0;}
void static __inline__* ffly_buff_begin(struct ffly_buff *__buff) {return (void*)__buff->p;}
void static __inline__* ffly_buff_end(struct ffly_buff *__buff) {return (void*)(__buff->p+((__buff->off-1)*__buff->blk_size));}
void static __inline__ ffly_buff_lock(struct ffly_buff *__buff) {ffly_mutex_lock(&__buff->m);}
void static __inline__ ffly_buff_unlock(struct ffly_buff *__buff) {ffly_mutex_unlock(&__buff->m);}
ffly_bool_t static __inline__ ffly_buff_full(struct ffly_buff *__buff) {return (__buff->off > __buff->blk_c);}
ffly_bool_t static __inline__ ffly_buff_empty(struct ffly_buff *__buff) {return !__buff->off;}
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace system {
static ffly_err_t(*buff_init)(struct ffly_buff*, uint_t, uint_t) = &ffly_buff_init;
static ffly_err_t(*buff_put)(struct ffly_buff*, void*) = &ffly_buff_put;
static ffly_err_t(*buff_get)(struct ffly_buff*, void*) = &ffly_buff_get;
static types::err_t(*buff_resize)(struct ffly_buff*, uint_t) = &ffly_buff_resize;
static types::err_t(*buff_de_init)(struct ffly_buff*) = &ffly_buff_de_init;
ffly_err_t static __inline__ buff_incr(struct ffly_buff *__buff) {return ffly_buff_incr(__buff);}
ffly_err_t static __inline__ buff_decr(struct ffly_buff *__buff) {return ffly_buff_decr(__buff);}
void static __inline__ buff_off_reset(struct ffly_buff *__buff) {ffly_buff_off_reset(__buff);}
void static __inline__* buff_begin(struct ffly_buff *__buff) {return ffly_buff_begin(__buff);}
void static __inline__* buff_end(struct ffly_buff *__buff) {return ffly_buff_end(__buff);}
static types::err_t(*buff_reset)(struct ffly_buff*) = &ffly_buff_reset;

template<typename _T>
struct buff {
	buff(uint_t __size) {
		if (this->init(__size) != FFLY_SUCCESS)
			io::printf(stderr, "buff: failed to init.\n");}
	~buff() {
		if (this->de_init() != FFLY_SUCCESS)
			io::printf(stderr, "buff: failed to de_init.\n");}
	types::err_t init(uint_t __size) {buff_init(&this->raw_buff, __size, sizeof(_T));}
	types::err_t de_init() {buff_de_init(&this->raw_buff);}

	_T* begin() {return buff_begin(&this->raw_buff);}
	_T* end() {return buff_end(&this->raw_buff);}

	void put(_T __elem) {buff_put(&this->raw_buff, &__elem);}
	_T get() {
		_T ret;
		buff_get(&this->rawbuff, &ret);
		return ret;
	}

	types::err_t resize(uint_t __nsize) {return buff_resize(&this->raw_buff, __nsize);}
	void reset_off() {buff_off_reset(&this->raw_buff);}
	void reset() {buff_reset(&this->raw_buff);}
	void incr() {buff_incr(&this->raw_buff);}
	void decr() {buff_decr(&this->_buff);}
	struct ffly_buff raw_buff;
};
}
}
}
# endif
# endif /*__buff__h*/

