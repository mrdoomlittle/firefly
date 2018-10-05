# ifndef __ffly__header__h
# define __ffly__header__h
# include "../ffint.h"
# define FF_NET_HDR struct ff_net_hdr
struct ff_net_sins {
	ff_u32_t seg;
	ff_uint_t len;
};

struct ff_net_hdr {
    ff_uint_t size; 
} __attribute__((packed));
// keep packed

# endif /*__ffly_header__h*/
