# ifndef __ffly__shell__parser__h
# define __ffly__shell__parser__h
# include "../ffint.h"
enum {
	_cmd
};

typedef struct node {
	ff_u8_t kind;
	char const *name;
	ff_uint_t nl;
	void *args[20];
	ff_uint_t argc;
} *nodep;

nodep ffsh_parse(void);
# endif /*__ffly__shell__parser__h*/
