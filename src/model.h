# ifndef __ffly__model__h
# define __ffly__model__h
# include "ffint.h"
# include "types.h"
# include "polygon.h"
# define ffly_model_nextpoly(__model) \
	(__model)->off++

struct ffly_mpvec {
	void *inn;
	ff_uint_t n;
};

struct ffly_model_poly_tex {
	ff_u16_t tx, txb;
	ff_uint_t n;
};

typedef struct {
	ffly_polygonp poly;
	ff_uint_t poly_c;
	struct ffly_model_poly_tex *tex;
	ff_uint_t off; // offset from poly to next poly
} ffly_model, *ffly_modelp;

ffly_modelp ffly_model_new(ff_uint_t);
void ffly_model_dismantle(ffly_modelp);
void ffly_model_tex(ffly_modelp, struct ffly_mpvec*);
# endif /*__ffly__model__h*/
