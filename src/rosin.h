# ifndef __ffly__rosin__h
# define __ffly__rosin__h
# include "ffint.h"
# include "types.h"
typedef struct ffly_rosin {
	ff_uint_t stack_size;
	ff_u8_t *stack;

	ff_u8_t(*fetch_byte)(ff_off_t);
	void(*ip_incr)(ff_uint_t);
	ff_addr_t(*get_ip)(void);
	void(*set_ip)(ff_addr_t);
	ff_off_t ip_off;

} *ffly_rosinp;


void ff_rosin_init(ffly_rosinp);
void ff_rosin_de_init(ffly_rosinp);
void ff_rosin_run(ffly_rosinp);
# endif /*__ffly__rosin__h*/
