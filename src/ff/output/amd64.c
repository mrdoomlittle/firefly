# define __ffly_compiler_internal
# define __ffly_generator
# include "../../compiler.h"
# include "../../dep/str_len.h"
# include "../../dep/mem_cpy.h"
# include "../../dep/str_cpy.h"
# include "../../memory/mem_free.h"
# include "../../system/string.h"
# include "../../dep/mem_dup.h"
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
}

void static
emit(ff_compilerp __compiler, struct node *__node) {


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
