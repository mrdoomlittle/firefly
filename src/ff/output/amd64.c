# define __ffly_compiler_internal
# define __ffly_generator
# include "../../compiler.h"
# include "../../dep/str_len.h"
# include "../../dep/mem_cpy.h"
# include "../../dep/str_cpy.h"
# include "../../memory/mem_free.h"
# include "../../system/string.h"
# include "../../dep/mem_dup.h"
void static emit(ff_compilerp, struct node*);
void static
out_s(ff_compilerp __compiler, char const *__s) {
	__compiler->out(__s, ffly_str_len(__s));
}

void static
emit_func(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;
 
	p+=ffly_str_cpy(p, __node->p);
	*(p++) = ':';
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
	out_s(__compiler, "pushq %rbp\n");
	out_s(__compiler, "movq %rbp, %rsp\n");

	struct node **itr;
	ffly_vecp block = &__node->block;
	___ffly_vec_nonempty(block) {
		itr = (struct node**)ffly_vec_beg(block);
		while(itr <= (struct node**)ffly_vec_end(block))
			emit(__compiler, *(itr++));
	}

	ffly_vec_de_init(block);


	out_s(__compiler, "movq %rsp, %rbp\n");
	out_s(__compiler, "popq %rbp\n");
	out_s(__compiler, "ret\n");
}

void static
emit_func_call(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p;

	p = buf;
	p+=ffly_str_cpy(p, "callq");
	*(p++) = ' ';
	*(p++) = '$';
	p+=ffly_str_cpy(p, __node->p);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
}

void static
emit_as(ff_compilerp __compiler, struct node *__node) {
	out_s(__compiler, __node->p);
	__ffly_mem_free(__node->p);
}

void
emit(ff_compilerp __compiler, struct node *__node) {
	switch(__node->kind) {
		case _ast_as:
			emit_as(__compiler, __node);
		break;
		case _ast_func:
			emit_func(__compiler, __node);
		break;
		case _ast_func_call:
			emit_func_call(__compiler, __node);
		break;
	}
}

void static
start(ff_compilerp __compiler) {
	out_s(__compiler, "\t.region text\n");
}

void static
final(ff_compilerp __compiler) {
	out_s(__compiler, ".endof\n");
}

# include "../gen.h"
void ffly_ff_amd64(void) {
	ffly_fprintf(ffly_log, "loading amd64 generator.\n");
	ffly_ff_gen_start = start;
	ffly_ff_gen_final = final;
	ffly_ff_emit = emit;
}
