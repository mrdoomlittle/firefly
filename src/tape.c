# include "tape.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "ffly_def.h"
# include "dep/mem_cpy.h"
/*
	code tape
*/

/*
	TODO:
		alloc,re,free should go thru function points if needed
*/
ffly_tapep ffly_tape_new(void) {
	ffly_tapep tape;

	tape = (ffly_tapep)__ffly_mem_alloc(sizeof(struct ffly_tape));
	tape->len = 0;
	tape->area = NULL;
	tape->page_c = 0;
	return tape;
}

void ffly_tape_get(ffly_tapep __tape, ff_uint_t __where, void *__buf, ff_uint_t __size) {
	ffly_mem_cpy(__buf, ((ff_u8_t*)__tape->area)+__where, __size);
}

// rename
void ffly_tape_raze(ffly_tapep __tape) {
	void *area;

	area = __tape->area;
	if (area != NULL)
		__ffly_mem_free(area);
	__ffly_mem_free(__tape);
}

# define PAGE_SHIFT 3
# define PAGE_SIZE (1<<PAGE_SHIFT)
void ffly_tape_insert(struct ffly_tape *__tape, void *__buf, ff_uint_t __size) {
	ff_uint_t len;
	
	len = __tape->len+__size;
	ff_uint_t pg, pg_c;

	pg = len>>PAGE_SHIFT;
	if (pg >= __tape->page_c) {
		pg_c = ((len+(0xffffffffffffffff>>(64-PAGE_SHIFT)))>>PAGE_SHIFT);
		if (!__tape->area)
			__tape->area = __ffly_mem_alloc(pg_c*PAGE_SIZE);
		else
			__tape->area = __ffly_mem_realloc(__tape->area, pg_c*PAGE_SIZE);
		__tape->page_c = pg_c;
	}

	ffly_mem_cpy(((ff_u8_t*)__tape->area)+__tape->len, __buf, __size);
	__tape->len = len;
}
