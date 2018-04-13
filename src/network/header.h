# ifndef __ffly__header__h
# define __ffly__header__h
# include "../ffint.h"
# define FF_NET_HDR struct ff_net_hdr
struct ff_net_hdr {
    ff_uint_t size; 
    ff_u16_t seg_no;
} __attribute__((packed));
// keep packed

# endif /*__ffly_header__h*/
