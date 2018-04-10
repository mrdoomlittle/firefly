# ifndef __ffly__bci__h
# define __ffly__bci__h
# include "types/err_t.h"
# include "types/off_t.h"
# include <mdlint.h>
# include "get_bit.h"
# include "ffly_def.h"

# define _op_exit 0x0
# define _op_as 0x1
# define _op_jmp 0x2
# define _op_st 0x3
# define _op_ld 0x4
# define _op_out 0x5 //debug
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
# define _gt 0x1
# define _lt 0x2
# define _eq 0x4
typedef mdl_u16_t ffly_addr_t;
typedef struct ffly_bci {
	struct ffly_getbit bit;
	mdl_uint_t stack_size;
	mdl_u8_t *stack;
	mdl_u8_t(*fetch_byte)(ffly_off_t);
	void(*ip_incr)(mdl_uint_t);
	ffly_addr_t(*get_ip)();
	void(*set_ip)(ffly_addr_t);
	ffly_off_t ip_off;
	void*(*rin)(mdl_u8_t, void*);
} *ffly_bcip;

void ffly_bci_sst(ffly_bcip, void*, ffly_addr_t, mdl_uint_t);
void ffly_bci_sld(ffly_bcip, ffly_addr_t, void*, mdl_uint_t);
ffly_err_t ffly_bci_init(ffly_bcip);
ffly_err_t ffly_bci_de_init(ffly_bcip);
ffly_err_t ffly_bci_exec(ffly_bcip, ffly_err_t*);
# endif /*__ffly__bci__h*/
