# define __ffly_compiler_internal
# define __ffly_generator
# include "../compiler.h"
# include "../dep/str_len.h"
# include "../dep/mem_cpy.h"
# include "../dep/str_cpy.h"
# include "../memory/mem_free.h"
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
	char *p = buf;

	ff_u8_t *flags = &__node->flags;
	if ((*flags&_func_gbl)>0)
		p+=ffly_str_cpy(p, ".globl");
	else if ((*flags&_func_exr)>0)
		p+=ffly_str_cpy(p, ".extern");
	*(p++) = ' ';
	p+=ffly_str_cpy(p, __node->p);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);

	if ((*flags&_func_def)>0) {
		p = buf;
		p+=ffly_str_cpy(p, __node->p);
		*(p++) = ':';
		*p = '\n';
		__compiler->out(buf, (p-buf)+1);

		struct node **itr;
		if (__node->_block != NULL) {
			___ffly_vec_nonempty(__node->_block) {
				itr = (struct node**)ffly_vec_begin(__node->_block);
				while(itr <= (struct node**)ffly_vec_end(__node->_block))
					emit(__compiler, *(itr++));
			}
			ffly_vec_de_init(__node->_block);
			__ffly_mem_free(__node->_block);
		}
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
