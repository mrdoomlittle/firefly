# ifndef __ffly__pkg__h
# define __ffly__pkg__h
# include "ffint.h"
#define IG_FILE     0x00
#define IG_DIR      0x01
#define IG_UNKNOWN  0x02
#define IG_ISFILE(__type) \
	(__type == IG_FILE)
#define IG_ISDIR(__type) \
	(__type == IG_DIR)

/*
	do clean up and naming
*/
struct pkc_header {
	ff_u32_t dof, dlen;
	ff_u32_t it;
	ff_uint_t ic;

};


struct __pkc_ingot {
	ff_u8_t type;
	ff_u32_t path;
	ff_uint_t plen;
	ff_u32_t tract;
	ff_uint_t ts;
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
