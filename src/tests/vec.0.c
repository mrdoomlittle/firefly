# include <time.h>
# include <stdio.h>
# include <string.h>
# include "../system/vec.h"
# include "../types/err_t.h"
# include "../system/errno.h"
# include "../system/err.h"
# include "../data/mem_cmp.h"
# include <mdlint.h>
struct test {
	mdl_u8_t a, b, c, d, e, f, g, h, i, j, k;
};

struct test mk(mdl_u8_t __a, mdl_u8_t __b, mdl_u8_t __c, mdl_u8_t __d,
	mdl_u8_t __e, mdl_u8_t __f, mdl_u8_t __g, mdl_u8_t __h, mdl_u8_t __i, mdl_u8_t __j, mdl_u8_t __k) {
	return (struct test) {
		.a = __a,
		.b = __b,
		.c = __c,
		.d = __d,
		.e = __e,
		.f = __f,
		.g = __g,
		.h = __h,
		.i = __i,
		.j = __j,
		.k = __k
	};
}

void pr(struct test *__t) {
	printf("%c%c%c%c%c%c%c%c%c%c%c\n", __t->a, __t->b, __t->c, __t->d, __t->e, __t->f, __t->g, __t->h, __t->i, __t->j, __t->k);
}
# define failure {\
	err = FFLY_FAILURE;\
	goto _end;}
ffly_err_t err = FFLY_SUCCESS;
# include "../system/io.h"
int main(void) {
	ffly_io_init();
	struct ffly_vec vec;
	ffly_vec_clear_flags(&vec);
	ffly_vec_tog_flag(&vec, VEC_AUTO_RESIZE|VEC_BLK_CHAIN|VEC_UUU_BLKS);
	if (_err(ffly_vec_init(&vec, sizeof(struct test)))) failure;

	struct test *a, *b, *c, *d;

	if (_err(ffly_vec_push_back(&vec, (void**)&a))) failure;
	if (_err(ffly_vec_push_back(&vec, (void**)&b))) failure;
	if (_err(ffly_vec_push_back(&vec, (void**)&c))) failure;

	ffly_vec_del(&vec, (void*)a);
	if (_err(ffly_vec_push_back(&vec, (void**)&d))) failure;

	if (_err(ffly_vec_push_back(&vec, (void**)&a))) failure;
	*a = mk('A', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
	*b = mk('B', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
	*c = mk('C', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
	*d = mk('D', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');

	pr(a);
	pr(b);
	pr(c);
	pr(d);

	printf("--\n\n");

	struct test save;
	struct test t;
	save = *a;
    printf("s0.\n");
	if (_err(ffly_vec_pop_back(&vec, &t))) failure;
	if (ffly_mem_cmp(&t, &save, sizeof(struct test))) failure;
	pr(&t);
	save = *d;
    printf("s1.\n");
	if (_err(ffly_vec_pop_back(&vec, &t))) failure;
	if (ffly_mem_cmp(&t, &save, sizeof(struct test))) failure;
	pr(&t);
	save = *c;
    printf("s2.\n");
	if (_err(ffly_vec_pop_back(&vec, &t))) failure;
	if (ffly_mem_cmp(&t, &save, sizeof(struct test))) failure;
	pr(&t);
	save = *b;
    printf("s3.\n");
	if (_err(ffly_vec_pop_back(&vec, &t))) failure;
	if (ffly_mem_cmp(&t, &save, sizeof(struct test))) failure;
	pr(&t);
	_end:
    if (_err(err)) {
        printf("failure.\n");
    }

	if (_err(ffly_vec_de_init(&vec))) {
		err = FFLY_FAILURE;
    }
	ffly_io_closeup();
	return err;
}
