# ifndef __ffly__queue__h
# define __ffly__queue__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "../types/off_t.h"
# include "../types/size_t.h"
# include "err.h"
/*
	page shift
	value: 32
*/
# define QUEUE_PAGE_SHIFT 5
/*
	real size = page_count*blk_size
*/
# define QUEUE_PAGE_SIZE (1<<QUEUE_PAGE_SHIFT)
/*
	max number of pages.
*/
# define QUEUE_MAX_PAGE_C 32
struct ffly_queue {
	void **p;
	mdl_uint_t page_c;
	ffly_size_t blk_size;
	ffly_off_t top, end;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_queue_init(struct ffly_queue*, mdl_uint_t);
ffly_err_t ffly_queue_de_init(struct ffly_queue*);
ffly_size_t ffly_queue_size(struct ffly_queue*);
ffly_err_t ffly_queue_push(struct ffly_queue*, void*);
ffly_err_t ffly_queue_pop(struct ffly_queue*, void*);
void* ffly_queue_front(struct ffly_queue*);
void* ffly_queue_back(struct ffly_queue*);
# ifdef __cplusplus
}
# include "io.h"
# include "errno.h"
namespace mdl {
namespace firefly {
namespace system {
static types::err_t(*queue_init)(struct ffly_queue*, uint_t) = &ffly_queue_init;
static types::err_t(*queue_de_init)(struct ffly_queue*) = &ffly_queue_de_init;
static types::size_t(*queue_size)(struct ffly_queue*) = &ffly_queue_size;
static types::err_t(*queue_push)(struct ffly_queue*, void*) = &ffly_queue_push;
static types::err_t(*queue_pop)(struct ffly_queue*, void*) = &ffly_queue_pop;
static void*(*queue_front)(struct ffly_queue*) = &ffly_queue_front;
static void*(*queue_back)(struct ffly_queue*) = &ffly_queue_back;
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
