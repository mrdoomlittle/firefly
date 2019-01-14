# include "matrix.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "context.h"
struct bron_matrix* bron_matrix_new(void) {
	struct bron_matrix *m;
	m = (struct bron_matrix*)__ffly_mem_alloc(sizeof(struct bron_matrix));
	m->data = (ff_i32_t*)__ffly_mem_alloc(16*sizeof(ff_i32_t));

	return m;
}

void bron_matrix_scale(struct bron_matrix *__m, ff_u32_t __x, ff_u32_t __y, ff_u32_t __z) {
	ff_i32_t *d;
	d = __m->data;
	d[0] *= __x;
	d[1] *= __x;
	d[2] *= __x;
	d[3] *= __x;

	d[4] *= __y;
	d[5] *= __y;
	d[6] *= __y;
	d[7] *= __y;

	d[8] *= __z;
	d[9] *= __z;
	d[10] *= __z;
	d[11] *= __z;
}

// copy 16bytes
#define copy128(__dst, __src)\
	__asm__("movdqa (%1),%%xmm0\n\t"\
			"movdqa %%xmm0,(%0)\n\t"\
		: : "r"(__dst), "r"(__src) : "xmm0");

void bron_matrix_load(struct bron_matrix *__m, ff_u32_t *__src) {
	ff_i32_t *d;
	d = __m->data;
	copy128(d, __m);
	copy128(d+4, __m+4);
	copy128(d+8, __m+8);
}

void bron_matrix_destroy(struct bron_matrix *__m) {
	__ffly_mem_free(__m->data);
	__ffly_mem_free(__m);
}
