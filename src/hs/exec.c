# include "exec.h"
# include "../ffly_def.h"
# include "../system/io.h"
# include "../dep/mem_cpy.h"
# include "../system/error.h"
// return buffer
# define RTBUF_SIZE 20

ff_hs_objp static rtbuf[RTBUF_SIZE];
ff_hs_objp static *retto = rtbuf;

ff_hs_objp static* cop;
ff_u8_t static stack[2048];
ff_u8_t static *sp = stack;

void static **fin_tbl;

ff_u8_t static flags;
void static
o_fresh(ff_hs_objp __obj) {
	__obj->p = sp;
	sp+=__obj->size;
	ffly_printf("size: %u\n", __obj->size);
}

void static
o_frame(ff_hs_objp __obj) {
	__obj->p = sp;
}

void static
o_subside(ff_hs_objp __obj) {
	sp = (ff_u8_t*)__obj->p;
}

ff_i8_t static done;
void static
o_exit(ff_hs_objp __obj) {
	done = 0;
}

void static
o_call(ff_hs_objp __obj) {
	if (retto-rtbuf>=RTBUF_SIZE) {
		ffly_printf("rt buffer overflow.\n");
		return;
	}
	*(retto++) = __obj->next;
	*cop = __obj->to;
	flags |= _rl;
}

void static
o_ret(ff_hs_objp __obj) {
	*cop = *(--retto);
	flags |= _rl;
}

void static
o_jmp(ff_hs_objp __obj) {
	*cop = __obj->to;
	flags |= _rl;
}

# include "../obj.h"
# include "../model.h"
void static
obj_alloc(ff_hs_objp **__params) {
	struct ffly_obj **obj = (struct ffly_obj**)(**__params)->p;

	ff_err_t err;
	*obj = ffly_obj_alloc(&err);
	ffly_printf("alloc object, %p\n", *obj);
}

void static
obj_prep(ff_hs_objp **__params) {
	struct ffly_obj *obj = *(struct ffly_obj**)(**__params)->p;

	ffly_obj_prepare(obj);	
	ffly_printf("prep object, %p\n", obj);
}

void static
obj_body(ff_hs_objp **__params) {
	struct ffly_obj *obj = *(struct ffly_obj**)(**__params)->p;
	ff_u8_t flags = *(ff_u8_t*)(*__params[1])->p;
	ffly_obj_body(obj, flags);
	ffly_printf("gave object a body, %p, %u\n", obj->phy_body, flags);
}

void static
get_body(ff_hs_objp **__params) {
	struct ffly_obj *obj = *(struct ffly_obj**)(**__params)->p;
	struct ffly_phy_body **body = (struct ffly_phy_body**)(*__params[1])->p;
	*body = ffly_get_phy_body(obj->phy_body);
}

void static
model_new(ff_hs_objp **__params) {
	ffly_model **m = (ffly_model**)(**__params)->p;
	ff_uint_t poly_c = *(ff_uint_t*)(*__params[1])->p;
	*m = ffly_model_new(poly_c);
	ffly_printf("new model, %p\n", *m);
}

void static
body_model_set(ff_hs_objp **__params) {
	struct ffly_phy_body *body = *(struct ffly_phy_body**)(**__params)->p;
	ffly_model *m = *(ffly_model**)(*__params[1])->p;
	body->model = m;
	ffly_printf("%p, %p\n", body, m);
}

void static
model_nextpoly(ff_hs_objp **__params) {
	ffly_model *m = *(ffly_model**)(**__params)->p;
	ffly_model_nextpoly(m);
}

void static
body_vertex(ff_hs_objp **__params) {
	struct ffly_phy_body *body = *(struct ffly_phy_body**)(**__params)->p;
	ff_int_t x = *(ff_int_t*)(*__params[1])->p;
	ff_int_t y = *(ff_int_t*)(*__params[2])->p;
	ff_int_t z = *(ff_int_t*)(*__params[3])->p;

	ffly_printf("%p - %u, %u, %u, %p\n", body, x, y, z, body->model);
	ffly_body_vertex(body, x, y, z);
}

void static
print(ff_hs_objp **__params) {
	void *p = (**__params)->p;

	ffly_printf("%u\n", *(ff_uint_t*)p);
}

static void(*rin[])(ff_hs_objp**) = {
	obj_alloc,
	obj_prep,
	obj_body,
	model_new,
	body_model_set,
	model_nextpoly,
	body_vertex,
	get_body,
	print
};

void static
o_rin(ff_hs_objp __obj) {
	ff_u8_t no;

	no = *(ff_u8_t*)(*(*__obj->params))->p;
	rin[no](__obj->params+1);
}

ff_hs_objp static objs[20];
ff_hs_objp static* obp = objs;

void static
o_push(ff_hs_objp __obj) {
	*(obp++) = __obj->obj;
}

void static
o_pop(ff_hs_objp __obj) {
	__obj->obj = *(--obp);
}

void static
o_assign(ff_hs_objp __obj) {
	ffly_mem_cpy(((ff_u8_t*)__obj->to->p)+__obj->offset, __obj->p, __obj->size);
}

void static
o_mim(ff_hs_objp __obj) {
	__obj->p = ((ff_u8_t*)__obj->obj->p)+__obj->offset;
}

void static
o_copy(ff_hs_objp __obj) {
	void *src;
	void *dst;

	src = (*__obj->src)->p;
	dst = (*__obj->dst)->p;
	
	ffly_mem_cpy(dst, src, __obj->size);
}

void static
o_fset(ff_hs_objp __obj) {
	ff_uint_t sz;
	void *src;
	void *dst;

	sz = *(ff_uint_t*)(*__obj->sz)->p;
	src = (*__obj->src)->p;
	dst = *(fin_tbl+(*(ff_u8_t*)(*__obj->dst)->p));
	ffly_mem_cpy(dst, src, sz);
}

void static
o_fget(ff_hs_objp __obj) {
	ff_uint_t sz;
	void *src;
	void *dst;

	sz = *(ff_uint_t*)(*__obj->sz)->p;
	src = *(fin_tbl+(*(ff_u8_t*)(*__obj->src)->p));
	dst = (*__obj->dst)->p;
	ffly_mem_cpy(dst, src, sz);
}

static void(*op[])(ff_hs_objp) = {
	o_fresh,
	o_frame,
	o_subside,
	o_exit,
	o_call,
	o_ret,
	o_jmp,
	o_rin,
	o_push,
	o_pop,
	o_assign,
	o_mim,
	o_copy,
	o_fset,
	o_fget
};

char const *ops(ff_u8_t __op) {
	switch(__op) {
		case _op_fresh:
			return "fresh";
		case _op_frame:
			return "frame";
		case _op_subside:
			return "subside";
		case _op_exit:
			return "exit";
		case _op_call:
			return "call";
		case _op_ret:
			return "ret";
		case _op_jmp:
			return "jmp";
		case _op_rin:
			return "rin";
		case _op_push:
			return "push";
		case _op_pop:
			return "pop";
		case _op_assign:
			return "assign";
		case _op_mim:
			return "mim";
		case _op_copy:
			return "copy";
		case _op_fset:
			return "fset";
		case _op_fget:
			return "fget";
	}
	return "unknown";
}

void ff_hs_run(ff_hs_objp __top, void **__fin_tbl) {
	fin_tbl = __fin_tbl;
	done = -1;
	ff_hs_objp cur;
	
	cur = __top;
	cop = &cur;
	while(cur != NULL && done == -1) {
		ffly_printf("op: %s\n", ops(cur->op));
		flags = 0x0;
		op[cur->op](cur);
		if ((flags&_rl)>0)
			continue;
		cur = cur->next;
	}
}
