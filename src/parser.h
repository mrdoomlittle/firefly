# ifndef __ffly__parser__h
# define __ffly__parser__h
# ifdef __ffly_compiler_internal
# include "ffint.h"
# include "compiler.h"
// later
typedef struct ff_parser {

} *ff_parserp;


struct node* ffc_node_alloc(void);
void ffc_node_free(struct node*);
void ffly_parser_cleanup(void);
# endif
# endif /*__ffly__parser__h*/
