# ifndef __ffly__texture__h
# define __ffly__texture__h
# include "ffint.h"
# define ffly_texture_innards(__tex) \
	((__tex)->innards)
# define TX_INO 0x01
typedef struct {
	void *innards;
	ff_u8_t flags;
	ff_uint_t size;
	ff_u16_t tx, txb;
} ffly_texture, *ffly_texturep;

ffly_texturep ffly_texture_creat(ff_uint_t);
ff_u16_t ffly_tex_get(ffly_texturep);
void ffly_texture_bulldoze(ffly_texturep);
void ffly_texture_resize(ffly_texturep, ff_uint_t);
# endif /*__ffly__texture__h*/
