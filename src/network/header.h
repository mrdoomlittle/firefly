# ifndef __ffly__header__h
# define __ffly__header__h
# include <mdlint.h>
# define FF_NET_HDR struct ff_net_hdr
struct ff_net_hdr {
    mdl_u32_t cksum; 
} __attribute__((packed));
// keep packed
# endif /*__ffly_header__h*/
