# ifndef __buff__h
# define __buff__h
# include "../ffint.h"
# include "../types.h"
# include "io.h"
# include "mutex.h"
# include "err.h"
# include "flags.h"
#ifdef __cplusplus
#	include "errno.h"
#endif
#define FF_BGF_MA	0x00
#define FF_BGF_MF	0x01
#define FF_BGF_MR	0x02
struct ffly_buff {
#ifdef FF_BUFF_SA
	void*(*alloc)(long long, ff_uint_t);
	void(*free)(long long, void*);
	void*(*realloc)(long long, void*, ff_uint_t);
	long long ma_arg;
#endif
	ff_mlock_t lock;
	ff_u8_t *p;
	ff_off_t off;
	ff_uint_t rs_blk_c;
	ff_uint_t blk_c;
	ff_size_t blk_size;
    ff_flag_t flags; //for later
};

typedef struct ffly_buff* ffly_buffp;
# ifdef __cplusplus
extern "C" {
# endif
#define ffly_buff_init(__buff, __bc, __bs)\
	_ffly_buff_init(__buff, __bc, __bs, NULL)
ff_err_t _ffly_buff_init(ffly_buffp, ff_uint_t, ff_size_t, void(*)(long long, ff_u8_t));
ff_err_t ffly_buff_put(ffly_buffp, void*);
ff_err_t ffly_buff_get(ffly_buffp, void*);
ff_err_t ffly_buff_resize(ffly_buffp, ff_uint_t);
ff_err_t ffly_buff_de_init(ffly_buffp);
ff_err_t ffly_buff_incr(ffly_buffp);
ff_err_t ffly_buff_decr(ffly_buffp);
ff_err_t ffly_buff_reset(ffly_buffp);
void* ffly_buff_at(ffly_buffp, ff_uint_t);
void* ffly_buff_getp(ffly_buffp);
# ifdef __cplusplus
}
# endif
ff_uint_t static __inline__ ffly_buff_ublk_c(struct ffly_buff *__buff){return __buff->off;}
void static __inline__ ffly_buff_off_reset(struct ffly_buff *__buff) {__buff->off = 0;}
void static __inline__* ffly_buff_begin(struct ffly_buff *__buff) {return (void*)__buff->p;}
void static __inline__* ffly_buff_end(struct ffly_buff *__buff) {return (void*)(__buff->p+((__buff->off-1)*__buff->blk_size));}
void static __inline__ ffly_buff_lock(struct ffly_buff *__buff) {ffly_mutex_lock(&__buff->lock);}
void static __inline__ ffly_buff_unlock(struct ffly_buff *__buff) {ffly_mutex_unlock(&__buff->lock);}
ff_bool_t static __inline__ ffly_buff_full(struct ffly_buff *__buff) {return (__buff->off == __buff->blk_c-1);}
ff_bool_t static __inline__ ffly_buff_empty(struct ffly_buff *__buff) {return !__buff->off;}
ff_off_t static __inline__ ffly_buff_off(struct ffly_buff *__buff){return __buff->off;}
ff_uint_t static __inline__ ffly_buff_size(struct ffly_buff *__buff){return __buff->blk_c;}
# ifdef __cplusplus
namespace ff {
static err_t(*buff_init)(ffly_buffp, uint_t, size_t) = &ffly_buff_init;
static err_t(*buff_put)(ffly_buffp, void*) = &ffly_buff_put;
static err_t(*buff_get)(ffly_buffp, void*) = &ffly_buff_get;
static err_t(*buff_resize)(ffly_buffp, uint_t) = &ffly_buff_resize;
static err_t(*buff_de_init)(ffly_buffp) = &ffly_buff_de_init;
err_t static __inline__ buff_incr(struct ffly_buff *__buff) {return ffly_buff_incr(__buff);}
err_t static __inline__ buff_decr(struct ffly_buff *__buff) {return ffly_buff_decr(__buff);}
void static __inline__ buff_off_reset(struct ffly_buff *__buff) {ffly_buff_off_reset(__buff);}
void static __inline__* buff_begin(struct ffly_buff *__buff) {return ffly_buff_begin(__buff);}
void static __inline__* buff_end(struct ffly_buff *__buff) {return ffly_buff_end(__buff);}
static err_t(*buff_reset)(ffly_buffp) = &ffly_buff_reset;

template<typename _T>
struct buff {
	buff(uint_t __size) {
		if (this->init(__size) != FFLY_SUCCESS)
			fprintf(ffly_err, "buff: failed to init.\n");}
	~buff() {
		if (this->de_init() != FFLY_SUCCESS)
			fprintf(ffly_err, "buff: failed to de_init.\n");}
	err_t init(size_t __size) {buff_init(&this->raw, __size, sizeof(_T));}
	err_t de_init() {buff_de_init(&this->raw);}

	_T* begin() {return buff_begin(&this->raw);}
	_T* end() {return buff_end(&this->raw);}

	void put(_T __elem) {buff_put(&this->raw, &__elem);}
	_T get() {
		_T ret;
		buff_get(&this->rawbuff, &ret);
		return ret;
	}

	err_t resize(size_t __size) {return buff_resize(&this->raw, __size);}
	void reset_off() {buff_off_reset(&this->raw);}
	void reset() {buff_reset(&this->raw);}
	void incr() {buff_incr(&this->raw);}
	void decr() {buff_decr(&this->raw);}

	void operator++(int) {this->incr();}
	void operator--(int) {this->decr();}
	_T& operator*() {
		return *static_cast<_T*>(ffly_buff_getp(&this->raw));
	}
	struct ffly_buff raw;
};
}
# endif
# endif /*__buff__h*/

