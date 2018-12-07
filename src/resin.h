# ifndef __ffly__resin__h
# define __ffly__resin__h
# include "types.h"
# include "ffint.h"
# include "get_bit.h"
# include "ffly_def.h"

/*
	TODO:
		add op to run rosin code

		why? to add function to it resin is more bone and more flex
		but rosin is for running internal engine function like alloc free from allocr or
		graphics functions etc.  

		the only things that resin and rosin should share is minimal operations like 'assign', 'out' for debug
		operations like mov, and, or, div, etc would only be within resin so basically work them together and
		it makes things abit more interesting.

		also add way for rosin and resin to access the stack of each other to allow for flow
		between them

		NOTE:
			resin will run rosin code
			or rosin will execute resin code

			but only if needed as to many calls
			will slow thigs down if doing so in small parts
*/
/*
	_op_asb
	_op_asw
	...
*/

# define _op_exit 0x00
// assign
# define _op_asb 0x01
# define _op_asw 0x02
# define _op_asd 0x03
# define _op_asq 0x04

# define _op_jmp 0x05
// store
# define _op_stb 0x06
# define _op_stw 0x07
# define _op_std 0x08
# define _op_stq 0x09
// load
# define _op_ldb 0x0a
# define _op_ldw 0x0b
# define _op_ldd 0x0c
# define _op_ldq 0x0d
// out
# define _op_outb 0x0e
# define _op_outw 0x0f
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

# define _op_as1br0 0x39
# define _op_as1wr0 0x3a
# define _op_as1dr0 0x3b
# define _op_as1qr0 0x3c

# define _op_as1br1 0x3d
# define _op_as1wr1 0x3e
# define _op_as1dr1 0x3f
# define _op_as1qr1 0x40

# define _op_mov1br0 0x41
# define _op_mov1wr0 0x42
# define _op_mov1dr0 0x43
# define _op_mov1qr0 0x44

# define _op_mov1br1 0x45
# define _op_mov1wr1 0x46
# define _op_mov1dr1 0x47
# define _op_mov1qr1 0x48

# define _op_mov2br0 0x49
# define _op_mov2wr0 0x4a
# define _op_mov2dr0 0x4b
# define _op_mov2qr0 0x4c

# define _op_mov2br1 0x4d
# define _op_mov2wr1 0x4e
# define _op_mov2dr1 0x4f
# define _op_mov2qr1 0x50

# define _op_mov3br0r0 0x51
# define _op_mov3br1r0 0x52
# define _op_mov3br2r0 0x53
# define _op_mov3br3r0 0x54

# define _op_mov3br0r1 0x55
# define _op_mov3br1r1 0x56
# define _op_mov3br2r1 0x57
# define _op_mov3br3r1 0x58

# define _op_out1br0 0x59
# define _op_out1wr0 0x5a
# define _op_out1dr0 0x5b
# define _op_out1qr0 0x5c

# define _op_out1br1 0x5d
# define _op_out1wr1 0x5e
# define _op_out1dr1 0x5f
# define _op_out1qr1 0x60

# define _op_inc1br0 0x61
# define _op_inc1wr0 0x62
# define _op_inc1dr0 0x63
# define _op_inc1qr0 0x64

# define _op_inc1br1 0x65
# define _op_inc1wr1 0x66
# define _op_inc1dr1 0x67
# define _op_inc1qr1 0x68

# define _op_dec1br0 0x69
# define _op_dec1wr0 0x6a
# define _op_dec1dr0 0x6b
# define _op_dec1qr0 0x6c

# define _op_dec1br1 0x6d
# define _op_dec1wr1 0x6e
# define _op_dec1dr1 0x6f
# define _op_dec1qr1 0x70

# define _op_jmp1r0 0x71
# define _op_jmp1r1 0x72

# define _op_div1br0r1 0x73
# define _op_div1wr0r1 0x74
# define _op_div1dr0r1 0x75
# define _op_div1qr0r1 0x76

# define _op_div1br1r0 0x77
# define _op_div1wr1r0 0x78
# define _op_div1dr1r0 0x79
# define _op_div1qr1r0 0x7a

# define _op_mul1br0r1 0x7b
# define _op_mul1wr0r1 0x7c
# define _op_mul1dr0r1 0x7d
# define _op_mul1qr0r1 0x7e

# define _op_mul1br1r0 0x7f
# define _op_mul1wr1r0 0x80
# define _op_mul1dr1r0 0x81
# define _op_mul1qr1r0 0x82

# define _op_sub1br0r1 0x83
# define _op_sub1wr0r1 0x84
# define _op_sub1dr0r1 0x85
# define _op_sub1qr0r1 0x86

# define _op_sub1br1r0 0x87
# define _op_sub1wr1r0 0x88
# define _op_sub1dr1r0 0x89
# define _op_sub1qr1r0 0x8a

# define _op_add1br0r1 0x8b
# define _op_add1wr0r1 0x8c
# define _op_add1dr0r1 0x8d
# define _op_add1qr0r1 0x8e

# define _op_add1br1r0 0x8f
# define _op_add1wr1r0 0x90
# define _op_add1dr1r0 0x91
# define _op_add1qr1r0 0x92

# define _op_lbpr0 0x93
# define _op_lbpr1 0x94

# define _op_lspr0 0x95
# define _op_lspr1 0x96

# define _op_r0r0 0x97
# define _op_r1r0 0x98
# define _op_r2r0 0x99
# define _op_r3r0 0x9a

# define _op_r0r1 0x9b
# define _op_r1r1 0x9c
# define _op_r2r1 0x9d
# define _op_r3r1 0x9e

# define _op_push0b 0x9f
# define _op_push0w 0xa0
# define _op_push0d 0xa1
# define _op_push0q 0xa2
# define _op_pop0b 0xa3
# define _op_pop0w 0xa4
# define _op_pop0d 0xa5
# define _op_pop0q 0xa6

# define _op_push1br0 0xa7
# define _op_push1wr0 0xa8
# define _op_push1dr0 0xa9
# define _op_push1qr0 0xaa
# define _op_push1br1 0xab
# define _op_push1wr1 0xac
# define _op_push1dr1 0xad
# define _op_push1qr1 0xae

# define _op_pop1br0 0xaf
# define _op_pop1wr0 0xb0
# define _op_pop1dr0 0xb1
# define _op_pop1qr0 0xb2
# define _op_pop1br1 0xb3
# define _op_pop1wr1 0xb4
# define _op_pop1dr1 0xb5
# define _op_pop1qr1 0xb6

# define _op_exit1 0xb7
# define _op_exit2r0 0xb8
# define _op_exit2r1 0xb9

#define _op_jmp0 0xba
#define _gt 0x01
#define _lt 0x02
#define _eq 0x04

// resin byte code interp

/*
	TODO:
		exec context

	TODO:
		no use of ff_off_t, ff_addr_t, etc
		use own.
		also remove getbit mabey? can it be used for somthing else ???
*/
typedef struct ffly_resin {
	struct ffly_getbit bit;
	ff_uint_t stack_size;
	ff_u8_t *stack;
	ff_u64_t bp, sp;
	/*
		registers pointers

		or rp0, rp1, rp2, rp3

		pointers to registers r0, r1, ...
	*/
	ff_u64_t *r[4];
	/*
		condition flags, {cmp, and ...}
	*/
	ff_u8_t cflags;

	// register data
	ff_u64_t r0, r1, r2, r3;
	ff_u8_t(*fetch_byte)(ff_off_t);
	void(*ip_incr)(ff_uint_t);
	ff_addr_t(*get_ip)(void);
	void(*set_ip)(ff_addr_t);
	ff_off_t ip_off;
	void*(*rin)(ff_u8_t, void*);
	ff_addr_t rtbuf[22];//return buffer
	ff_addr_t *retto;
} *ffly_resinp;
/*
	TODO:
stack_get and stack_put go thru pointer so to allow user to change shit

to be passed thru exec 
struct exec_context {

};
*/
/*
	operation size
	TODO:
		rename??? to operation magnitude or weight?? or just opcs for operation code size
	used by:
		resin dismantler - rdm
*/
ff_u8_t ff_resin_ops(ff_u8_t);
void* ff_resin_resolv_adr(ffly_resinp, ff_addr_t);
/*
	TODO:
		i think names are switched to lazy to check
*/
void ff_resin_sst(ffly_resinp, void*, ff_addr_t, ff_uint_t);
void ff_resin_sld(ffly_resinp, ff_addr_t, void*, ff_uint_t);

void ff_resin_rset(ffly_resinp, void*, ff_u8_t, ff_uint_t);
void ff_resin_rget(ffly_resinp, ff_u8_t, void*, ff_uint_t);

ff_err_t ff_resin_init(ffly_resinp);
ff_err_t ff_resin_de_init(ffly_resinp);
ff_err_t ff_resin_exec(ffly_resinp, ff_err_t*);
# endif /*__ffly__resin__h*/
