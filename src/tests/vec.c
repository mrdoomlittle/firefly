# include <time.h>
# include <stdio.h>
# include <string.h>
# include "../system/vec.h"
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
# include "../system/io.h"
int main(void) {
	ffly_io_init();
	struct ffly_vec vec;
	ffly_vec_clear_flags(&vec);
	ffly_vec_tog_flag(&vec, VEC_AUTO_RESIZE|VEC_BLK_CHAIN|VEC_UUU_BLKS);
	ffly_vec_init(&vec, sizeof(struct test));

	struct test *a, *b, *c, *d;

	ffly_vec_push_back(&vec, (void**)&a);
	ffly_vec_push_back(&vec, (void**)&b);
	ffly_vec_push_back(&vec, (void**)&c);

	ffly_vec_del(&vec, (void*)a);
	ffly_vec_push_back(&vec, (void**)&d);

//	ffly_vec_push_back(&vec, (void**)&a);
//	*a = mk('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
	*b = mk('b', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
	*c = mk('c', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
	*d = mk('d', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');

//	pr(a);
	pr(b);
	pr(c);
	pr(d);

	printf("\n\n");

	struct test t;
	ffly_vec_pop_back(&vec, &t);
	pr(&t);
	ffly_vec_pop_back(&vec, &t);
	pr(&t);
	ffly_vec_pop_back(&vec, &t);
	pr(&t);
//	ffly_vec_pop_back(&vec, &t);
//	pr(&t);
	ffly_vec_de_init(&vec);
	ffly_io_closeup();
}
