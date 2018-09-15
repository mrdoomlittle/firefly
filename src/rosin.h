# ifndef __ffly__rosin__h
# define __ffly__rosin__h
# include "ffint.h"
# include "types.h"

/*
	unlike resin, rosin have complete access to the engine and is not within a sandbox environment
	rosin - calling of functions around engine
	resin - same but limited, only things diffrent is the flexibility e.g. no loops, no jumps, add, sub, div, etc
*/
typedef struct ffly_rosin {
	ff_uint_t stack_size;
	ff_u8_t *stack;

	ff_u8_t(*fetch_byte)(ff_off_t);
	void(*ip_incr)(ff_uint_t);
	ff_addr_t(*get_ip)(void);
	void(*set_ip)(ff_addr_t);
	ff_off_t ip_off;

} *ffly_rosinp;

struct rr_struc {
	/*
		sub operation
	*/
	ff_u8_t op;
	/*
		args for operation
	*/
	void **args;
	// number of args
	ff_u8_t n;
} __attribute__((packed));

void ff_rosin_init(ffly_rosinp);
void ff_rosin_de_init(ffly_rosinp);
void ff_rosin_run(ffly_rosinp);
# endif /*__ffly__rosin__h*/
