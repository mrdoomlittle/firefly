# ifndef __ffly__header__h
# define __ffly__header__h
# include "../ffint.h"
# include "../linux/time.h"
#define FF_NET_HDR struct ff_net_hdr

/*
	TODO:
		add timestamp to segment header
*/
struct ff_net_sins {
	ff_u32_t seg;
	ff_uint_t len;
};

struct ff_net_hdr {
    ff_uint_t size; 

	struct {
		ff_u32_t tv_sec;
		ff_u64_t tv_nsec;
	} ts;
} __attribute__((packed));
// keep packed

# endif /*__ffly_header__h*/
