# ifndef __ffly__nt__plate__h
# define __ffly__nt__plate__h
# include "../ffint.h"
struct nt_plate {
	void *p;
	struct nt_plate *link;
};

ff_u16_t nt_plate_alloc(void);
void nt_plate_free(ff_u16_t);
void nt_plate_put(ff_u16_t, void*);
void* nt_plate_get(ff_u16_t);
# endif /*__ffly__nt__plate__h*/
