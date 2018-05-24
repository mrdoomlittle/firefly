# ifndef __ffly__resin__h
# define __ffly__resin__h
# include "types.h"
# include "ffint.h"
# include "get_bit.h"
# include "ffly_def.h"
/*
	_op_asb
	_op_asw
	...
*/

# define _op_exit 0x0
# define _op_as 0x1
# define _op_jmp 0x2
# define _op_st 0x3
# define _op_ld 0x4
# define _op_out 0x5
# define _op_mov 0x6
# define _op_rin 0x7
# define _op_div 0x8
# define _op_mul 0x9
# define _op_sub 0xa
# define _op_add 0xb
# define _op_inc 0xc
# define _op_dec 0xd
# define _op_cmp 0xe
# define _op_je 0xf
# define _op_jne 0x10
# define _op_jg 0x11
# define _op_jl 0x12
# define _op_call 0x13
# define _op_ret 0x14
# define _gt 0x1
# define _lt 0x2
# define _eq 0x4
// resin byte code interp

typedef struct ffly_resin {
	struct ffly_getbit bit;
	ff_uint_t stack_size;
	ff_u8_t *stack;
	ff_u8_t(*fetch_byte)(ff_off_t);
	void(*ip_incr)(ff_uint_t);
	ff_addr_t(*get_ip)();
	void(*set_ip)(ff_addr_t);
	ff_off_t ip_off;
	void*(*rin)(ff_u8_t, void*);
	ff_addr_t rtbuf[22];//return buffer
	ff_addr_t *retto;
} *ffly_resinp;

void* ff_resin_resolv_adr(ffly_resinp, ff_addr_t);
void ff_resin_sst(ffly_resinp, void*, ff_addr_t, ff_uint_t);
void ff_resin_sld(ffly_resinp, ff_addr_t, void*, ff_uint_t);
ff_err_t ff_resin_init(ffly_resinp);
ff_err_t ff_resin_de_init(ffly_resinp);
ff_err_t ff_resin_exec(ffly_resinp, ff_err_t*);
# endif /*__ffly__resin__h*/
