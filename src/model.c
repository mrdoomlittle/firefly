# include "model.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
ffly_modelp
ffly_model_new(ff_uint_t __poly_c) {
	ffly_modelp m;

	m = (ffly_modelp)__ffly_mem_alloc(sizeof(ffly_model));

	m->poly = (ffly_polygonp)__ffly_mem_alloc(__poly_c*sizeof(ffly_polygon));

	m->poly_c = __poly_c;
	m->off = 0;
	m->texture = NULL;
	return m;
}

void ffly_model_dismantle(ffly_modelp __model) {
	__ffly_mem_free(__model->poly);
	__ffly_mem_free(__model);	
}

void ffly_model_tex(ffly_modelp __model, ff_uint_t __n, ff_u32_t *__loc) {
	ff_uint_t i;

	ff_u32_t *l;

	l = __loc;
	i = 0;
	for(;i != __n;i++)
		(__model->poly+i)->c = *(l++);
}
