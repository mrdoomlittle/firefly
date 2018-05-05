# ifndef __ffly__plate__h
# define __ffly__plate__h
# include "../ffint.h"
/*
	i gave up trying to name this,
	soo what this is ment to do
	is to allow us to use the plate number
	and not the address to when giving a piece 
	of memory to a unknown entity we give them 
	the plate number and not the address
*/
ff_uint_t ffly_plateno(void*);
void* ffly_plate_at(ff_uint_t);
void ffly_plate_store(void*, void**);
void ffly_plate_load(void*, void*);
void* ffly_plate_alloc(void);
void ffly_plate_free(void*);
void ffly_plate_cleanup();
# endif /*__ffly__plate__h*/
