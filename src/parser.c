# define __ffly_compiler_internal
# define __ffly_parser
# include "parser.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "dep/str_len.h"

# define be_vigilant
static struct node *fast[20];
static struct node **next = fast;

void cu_task(struct ffly_compiler*, void*, void(*)(void*));
# define PAGE_SIZE 20
struct page {
# ifdef be_vigilant
	char ident[4];
# endif
	void *p;
	ff_u16_t off;
	ff_u8_t n;
};

/*
	0h - pointer to page
	< chunk
	0h - bytes from top <- 16bits
	2h - +nodesize - node data
	>
*/

static struct page *pg = NULL;
// rename
ff_i8_t static nofast = -1;
struct node*
ffc_node_alloc(void) {
	if (next>fast && nofast == -1)
		return *(--next);

	if (!pg) {
	_np:
		pg = (struct page*)__ffly_mem_alloc(sizeof(struct page));
		*((struct page**)(pg->p = __ffly_mem_alloc(sizeof(struct page*)+(PAGE_SIZE*(2+sizeof(struct node)))))) = pg;
		pg->p = (void*)((ff_u8_t*)pg->p+sizeof(struct page*));
		pg->off = 0;
		pg->n = 0;
# ifdef be_vigilant
		pg->ident[0] = 'f';
		pg->ident[1] = 'f';
		pg->ident[2] = 'l';
		pg->ident[3] = 'y';
# endif
	} else if (pg->off>=PAGE_SIZE)
		goto _np;
	void *p;

	p = (void*)((ff_u8_t*)pg->p+(pg->off*(2+sizeof(struct node))));
	*(ff_u16_t*)p = pg->off++;
	p = (void*)((ff_u8_t*)p+2);
	pg->n++;

	return (struct node*)p;
}

void
ffc_node_free(struct node *__node) {
	struct page *p;
	if (next != fast+20 && nofast == -1) {
		*(next++) = __node;
		return;
	}

	p = *(struct page**)((((ff_u8_t*)__node-2)-((*(ff_u16_t*)((ff_u8_t*)__node-2))*(2+sizeof(struct node))))-sizeof(struct page*));
# ifdef be_vigilant
	if (p->ident[0] != 'f' || p->ident[1] != 'f' || p->ident[2] != 'l' || p->ident[3] != 'y') {
		ffly_printf("node free failure.\n");
		return;
	}
# endif

	if (!--p->n) {
		__ffly_mem_free((ff_u8_t*)p->p-sizeof(struct page*));	
		__ffly_mem_free(p);
		if (p == pg)
			pg = NULL;
	}
}

void
_node(void *__arg) {
	ffc_node_free(__arg);
}

void ffly_parser_cleanup(void) {
	nofast = 0;
	while(next != fast)
		ffc_node_free(*(--next));
}

void 
ffc_build_node(struct ffly_compiler *__compiler, struct node **__node, struct node *__tmpl) {
	*__node = (struct node*)ffc_node_alloc();
	**__node = *__tmpl;
	(*__node)->p = NULL;
	cu_task(__compiler, *__node, _node);
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
