# ifndef __ffly__plate__h
# define __ffly__plate__h
# include "../ffint.h"
/*
	as the name states a plate that holds on to a memory pointer
	this is so be dont have to give foreign entities the pointer
	to that piece of memory and instead give them the plate number.
*/
ff_uint_t ffly_plateno(void*);
void* ffly_plate_at(ff_uint_t);
void ffly_plate_store(void*, void**);
void ffly_plate_load(void*, void*);
void* ffly_plate_alloc(void);
void ffly_plate_free(void*);
void ffly_plate_cleanup(void);
# endif /*__ffly__plate__h*/
