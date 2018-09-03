# ifndef __ffly__film__h
# define __ffly__film__h
# include "../ffint.h"

struct typo_film {
	void *area;
	ff_uint_t len, page_c;
};


struct typo_film *typo_film_new(void);
// rename
void typo_film_raze(struct typo_film*);

void typo_film_insert(struct typo_film*, void*, ff_uint_t);
# endif /*__ffly__film__h*/
