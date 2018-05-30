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
emit(ff_compilerp __compiler, struct node *__node) {


}

void static
start(ff_compilerp __compiler) {
}

void static
final(ff_compilerp __compiler) {

}

# include "../gen.h"
void ffly_ff_amd64(void) {
	ffly_fprintf(ffly_log, "loading amd64 generator.\n");
	ffly_ff_gen_start = start;
	ffly_ff_gen_final = final;
	ffly_ff_emit = emit;
}
