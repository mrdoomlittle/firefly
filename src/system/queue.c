# include "queue.h"
# include "io.h"
# include "err.h"
# include "errno.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_realloc.h"
# include "../dep/mem_cpy.h"
# include "../memory/mem_free.h"
/*
	TODO:
		queue size should depend on (0xffffffffffffffff>>(64-{QUEUE SIZE}))
		so 

		QUEUE_SIZE&MASK


		this will loop back to zero
*/

typedef struct queue_priv {
	void **p;
	ff_uint_t page_c;
	ff_size_t blk_size;
	ff_off_t top, end;
} *queue_privp;

/*
	TODO:
		major cleaning and improv
*/
ff_err_t static
queue_init(queue_privp __queue, ff_uint_t __blk_size) {
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

ff_err_t static
queue_de_init(queue_privp __queue) {
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

ff_size_t static
queue_size(queue_privp __queue) {
	ff_size_t size = 0;
	if (__queue->end < __queue->top)
		size = __queue->end+((__queue->page_c*QUEUE_PAGE_SIZE)-__queue->top);
	else if (__queue->end > __queue->top)
		size = __queue->end-__queue->top;
	return size;
}

ff_err_t static
queue_push(queue_privp __queue, void *__p) {
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
	ff_uint_t page = __queue->end>>QUEUE_PAGE_SHIFT;
	if (!*(__queue->p+page)) {
		if ((*(__queue->p+page) = (void*)__ffly_mem_alloc(QUEUE_PAGE_SIZE*__queue->blk_size)) == NULL) {
			ffly_fprintf(ffly_err, "queue: failed to alloc memory.\n");
			return FFLY_FAILURE;
		}
	}

	ffly_mem_cpy((void*)(((ff_u8_t*)*(__queue->p+page))+((__queue->end++)-(page*QUEUE_PAGE_SIZE))*__queue->blk_size), __p, __queue->blk_size);
	return FFLY_SUCCESS;
}


ff_err_t static
queue_pop(queue_privp __queue, void *__p) {
	if (__queue->top == __queue->end) {
		ffly_fprintf(ffly_err, "queue: underflow.\n");
		return FFLY_FAILURE;
	}

	if (__queue->end < __queue->top && __queue->top+1 == __queue->page_c*QUEUE_PAGE_SIZE && __queue->top <= QUEUE_PAGE_SIZE*QUEUE_MAX_PAGE_C)
		__queue->top = 0;
	ff_uint_t page = __queue->top>>QUEUE_PAGE_SHIFT;
	ffly_mem_cpy(__p, (void*)(((ff_u8_t*)*(__queue->p+page))+((__queue->top++)-(page*QUEUE_PAGE_SIZE))*__queue->blk_size), __queue->blk_size);
	if (__queue->top>>QUEUE_PAGE_SHIFT > page) {
		__ffly_mem_free(*(__queue->p+page));
		*(__queue->p+page) = NULL;
	}
	return FFLY_SUCCESS;
}

void static*
queue_front(queue_privp __queue) {
	return (void*)((ff_u8_t*)(*(__queue->p+(__queue->top>>QUEUE_PAGE_SHIFT)))+((__queue->top-((__queue->top>>QUEUE_PAGE_SHIFT)*QUEUE_PAGE_SIZE))*__queue->blk_size));}
void static*
queue_back(queue_privp __queue) {
	return (void*)((ff_u8_t*)(*(__queue->p+(__queue->end>>QUEUE_PAGE_SHIFT)))+(((__queue->end-((__queue->end>>QUEUE_PAGE_SHIFT)*QUEUE_PAGE_SIZE))-1)*__queue->blk_size));}

void ffly_queue_loadfuncs(ffly_queuep __q) {
	__q->init		= (ff_err_t(*)(long long, ff_uint_t))queue_init;
	__q->de_init	= (ff_err_t(*)(long long))queue_de_init;
	__q->size		= (ff_size_t(*)(long long))queue_size;
	__q->push		= (ff_err_t(*)(long long, void*))queue_push;
	__q->pop		= (ff_err_t(*)(long long, void*))queue_pop;
	__q->front		= (void*(*)(long long))queue_front;
	__q->back		= (void*(*)(long long))queue_back;
	struct queue_priv *pv;
	pv = __ffly_mem_alloc(sizeof(struct queue_priv));


	__q->ctx = (long long)pv;
}
