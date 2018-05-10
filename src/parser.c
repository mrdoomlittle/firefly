# define __ffly_compiler_internal
# define __ffly_parser
# include "compiler.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "dep/str_len.h"

static struct node *fast[20];
static struct node **next = fast;

struct node*
ffc_node_alloc() {
	if (next>fast)
		return *(--next);
	return __ffly_mem_alloc(sizeof(struct node));
}

void
ffc_node_free(struct node *__node) {
	if (next != fast+20) {
		*(next++) = __node;
		return;
	}
	__ffly_mem_free(__node);
}

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

struct type*
ffc_get_typedef(struct ffly_compiler *__compiler, char *__name, ff_err_t *__err) {
	return (struct type*)ffly_map_get(&__compiler->typedefs, __name, ffly_str_len(__name), __err);
}
