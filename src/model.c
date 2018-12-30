# include "model.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# include "dep/mem_set.h"
# include "bron/tex.h"
ffly_modelp
ffly_model_new(ff_uint_t __poly_c) {
	ffly_modelp m;

	ffly_printf("new model,  poly count: %u.\n", __poly_c);
	m = (ffly_modelp)__ffly_mem_alloc(sizeof(ffly_model));

	m->poly = (ffly_polygonp)__ffly_mem_alloc(__poly_c*sizeof(ffly_polygon));
	ffly_mem_set(m->poly, 0, __poly_c*sizeof(ffly_polygon));

	m->poly_c = __poly_c;
	m->off = 0;
	m->tex = (struct ffly_model_poly_tex*)__ffly_mem_alloc(__poly_c*sizeof(struct ffly_model_poly_tex));
	return m;
}

void ffly_model_dismantle(ffly_modelp __model) {
	__ffly_mem_free(__model->poly);
	__ffly_mem_free(__model);	
}

void ffly_model_tex(ffly_modelp __model, struct ffly_mpvec *__vec) {
	ff_uint_t i;

	struct ffly_mpvec *p;
	struct ffly_model_poly_tex *tx;
	i = 0;
	while(i != __model->poly_c) {
		p = __vec+i;
		tx = __model->tex+i;
		tx->txb = ffly_bron_texbuf_new(p->n*4);
		// map texture memory
		ffly_bron_texbuf_map(tx->txb);
		ffly_bron_texbuf_write(tx->txb, 0, p->n*4, p->inn);
		tx->tx = ffly_bron_tex_new(tx->txb);
		tx->n = p->n;
		i++;
	}
}
