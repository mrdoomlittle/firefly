# ifndef __ffly__bron__matrix__h
# define __ffly__bron__matrix__h
# include "../ffint.h"
struct bron_matrix {
	ff_u32_t *data;	
};

struct bron_matrix* bron_matrix_new(void);
void bron_matrix_load(struct bron_matrix*, ff_u32_t*);
void bron_matrix_destroy(struct bron_matrix*);
# endif /*__ffly__bron__matrix__h*/
