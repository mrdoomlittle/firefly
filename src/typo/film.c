# include "film.h"
# include "memalloc.h"
# include "../ffly_def.h"
# include "../dep/mem_cpy.h"
/*
	code film
*/
struct typo_film *typo_film_new(void) {
	struct typo_film *film;

	film = (struct typo_film*)ffly_tmalloc(sizeof(struct typo_film));
	film->len = 0;
	film->area = NULL;
	film->page_c = 0;
	return film;
}
// rename
void typo_film_raze(struct typo_film *__film) {
	void *area;

	area = __film->area;
	if (area != NULL)
		ffly_tfree(area);
	ffly_tfree(__film);
}

# define PAGE_SHIFT 3
# define PAGE_SIZE (1<<PAGE_SHIFT)
void typo_film_insert(struct typo_film *__film, void *__buf, ff_uint_t __size) {
	ff_uint_t len;
	
	len = __film->len+__size;
	ff_uint_t pg, pg_c;

	pg = len>>PAGE_SHIFT;
	if (pg >= __film->page_c) {
		pg_c = ((len+(0xffffffffffffffff>>(64-PAGE_SHIFT)))>>PAGE_SHIFT);
		if (!__film->area)
			__film->area = ffly_tmalloc(pg_c*PAGE_SIZE);
		else
			__film->area = ffly_trealloc(__film->area, pg_c*PAGE_SIZE);
		__film->page_c = pg_c;
	}

	ffly_mem_cpy(((ff_u8_t*)__film->area)+__film->len, __buf, __size);
	__film->len = len;
}
