# ifndef __ffly__script__h
# define __ffly__script__h
# include "types/byte_t.h"
# include "types/err_t.h"
# include "compiler.h"
# ifdef __ffly_script_internal
enum {
	_64l_u,
	_32l_u,
	_16l_u,
	_8l_u,
	_64l_s,
	_32l_s,
	_16l_s,
	_8l_s,
	_float32,
	_float64
};

enum {
	_op_fresh_,
	_op_free_,
	_op_assign_,
	_op_copy_,
	_op_print_,
	_op_compare_,
	_op_jump_,
	_op_cond_jump_,
	_op_zero_,
	_op_push_,
	_op_pop_,
	_op_incr_,
	_op_decr_,
	_op_call_,
	_op_exit_,
	_op_frame_,
	_op_free_frame_,
	_op_conv_,
	_op_add_,
	_op_sub_,
	_op_mul_,
	_op_div_
};
# endif

typedef struct {
/*
	-----------------
	| static memory |
	-----------------
	| to be used    |
	-----------------
*/
	ffly_byte_t *stack;
	ffly_byte_t *fresh;
	void *top; // top object
	void *arg_p;
	// id, arg_p, params that where passed thru. max 12
	void*(*call)(mdl_u8_t, void*, void**);
} ffscript, *ffscriptp;

struct ffly_script {
	//compiler context
	struct ffly_compiler c_ctx;
};
/* clean this up */
struct obj {
	mdl_u32_t off;
	mdl_u8_t opcode, cond;
	void *p;
	mdl_uint_t size;
	mdl_u8_t _type;
	struct obj *objpp, *frame;
	struct obj **dst, **src, *_obj, **no;
	struct obj **to, **from, **l, **r, **by;
	struct obj **val, ***jmp, *flags;
	struct obj *next;
	struct obj **params[12];
};

ffly_err_t ffly_script_build(struct ffly_script*, void**, ffly_byte_t**);
ffly_err_t ffly_script_prepare(struct ffly_script*);
ffly_err_t ffly_script_free(struct ffly_script*);

ffly_err_t ffscript_init(ffscriptp, mdl_uint_t);
ffly_err_t ffscript_exec(ffscriptp, void*(*)(mdl_u8_t, void*, void**), void*, void*, void*);
void* ffscript_call(ffscriptp, void*);
ffly_err_t ffscript_free(ffscriptp);
ffly_err_t ffly_script_free(struct ffly_script*);
# endif /*__ffly__script__h*/
