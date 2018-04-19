# define __ffly_compiler_internal
# define __ffly_generator
# include "../compiler.h"
# include "../dep/str_len.h"
# include "../dep/mem_cpy.h"
# include "../dep/str_cpy.h"

void emit(ff_compilerp, struct node*);
void static
out_s(ff_compilerp __compiler, char const *__s) {
	__compiler->out(__s, ffly_str_len(__s));
}

void static
emit_as(ff_compilerp __compiler, struct node *__node) {
	out_s(__compiler, __node->p);
}

void static
emit_func(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	ff_uint_t l;
	l = ffly_str_cpy(buf, __node->p);
	*(buf+l) = ':';
	*(buf+l+1) = '\n';
	__compiler->out(buf, l+2);

	struct node **itr;
	___ffly_vec_nonempty(&__node->block) {
		itr = (struct node**)ffly_vec_begin(&__node->block);
		while(itr <= (struct node**)ffly_vec_end(&__node->block))
			emit(__compiler, *(itr++));
	}
}

void static
emit_ret(ff_compilerp __compiler, struct node *__node) {
	out_s(__compiler, "ret\n");
}

void static
emit_func_call(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;

	p+=ffly_str_cpy(p, "call");
	*(p++) = ' ';
	*(p++) = '$';
	p+=ffly_str_cpy(p, __node->p);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
}

void emit(ff_compilerp __compiler, struct node *__node) {
	switch(__node->kind) {
		case _ast_func:
			emit_func(__compiler, __node);
		break;
		case _ast_as:
			emit_as(__compiler, __node);
		break;
		case _ast_ret:
			emit_ret(__compiler, __node);
		break;
		case _ast_func_call:
			emit_func_call(__compiler, __node);
		break;
	}
}


ff_err_t
ffly_ff_gen(ff_compilerp __compiler) {
	if (!ffly_vec_size(&__compiler->nodes)) return FFLY_FAILURE;

	out_s(__compiler, ".region text\n");
	___ffly_vec_nonempty(&__compiler->nodes) {
		struct node **p = (struct node**)ffly_vec_begin(&__compiler->nodes);
		while(!vec_deadstop(p, &__compiler->nodes))
			emit(__compiler, *(p++));
	}
	out_s(__compiler, ".endof\n");
	retok;
}
