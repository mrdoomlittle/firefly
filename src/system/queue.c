# include "queue.h"
# include "io.h"
# include "errno.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_realloc.h"
# include "../data/mem_cpy.h"
# include "../memory/mem_free.h"
ffly_err_t ffly_queue_init(struct ffly_queue *__queue, mdl_uint_t __blk_size) {
	__queue->page_c = 0;
	if ((__queue->p = (void**)__ffly_mem_alloc((++__queue->page_c)*sizeof(void*))) == NULL) {
		ffly_printf(stderr, "queue: failed to alloc memory.\n");
		return FFLY_FAILURE;
	}

	__queue->blk_size = __blk_size;
	if ((*(__queue->p+(__queue->page_c-1)) = (void*)__ffly_mem_alloc(QUEUE_PAGE_SIZE*__queue->blk_size)) == NULL) {
		ffly_printf(stderr, "queue: failed to init page.\n");
		return FFLY_FAILURE;
	}
	__queue->begin_off = __queue->end_off = 0;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_queue_de_init(struct ffly_queue *__queue) {
	void **itr = __queue->p;
	while(itr != __queue->p+__queue->page_c) {
		if (*itr != NULL) __ffly_mem_free(*itr);
		itr++;
	}

	if (__queue->p != NULL)
		__ffly_mem_free(__queue->p);
	return FFLY_SUCCESS;
}

mdl_uint_t ffly_queue_size(struct ffly_queue *__queue) {
	mdl_uint_t ret_val = 0;
	if (__queue->end_off < __queue->begin_off)
		ret_val = __queue->end_off+((__queue->page_c*QUEUE_PAGE_SIZE)-__queue->begin_off);
	else if (__queue->end_off > __queue->begin_off)
		ret_val = __queue->end_off-__queue->begin_off;
	return ret_val;
}

ffly_err_t ffly_queue_push(struct ffly_queue *__queue, void *__p) {
	if (__queue->end_off+1 == __queue->begin_off) {
		ffly_printf(stderr, "queue: overflow.\n");
		return FFLY_NOP;
	}

	if (!__queue->p) {
		if ((__queue->p = (void**)__ffly_mem_alloc((++__queue->page_c)*sizeof(void*))) == NULL) {
			ffly_printf(stderr, "queue: failed to alloc memory.\n");
			return FFLY_FAILURE;
		}
		goto _init_page;
 	}

	if (__queue->end_off >> 5 == __queue->page_c) {
		if ((__queue->p = (void**)__ffly_mem_realloc(__queue->p, (++__queue->page_c)*sizeof(void*))) == NULL) {
			ffly_printf(stderr, "queue: failed to realloc memory.\n");
			return FFLY_FAILURE;
		}
		goto _init_page;
	}

	goto _sk_init_page;
	_init_page:
	if ((*(__queue->p+(__queue->page_c-1)) = (void*)__ffly_mem_alloc(QUEUE_PAGE_SIZE*__queue->blk_size)) == NULL) {
		ffly_printf(stderr, "queue: failed to init page.\n");
		return FFLY_FAILURE;
	}
	_sk_init_page:

	if (__queue->begin_off >> 5 > 0 && __queue->end_off == __queue->page_c*QUEUE_PAGE_SIZE && __queue->end_off <= QUEUE_PAGE_SIZE*QUEUE_MAX_PAGE_C) __queue->end_off = 0;
	mdl_uint_t page = __queue->end_off >> 5;
	if (!*(__queue->p+page)) {
		if ((*(__queue->p+page) = (void*)__ffly_mem_alloc(QUEUE_PAGE_SIZE*__queue->blk_size)) == NULL) {
			ffly_printf(stderr, "queue: failed to alloc memory.\n");
			return FFLY_FAILURE;
		}
	}

	ffly_mem_cpy((void*)(((mdl_u8_t*)*(__queue->p+page))+((__queue->end_off++)-(page*QUEUE_PAGE_SIZE))*__queue->blk_size), __p, __queue->blk_size);
	return FFLY_SUCCESS;
}


ffly_err_t ffly_queue_pop(struct ffly_queue *__queue, void *__p) {
	if (__queue->begin_off == __queue->end_off) {
		ffly_printf(stderr, "queue: underflow.\n");
		return FFLY_FAILURE;
	}

	if (__queue->end_off < __queue->begin_off && __queue->begin_off+1 == __queue->page_c*QUEUE_PAGE_SIZE && __queue->begin_off <= QUEUE_PAGE_SIZE*QUEUE_MAX_PAGE_C) __queue->begin_off = 0;
	mdl_uint_t page = __queue->begin_off >> 5;
	ffly_mem_cpy(__p, (void*)(((mdl_u8_t*)*(__queue->p+page))+((__queue->begin_off++)-(page*QUEUE_PAGE_SIZE))*__queue->blk_size), __queue->blk_size);
	if (__queue->begin_off >> 5 > page) {
		__ffly_mem_free(*(__queue->p+page));
		*(__queue->p+page) = NULL;
	}
	return FFLY_SUCCESS;
}

void* ffly_queue_front(struct ffly_queue *__queue) {
	return (void*)((mdl_u8_t*)(*(__queue->p+(__queue->begin_off>>5)))+((__queue->begin_off-((__queue->begin_off>>5)*QUEUE_PAGE_SIZE))*__queue->blk_size));}
void* ffly_queue_back(struct ffly_queue *__queue) {
	return (void*)((mdl_u8_t*)(*(__queue->p+(__queue->end_off>>5)))+(((__queue->end_off-((__queue->end_off>>5)*QUEUE_PAGE_SIZE))-1)*__queue->blk_size));}
