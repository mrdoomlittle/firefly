# ifndef __ffly__sr__plate__h
# define __ffly__sr__plate__h
# include "../ffint.h"
struct sr_plate {
	void *p;
	struct sr_plate *link;
};

ff_u16_t sr_plate_alloc(void);
void sr_plate_free(ff_u16_t);
void sr_plate_put(ff_u16_t, void*);
void* sr_plate_get(ff_u16_t);
# endif /*__ffly__sr__plate__h*/
