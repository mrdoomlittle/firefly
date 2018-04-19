# define __ffly_compiler_internal
# define __ffly_parser
# include "compiler.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
void 
ffc_build_node(struct ffly_compiler *__compiler, struct node **__node, struct node *__tmpl) {
	*__node = (struct node*)__ffly_mem_alloc(sizeof(struct node));
	**__node = *__tmpl;
	(*__node)->p = NULL;
	cleanup(__compiler, (void*)*__node);
}

void 
ffc_build_type(struct ffly_compiler *__compiler, struct type **__type, struct type *__tmpl) {
	*__type = (struct type*)__ffly_mem_alloc(sizeof(struct type));
	**__type = *__tmpl;
	cleanup(__compiler, (void*)*__type);
}

ffly_mapp
ffc_get_env(struct ffly_compiler *__compiler) {
	return __compiler->local != NULL?__compiler->local:&__compiler->env;
}
