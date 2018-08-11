# ifndef __ffly__texture__h
# define __ffly__texture__h
# include "ffint.h"
# define ffly_texture_innards(__tex) \
	((__tex)->innards)
typedef struct {
	void *innards;
} ffly_texture, *ffly_texturep;

ffly_texturep ffly_texture_creat(ff_uint_t);
void ffly_texture_bulldoze(ffly_texturep);
void ffly_texture_resize(ffly_texturep, ff_uint_t);
# endif /*__ffly__texture__h*/
