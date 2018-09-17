# ifndef __ffly__clay__parser__h
# define __ffly__clay__parser__h
# include "../clay.h"

typedef struct clay_node {
	struct clay_node *next;
	ff_u8_t type;
	char const *name;
	ff_uint_t nlen;

	ff_u8_t val_id;
	ff_u8_t val[8];

	ff_uint_t sz;
	void *p;

	struct clay_node *link;
} *clay_nodep;

clay_nodep clay_parser(ffly_clayp);
# endif /*__ffly__clay__parser__h*/
