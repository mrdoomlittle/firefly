# ifndef __queue__h
# define __queue__h
# include <mdlint.h>
# include "io.h"
# include "../types/err_t.h"
# define QUEUE_PAGE_SIZE 32 // dont change
# define QUEUE_MAX_PAGE_C 32
struct ffly_queue {
	void **p;
	mdl_uint_t page_c, blk_size;
	mdl_uint_t begin_off, end_off;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_queue_init(struct ffly_queue*, mdl_uint_t);
ffly_err_t ffly_queue_de_init(struct ffly_queue*);
mdl_uint_t ffly_queue_size(struct ffly_queue*);
ffly_err_t ffly_queue_push(struct ffly_queue*, void*);
ffly_err_t ffly_queue_pop(struct ffly_queue*, void*);
void* ffly_queue_front(struct ffly_queue*);
void* ffly_queue_back(struct ffly_queue*);
# ifdef __cplusplus
}
# include "errno.h"
namespace mdl {
namespace firefly {
namespace system {
static types::err_t(*queue_init)(struct ffly_queue*, uint_t) = &ffly_queue_init;
static types::err_t(*queue_de_init)(struct ffly_queue*) = &ffly_queue_de_init;
static uint_t(*queue_size)(struct ffly_queue*) = &ffly_queue_size;
static types::err_t(*queue_push)(struct ffly_queue*, void*) = &ffly_queue_push;
static types::err_t(*queue_pop)(struct ffly_queue*, void*) = &ffly_queue_pop;
static void*(*queue_front)(struct ffly_queue*) = &ffly_queue_front;
static void*(*queue_back)(struct ffly_queue*) = &ffly_queue_back;
template<typename _T>
struct queue {
	queue() {
		if (this->init() != FFLY_SUCCESS)
			io::printf(stderr, "queue: failed to init.\n");}
	~queue() {
		if (this->de_init() != FFLY_SUCCESS)
			io::printf(stderr, "queue: failed to de_init.\n");}
	types::err_t init() {return queue_init(&this->raw_queue, sizeof(_T));}
	types::err_t de_init() {return queue_de_init(&this->raw_queue);}
	uint_t size() {return queue_size(&this->raw_queue);}

	types::err_t push(_T __elem) {return queue_push(&this->raw_queue, &__elem);}
	_T pop(types::err_t& __any_err) {
		_T ret;
		__any_err = queue_pop(&this->raw_queue, &ret);
		return ret;
	}

	_T* front() {return (_T*)queue_front(&this->raw_queue);}
	_T* back() {return (_T*)queue_back(&this->raw_queue);}
	struct ffly_queue raw_queue;
};

}
}
}


# endif
# endif /*__queue__h*/
