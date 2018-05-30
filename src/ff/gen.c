# define __ffly_compiler_internal
# define __ffly_generator
# include "../compiler.h"
void(*ffly_ff_gen_start)(ff_compilerp);
void(*ffly_ff_gen_final)(ff_compilerp);
void(*ffly_ff_emit)(ff_compilerp, struct node*);

ff_err_t
ffly_ff_gen(ff_compilerp __compiler) {
	if (!ffly_vec_size(&__compiler->nodes)) return FFLY_FAILURE;

	ffly_ff_gen_start(__compiler);
	___ffly_vec_nonempty(&__compiler->nodes) {
		struct node **p = (struct node**)ffly_vec_begin(&__compiler->nodes);
		while(!vec_deadstop(p, &__compiler->nodes))
			ffly_ff_emit(__compiler, *(p++));
	}

	ffly_ff_gen_final(__compiler);
	retok;
}
