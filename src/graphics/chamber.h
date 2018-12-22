# ifndef __ffly__graphics__chamber__h
# define __ffly__graphics__chamber__h
# include "../ffint.h"
#define __ffly__chamber__ ffly_cch

/*
	this may need renaming <- 
	ui/btn.c has a function that draws said button so to make things easer 
	when drawing diffrent things.

	might be moved out of /graphics/
*/
ff_u32_t extern ffly_cch;
void ffly_chamber_run(ff_u32_t);
void ffly_chamber_rm(ff_u32_t, ff_uint_t);
ff_u32_t ffly_chamber_add(void(*)(long long), long long, ff_u32_t);
ff_u32_t ffly_chamber_new(void);
# endif /*__ffly__graphics__chamber__h*/
