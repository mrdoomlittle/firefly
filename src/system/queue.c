# include "queue.h"
# include "io.h"
# include "err.h"
# include "errno.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_realloc.h"
# include "../data/mem_cpy.h"
# include "../memory/mem_free.h"
ffly_err_t ffly_queue_init(struct ffly_queue *__queue, mdl_uint_t __blk_size) {
	__queue->page_c = 0;
	if ((__queue->p = (void**)__ffly_mem_alloc((++__queue->page_c)*sizeof(void*))) == NULL) {
		ffly_fprintf(ffly_err, "queue: failed to alloc memory.\n");
		return FFLY_FAILURE;
	}

	__queue->blk_size = __blk_size;
	if ((*(__queue->p+(__queue->page_c-1)) = (void*)__ffly_mem_alloc(QUEUE_PAGE_SIZE*__queue->blk_size)) == NULL) {
		ffly_fprintf(ffly_err, "queue: failed to init page.\n");
		return FFLY_FAILURE;
	}
	__queue->top = 0;
	__queue->end = 0;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_queue_de_init(struct ffly_queue *__queue) {
	void **itr = __queue->p;
	while(itr != __queue->p+__queue->page_c) {
		if (*itr != NULL) {
			ffly_fprintf(ffly_log, "queue: page %u is going to be freed.\n", itr-__queue->p);
			__ffly_mem_free(*itr);
		}
		itr++;
	}

	if (__queue->p != NULL)
		__ffly_mem_free(__queue->p);
	return FFLY_SUCCESS;
}

ffly_size_t ffly_queue_size(struct ffly_queue *__queue) {
	ffly_size_t size = 0;
	if (__queue->end < __queue->top)
		size = __queue->end+((__queue->page_c*QUEUE_PAGE_SIZE)-__queue->top);
	else if (__queue->end > __queue->top)
		size = __queue->end-__queue->top;
	return size;
}

ffly_err_t ffly_queue_push(struct ffly_queue *__queue, void *__p) {
	if (__queue->end+1 == __queue->top) {
		ffly_fprintf(ffly_err, "queue: overflow.\n");
		return FFLY_NOP;
	}

	if (!__queue->p) {
		if ((__queue->p = (void**)__ffly_mem_alloc((++__queue->page_c)*sizeof(void*))) == NULL) {
			ffly_fprintf(ffly_err, "queue: failed to alloc memory.\n");
			return FFLY_FAILURE;
		}
		goto _init_page;
 	}

	if (__queue->end>>QUEUE_PAGE_SHIFT == __queue->page_c) {
		if ((__queue->p = (void**)__ffly_mem_realloc(__queue->p, (++__queue->page_c)*sizeof(void*))) == NULL) {
			ffly_fprintf(ffly_err, "queue: failed to realloc memory.\n");
			return FFLY_FAILURE;
		}
		goto _init_page;
	}

	goto _sk_init_page;
	_init_page:
	if ((*(__queue->p+(__queue->page_c-1)) = (void*)__ffly_mem_alloc(QUEUE_PAGE_SIZE*__queue->blk_size)) == NULL) {
		ffly_fprintf(ffly_err, "queue: failed to init page.\n");
		return FFLY_FAILURE;
	}

	_sk_init_page:
	if (__queue->top>>QUEUE_PAGE_SHIFT > 0 && __queue->end == __queue->page_c*QUEUE_PAGE_SIZE && __queue->end <= QUEUE_PAGE_SIZE*QUEUE_MAX_PAGE_C)
		__queue->end = 0;
	mdl_uint_t page = __queue->end>>QUEUE_PAGE_SHIFT;
	if (!*(__queue->p+page)) {
		if ((*(__queue->p+page) = (void*)__ffly_mem_alloc(QUEUE_PAGE_SIZE*__queue->blk_size)) == NULL) {
			ffly_fprintf(ffly_err, "queue: failed to alloc memory.\n");
			return FFLY_FAILURE;
		}
	}

	ffly_mem_cpy((void*)(((mdl_u8_t*)*(__queue->p+page))+((__queue->end++)-(page*QUEUE_PAGE_SIZE))*__queue->blk_size), __p, __queue->blk_size);
	return FFLY_SUCCESS;
}


ffly_err_t ffly_queue_pop(struct ffly_queue *__queue, void *__p) {
	if (__queue->top == __queue->end) {
		ffly_fprintf(ffly_err, "queue: underflow.\n");
		return FFLY_FAILURE;
	}

	if (__queue->end < __queue->top && __queue->top+1 == __queue->page_c*QUEUE_PAGE_SIZE && __queue->top <= QUEUE_PAGE_SIZE*QUEUE_MAX_PAGE_C)
		__queue->top = 0;
	mdl_uint_t page = __queue->top>>QUEUE_PAGE_SHIFT;
	ffly_mem_cpy(__p, (void*)(((mdl_u8_t*)*(__queue->p+page))+((__queue->top++)-(page*QUEUE_PAGE_SIZE))*__queue->blk_size), __queue->blk_size);
	if (__queue->top>>QUEUE_PAGE_SHIFT > page) {
		__ffly_mem_free(*(__queue->p+page));
		*(__queue->p+page) = NULL;
	}
	return FFLY_SUCCESS;
}

void* ffly_queue_front(struct ffly_queue *__queue) {
	return (void*)((mdl_u8_t*)(*(__queue->p+(__queue->top>>QUEUE_PAGE_SHIFT)))+((__queue->top-((__queue->top>>QUEUE_PAGE_SHIFT)*QUEUE_PAGE_SIZE))*__queue->blk_size));}
void* ffly_queue_back(struct ffly_queue *__queue) {
	return (void*)((mdl_u8_t*)(*(__queue->p+(__queue->end>>QUEUE_PAGE_SHIFT)))+(((__queue->end-((__queue->end>>QUEUE_PAGE_SHIFT)*QUEUE_PAGE_SIZE))-1)*__queue->blk_size));}
