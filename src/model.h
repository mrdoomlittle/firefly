# ifndef __ffly__model__h
# define __ffly__model__h
# include "ffint.h"
# include "types.h"
# include "polygon.h"
# define ffly_model_nextpoly(__model) \
	(__model)->off++
typedef struct {
	ffly_polygonp poly;
	ff_uint_t poly_c;
	ff_byte_t *texture;
	ff_uint_t off; // offset from poly to next poly
} ffly_model, *ffly_modelp;

ffly_modelp ffly_model_new(ff_uint_t);
void ffly_model_dismantle(ffly_modelp);
void ffly_model_tex(ffly_modelp, ff_uint_t, ff_u32_t*);
# endif /*__ffly__model__h*/
