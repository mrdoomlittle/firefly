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
// assign
# define _op_asb 0x1
# define _op_asw 0x2
# define _op_asd 0x3
# define _op_asq 0x4

# define _op_jmp 0x5
// store
# define _op_stb 0x6
# define _op_stw 0x7
# define _op_std 0x8
# define _op_stq 0x9
// load
# define _op_ldb 0xa
# define _op_ldw 0xb
# define _op_ldd 0xc
# define _op_ldq 0xd
// out
# define _op_outb 0xe
# define _op_outw 0xf
# define _op_outd 0x10
# define _op_outq 0x11
// move
# define _op_movb 0x12
# define _op_movw 0x13
# define _op_movd 0x14
# define _op_movq 0x15

# define _op_rin 0x16

# define _op_divb 0x17
# define _op_divw 0x18
# define _op_divd 0x19
# define _op_divq 0x1a

# define _op_mulb 0x1b
# define _op_mulw 0x1c
# define _op_muld 0x1d
# define _op_mulq 0x1e

# define _op_subb 0x1f
# define _op_subw 0x20
# define _op_subd 0x21
# define _op_subq 0x22

# define _op_addb 0x23
# define _op_addw 0x24
# define _op_addd 0x25
# define _op_addq 0x26

# define _op_incb 0x27
# define _op_incw 0x28
# define _op_incd 0x29
# define _op_incq 0x2a

# define _op_decb 0x2b
# define _op_decw 0x2c
# define _op_decd 0x2d
# define _op_decq 0x2e

# define _op_cmpb 0x2f
# define _op_cmpw 0x30
# define _op_cmpd 0x31
# define _op_cmpq 0x32

# define _op_je 0x33
# define _op_jne 0x34
# define _op_jg 0x35
# define _op_jl 0x36
# define _op_call 0x37
# define _op_ret 0x38
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
