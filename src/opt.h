# ifndef __ffly__opt__h
# define __ffly__opt__h
# include "ffint.h"
# include "types.h"
typedef struct ffopt {
	char const *val;
} *ffoptp;

# define ffoptval(__opt) \
	(!(__opt)?NULL:(__opt)->val)

void ffoe_prep();
void ffoe_end();

/*
	 P0    P1
	OPT | VALUE
*/

/*
	default
	space is used to seperate parcels
*/
typedef struct ffpcl {
	char const *p, *end;
	char *buf;
} *ffpclp;

/*
	already seperated as in list format
*/
typedef struct ffpcll {
	char const **cur, **end;
} *ffpcllp;

// parcelelized list
char const *ffoe_pcl(void*);
char const* ffoe_pcll(void*);
void ffoe(char const*(*)(void*), void*);
ffoptp ffoe_get(char const*);
void ffoe_clr();
# endif /*__ffly__opt__h*/
