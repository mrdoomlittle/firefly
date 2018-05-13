# ifndef __ffly__script__h
# define __ffly__script__h
# define __ffly_compiler_internal
# include "types.h"
# include "compiler.h"
# include "system/map.h"
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
	| other.........|
	-----------------
	-----------------
	| to be used    |
	-----------------
*/
	ff_byte_t *stack;
	ff_byte_t *fresh;
	void *top; // top object
	void *arg_p;
	// id, arg_p, params that where passed thru. max 12
	void*(*call)(ff_u8_t, void*, void**);
	struct ffly_map symbols;
} ffscript, *ffscriptp;

struct ffly_script {	
	//compiler context
	struct ffly_compiler c_ctx;
};

struct obj;
typedef struct symbol {
	// function
	struct obj **start, **end;	

	ff_u8_t type;
	struct symbol *next;
} *symbolp;

/* clean this up */
struct obj {
	ff_uint_t off;
	ff_u8_t opno, cond;
	void *p;
	ff_uint_t size;
	ff_u8_t _type;
	struct obj *objpp, *frame;
	struct obj **dst, **src, *_obj, **no;
	struct obj **to, **from, **l, **r, **by;
	struct obj **val, ***jmp, *flags;
	struct obj *next;
	struct obj **params[12];
};

ff_err_t ffly_script_build(struct ffly_script*, void**, ff_byte_t**);
ff_err_t ffly_script_prepare(struct ffly_script*);
ff_err_t ffly_script_free(struct ffly_script*);

ff_err_t ffscript_init(ffscriptp, ff_uint_t);
ff_err_t ffscript_exec(ffscriptp, void*(*)(ff_u8_t, void*, void**), void*, void*, void*);
void* ffscript_call(ffscriptp, char const*);
ff_err_t ffscript_free(ffscriptp);
ff_err_t ffly_script_free(struct ffly_script*);
# endif /*__ffly__script__h*/
