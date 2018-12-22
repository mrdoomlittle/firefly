# ifndef __ffly__queue__h
# define __ffly__queue__h
# include "../ffint.h"
# include "../types.h"
# include "err.h"
#define QUEUE_
/*
	page shift
	value: 32
*/
#define QUEUE_PAGE_SHIFT 5
/*
	real size = page_count*blk_size
*/
#define QUEUE_PAGE_SIZE (1<<QUEUE_PAGE_SHIFT)
/*
	max number of pages.
*/
#define QUEUE_MAX_PAGE_C 32
#define _queue_loadfuncs(...)\
	ffly_queue_loadfuncs(__VA_ARGS__)
#define _queue_init(...)\
	ffly_queue_init(__VA_ARGS__)
#define _queue_de_init(...)\
	ffly_queue_de_init(__VA_ARGS__)
#define _queue_size(...)\
	ffly_queue_size(__VA_ARGS__)
#define _queue_push(...)\
	ffly_queue_push(__VA_ARGS__)
#define _queue_pop(...)\
	ffly_queue_pop(__VA_ARGS__)
#define _queue_front(...)\
	ffly_queue_front(__VA_ARGS__)
#define _queue_back(...)\
	ffly_queue_back(__VA_ARGS__)
struct ffly_queue {
/*
	void **p;
	ff_uint_t page_c;
	ff_size_t blk_size;
	ff_off_t top, end;
*/
	ff_err_t(*init)(long long, ff_uint_t);
	ff_err_t(*de_init)(long long);
	ff_size_t(*size)(long long);
	ff_err_t(*push)(long long, void*);
	ff_err_t(*pop)(long long, void*);
	void*(*front)(long long);
	void*(*back)(long long);
	long long ctx;
};

typedef struct ffly_queue* ffly_queuep;
#define ffly_queue_init(__q, __bks)\
	(__q)->init((__q)->ctx, __bks)
#define ffly_queue_de_init(__q)\
	(__q)->de_init((__q)->ctx)
#define ffly_queue_size(__q)\
	(__q)->size((__q)->ctx)
#define ffly_queue_push(__q, __p)\
	(__q)->push((__q)->ctx, __p)
#define ffly_queue_pop(__q, __p)\
	(__q)->pop((__q)->ctx, __p)
#define ffly_queue_front(__q)\
	(__q)->front((__q)->ctx)
#define ffly_queue_back(__q)\
	(__q)->back((__q)->ctx)
# ifdef __cplusplus
extern "C" {
# endif
/*
ff_err_t ffly_queue_init(ffly_queuep, ff_uint_t);
ff_err_t ffly_queue_de_init(ffly_queuep);
ff_size_t ffly_queue_size(ffly_queuep);
ff_err_t ffly_queue_push(ffly_queuep, void*);
ff_err_t ffly_queue_pop(ffly_queuep, void*);
void* ffly_queue_front(ffly_queuep);
void* ffly_queue_back(ffly_queuep);
*/
void ffly_queue_loadfuncs(ffly_queuep);
# ifdef __cplusplus
}
# include "io.h"
# include "errno.h"
namespace mdl {
namespace firefly {
namespace system {
static types::err_t(*queue_init)(ffly_queuep, uint_t) = &ffly_queue_init;
static types::err_t(*queue_de_init)(ffly_queuep) = &ffly_queue_de_init;
static types::size_t(*queue_size)(ffly_queuep) = &ffly_queue_size;
static types::err_t(*queue_push)(ffly_queuep, void*) = &ffly_queue_push;
static types::err_t(*queue_pop)(ffly_queuep, void*) = &ffly_queue_pop;
static void*(*queue_front)(ffly_queuep) = &ffly_queue_front;
static void*(*queue_back)(ffly_queuep) = &ffly_queue_back;
template<typename _T>
struct queue {
	queue() {
		if (_err(this->init()))
			io::fprintf(ffly_err, "queue: failed to init.\n");}
	~queue() {
		if (_err(this->de_init()))
			io::fprintf(ffly_err, "queue: failed to de_init.\n");}
	types::err_t init() {return queue_init(&this->raw, sizeof(_T));}
	types::err_t de_init() {return queue_de_init(&this->raw);}
	types::size_t size() {return queue_size(&this->raw);}

	types::err_t push(_T __elem) {return queue_push(&this->raw, &__elem);}
	_T pop(types::err_t& __err) {
		_T ret;
		__err = queue_pop(&this->raw, &ret);
		return ret;
	}

	_T* front() {return (_T*)queue_front(&this->raw);}
	_T* back() {return (_T*)queue_back(&this->raw);}
	struct ffly_queue raw;
};

}
}
}


# endif
# endif /*__ffly__queue__h*/
