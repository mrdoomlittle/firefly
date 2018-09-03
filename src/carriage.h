# ifndef __ffly__carriage__h
# define __ffly__carriage__h
# include "ffint.h"

enum {
	_ff_carr0
};

ff_i8_t ffly_carriage_ready(ff_uint_t);
ff_i8_t ffly_carriage_turn(ff_uint_t, ff_u16_t);
ff_u16_t ffly_carriage_add(ff_uint_t);
void ffly_carriage_put(ff_uint_t);
void ffly_carriage_done(ff_uint_t, ff_u16_t);
void ffly_carriage_wait(ff_uint_t);
void ffly_carriage_reset(ff_uint_t);
void ffly_carriage_dud(ff_uint_t);
void ffly_carriage_udud(ff_uint_t);
void ffly_carriage_cleanup(void);
# endif /*__ffly__carriage__h*/
