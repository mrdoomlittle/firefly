# ifndef __ffly__pkg__h
# define __ffly__pkg__h
# include "ffint.h"

struct pkc_header {


};

struct pkc_ingot {
    ff_u8_t type;
    char const *path;
    ff_uint_t plen;
	ff_u32_t tract;
};

struct pkc_plan {
    struct pkc_ingot **i;
    ff_uint_t n;
};

void ff_pkc_init(void);
struct pkc_plan* pkc_get_plan(void);
void ff_pkc_construct(char const*, char const*);
void ff_pkc_deconstruct(char const*, char const*);
void ff_pkc_de_init(void);
# endif /*__ffly__pkg__h*/
