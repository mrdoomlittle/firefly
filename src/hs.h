# ifndef __ffly__hs__h
# define __ffly__hs__h
# include "ffint.h"
# include "types.h"
# include "hs/hash.h"
# include "hs/vec.h"
typedef struct ff_hs {
	ff_u8_t *p;
	ff_u32_t off, end;
	struct hash env;
} *ff_hsp;

enum {
	_null,
	_tok_ident,
	_tok_keywd,
	_tok_no
};

enum {
	_keywd_func,
	_keywd_object,
	_keywd_model,
	_keywd_ret,
	_l_bracket,
	_r_bracket,
	_semicolon,
	_l_brace,
	_r_brace,
	_pound,
	_comma,
	_keywd_struct,
	_period,
	_eq,
	_keywd_uint_t,
	_keywd_int_t,
	_keywd_body,
	_keywd_fset,
	_keywd_fget
};

enum {
	_object,
	_model,
	_body,
	_uint_t,
	_struct,
	_int_t
};

enum {
	_func,
	_decl,
	__ret,
	_func_call,
	_rin,
	_var,
	_literal,
	_struct_ref,
	_assign,
	_fset,
	_fget
};

enum  {
	_op_fresh,
	_op_frame,
	_op_subside,
	_op_exit,
	_op_call,
	_op_ret,
	_op_jmp,
	_op_rin,
	_op_push,
	_op_pop,
	_op_assign,
	_op_mim,
	_op_copy,
	_op_fset,
	_op_fget
};

# define _rl 0x1

typedef struct ff_hs_obj {
	struct ff_hs_obj *next;
	ff_u8_t op;
	ff_uint_t size, offset;
	void *p;
	struct ff_hs_obj *to, *obj, **src, **dst, **sz;
	struct ff_hs_obj ***params;
} *ff_hs_objp;

typedef struct ff_hs_type {
	ff_u8_t id;
	struct hash fields;
	ff_uint_t offset, size;
} *ff_hs_typep;

typedef struct ff_hs_node {
	struct ff_hs_node *next;
	/*
		node might be linked to object
	*/
	ff_hs_objp obj, *start;

	struct ff_hs_vec vec, params;
	struct ff_hs_node *func, *var, *struc;
	struct ff_hs_node *l, *r, *sz;
	ff_hs_typep t;
	ff_u8_t kind;
	ff_u64_t val;
} *ff_hs_nodep;

char const* nks(ff_u8_t);
char const* tids(ff_u8_t);
ff_u8_t ff_hs_tsize(ff_u8_t);
ff_u8_t ff_hs_perhaps_keywd(ff_u8_t);
char ff_hs_getc(ff_hsp);
void ff_hs_ugetc(ff_hsp, char);
ff_u8_t ff_hs_at_eof(ff_hsp);
ff_u8_t ff_hs_nexttokis(ff_hsp, ff_u8_t, ff_u8_t);
ff_u8_t ff_hs_reckon(ff_hsp, ff_u8_t, ff_u8_t);
ff_hs_objp ff_hs_build(ff_hsp);
void ff_hs_load(ff_hsp, char const*);
void ff_hs_clean(ff_hsp);
void ff_hs_init(ff_hsp);
# endif /*__ffly__hs__h*/
