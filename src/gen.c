# define __ffly_compiler_internal
# define __ffly_script_internal
# define __ffly_generator
# include "compiler.h"
# include "script.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/buff.h"
# include "dep/mem_cpy.h"
struct obj static *rg_8l_u, *rg_16l_u, *rg_32l_u, *rg_64l_u;
void static
build_obj(struct obj **__obj, struct obj *__tmpl) {
	*__obj = (struct obj*)__ffly_mem_alloc(sizeof(struct obj));
	**__obj = *__tmpl;
}

struct obj static**
objpp(struct obj *__obj) {
	__obj->objpp = __obj;
	return &__obj->objpp;
}

struct obj static*
get_reg(mdl_u8_t __kind) {
	switch(__kind) {
		case _8l_u: return rg_8l_u;
		case _16l_u: return rg_16l_u;
		case _32l_u: return rg_32l_u;
		case _64l_u: return rg_64l_u; 
		case _8l_s: return NULL;
		case _16l_s: return NULL;
		case _32l_s: return NULL;
		case _64l_s: return NULL;
	}
}

ffly_byte_t static **stack;
static struct obj *top = NULL;
static struct obj *end = NULL;
struct obj static*
next_obj(struct ffly_compiler *__compiler, struct obj __tmpl) {
	struct obj *_obj;
	build_obj(&_obj, &__tmpl);
	_obj->next = NULL;
	if (top == NULL)
		top = (void*)_obj;
	else {
		end->next = _obj;
		end = _obj;
	}

	end = _obj;
	return _obj;
}

mdl_u8_t static
convtk(mdl_u8_t __kind) {
	if (__kind == _uint_t) {
		switch(sizeof(mdl_uint_t)) {
			case 1: return _8l_u;
			case 2: return _16l_u;
			case 4: return _32l_u;
			case 8: return _64l_u;
		}
	}
	switch(__kind) {
		case _u64_t: return _64l_u;
		case _u32_t: return _32l_u;
		case _u16_t: return _16l_u;
		case _u8_t: return _8l_u;
		case _i64_t: return _64l_s;
		case _i32_t: return _32l_s;
		case _i16_t: return _16l_s;
		case _i8_t: return _8l_s;
		case _float: return _float64;
	}
	return 0;
}

struct obj obj_tmpl = {
	.off = 0, .opno = 0, .cond = 0,
	.p = NULL, .to = NULL, .from = NULL,
	.l = NULL, .r = NULL, .val = NULL, .jmp = NULL,
	.flags = NULL, .next = NULL
};

struct obj static
mk_op_copy(mdl_uint_t __size, struct obj **__to, struct obj **__from) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_copy_;
	_obj.size = __size;
	_obj.to = __to;
	_obj.from = __from;
	return _obj;
}

struct obj static
mk_op_fresh(mdl_uint_t __size) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_fresh_;
	_obj.size = __size;
	return _obj;
}

struct obj static
mk_op_free(mdl_uint_t __size) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_free_;
	_obj.size = __size;
	return _obj;
}

struct obj static
mk_exit() {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_exit_;
	return _obj;
}

struct obj static
mk_op_assign(void *__p, mdl_uint_t __size, struct obj **__to) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_assign_;
	_obj.p = __p;
	_obj.size = __size;
	_obj.to = __to;
	return _obj;
}

struct obj static
mk_op_compare(struct obj *__flags, struct obj **__l, struct obj **__r) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_compare_;
	_obj.l = __l;
	_obj.r = __r;
	_obj.flags = __flags;
	return _obj;
}

struct obj static
mk_op_cond_jump(struct obj *__flags, mdl_u8_t __cond) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_cond_jump_;
	_obj.flags = __flags;
	_obj.cond = __cond;
	return _obj;
}

struct obj static
mk_op_conv(struct obj **__dst, struct obj **__src) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_conv_;
	_obj.dst = __dst;
	_obj.src = __src;
	return _obj;
}

struct obj static
mk_op_zero(struct obj **__dst) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_zero_;
	_obj.dst = __dst;
	return _obj;
}

struct obj static
mk_op_jump() {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_jump_;
	return _obj;
}

struct obj static
mk_op_push(struct obj *__obj) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_push_;
	_obj._obj = __obj;
	return _obj;
}

struct obj static mk_op_pop() {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_pop_;
	return _obj;
}

struct obj static 
mk_incr(struct obj **__obj) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_incr_;
	_obj.val = __obj;
	return _obj;
}

struct obj static
mk_decr(struct obj **__obj) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_decr_;
	_obj.val = __obj;
	return _obj;
}

struct obj static 
mk_op_frame() {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_frame_;
	return _obj;
}

struct obj static 
mk_op_call(struct obj **__no) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_call_;
	_obj.no = __no;
	return _obj;
}

struct obj static
mk_op_free_frame(struct obj *__frame) {
	struct obj _obj = obj_tmpl;
	_obj.opno = _op_free_frame_;
	_obj.frame = __frame;
	return _obj;
}

struct obj static*
push(struct ffly_compiler *__compiler, struct obj *__obj) {
	return next_obj(__compiler, mk_op_push(__obj));
}

struct obj static*
pop(struct ffly_compiler *__compiler, struct obj ***__obj) {
	struct obj *_obj = next_obj(__compiler, mk_op_pop());
	*__obj = &_obj->_obj;
	return _obj;
}

struct obj static***
call(struct ffly_compiler *__compiler, struct obj **__no) {
	return next_obj(__compiler, mk_op_call(__no))->params;
}

struct obj static*
create_frame(struct ffly_compiler *__compiler) {
	return next_obj(__compiler, mk_op_frame());
}

void static
free_frame(struct ffly_compiler *__compiler, struct obj *__frame) {
	next_obj(__compiler, mk_op_free_frame(__frame));
}

struct obj static*
__fresh(struct ffly_compiler *__compiler, mdl_uint_t __size) {
	return next_obj(__compiler, mk_op_fresh(__size));
}

void static
__free(struct ffly_compiler *__compiler, mdl_uint_t __n) {
	next_obj(__compiler, mk_op_free(__n));
}

void static*
stack_push(void *__p, mdl_uint_t __n) {
	void *p = *stack;
	(*stack)+=__n;
	ffly_mem_cpy(p, __p, __n);
	return p;
}

/*		emit	  */
void static
emit(struct ffly_compiler*, struct node*);

void static
conv(struct ffly_compiler *__compiler, mdl_u8_t __to, mdl_uint_t __size) {
	struct obj **_obj, *dst = __fresh(__compiler, __size);
	dst->_type = __to;
	pop(__compiler, &_obj);
	next_obj(__compiler, mk_op_conv(objpp(dst), _obj)); 
	push(__compiler, dst);
} 

void static
emit_conv(struct ffly_compiler *__compiler, struct node *__node) {
	emit(__compiler, __node->operand);
	conv(__compiler, convtk(__node->_type->kind), __node->_type->size);
}

void static
emit_cast(struct ffly_compiler *__compiler, struct node *__node) {
	emit(__compiler, __node->operand);
	conv(__compiler, convtk(__node->_type->kind), __node->_type->size); 
}

void static
emit_decl_init(struct ffly_compiler *__compiler, struct node *__node, struct obj *__to) {
	emit(__compiler, __node->init);
	struct obj **from;
	pop(__compiler, &from);
	next_obj(__compiler, mk_op_copy(__node->init->_type->size, objpp(__to), from));
}

void static
emit_decl(struct ffly_compiler *__compiler, struct node *__node) { 
	if (__node->var->_type->kind == _struct) {
		ffly_vec_init(&__node->var->fields, sizeof(struct obj*));
		ffly_vec_resize(&__node->var->fields, __node->var->_type->size);
		void *itr = ffly_map_begin(&__node->var->_type->fields);
		while(itr != NULL) {
			struct type *_type = (struct type*)ffly_map_getp(itr);
			*(struct obj**)ffly_vec_at(&__node->var->fields, _type->off) = __fresh(__compiler, _type->size); 
			ffly_map_itr(&__node->var->_type->fields, &itr, MAP_ITR_FD);
		}
	} else {
		struct obj *m = __fresh(__compiler, __node->var->_type->size);
		m->_type = convtk(__node->var->_type->kind);
		m->size = __node->var->_type->size;
		__node->var->_obj = m;
		if (__node->init != NULL)
			emit_decl_init(__compiler, __node, m);
	}
}

void static
emit_assign(struct ffly_compiler *__compiler, struct node *__node) {
	emit(__compiler, __node->l);
	emit(__compiler, __node->r);
	struct obj **l, **r;
	pop(__compiler, &r);
	pop(__compiler, &l);
	next_obj(__compiler, mk_op_copy(__node->_type->size, l, r));
}

void static
emit_literal(struct ffly_compiler *__compiler, struct node *__node) {
	struct obj *m = __fresh(__compiler, __node->_type->size);
	void *p = stack_push(__node->val, __node->_type->size);
	next_obj(__compiler, mk_op_assign(p, __node->_type->size, objpp(m)));
	m->_type = convtk(__node->_type->kind);
	push(__compiler, m);
}

void static
emit_print_call(struct ffly_compiler *__compiler, struct node *__node) {
	struct obj **arg;
	emit(__compiler, __node->arg);
	pop(__compiler, &arg);
	next_obj(__compiler, (struct obj){.opno=_op_print_, .p=NULL, ._type=convtk(__node->arg->_type->kind), .to=NULL, .from=NULL, .val=arg});
}

void static
emit_var(struct ffly_compiler *__compiler, struct node *__node) {
	push(__compiler, __node->_obj);    
}

void static
emit_if(struct ffly_compiler *__compiler, struct node *__node) {
	struct obj *frame = create_frame(__compiler);
	emit(__compiler, __node->cond);
	mdl_u8_t flag = 0x0;
	if (__node->cond->kind == _op_eq)
		flag = _flg_neq;
	else if (__node->cond->kind == _op_neq)
		flag = _flg_eq;
	else if (__node->cond->kind == _op_gt)
		flag = _flg_lt|_flg_eq;
	else if (__node->cond->kind == _op_lt)
		flag = _flg_gt|_flg_eq;

	struct obj *jmp = next_obj(__compiler, mk_op_cond_jump(rg_8l_u, flag));
	struct node **itr = NULL;
	___ffly_vec_nonempty(&__node->_do) {
		itr = (struct node**)ffly_vec_begin(&__node->_do);
		while(itr <= (struct node**)ffly_vec_end(&__node->_do))
			emit(__compiler, *(itr++));
	}

	struct obj **p;
	struct obj *sk = next_obj(__compiler, mk_op_jump());

	p = &end->next;
	jmp->jmp = (struct obj***)stack_push(&p, sizeof(struct obj**));

	___ffly_vec_nonempty(&__node->_else) {
		itr = (struct node**)ffly_vec_begin(&__node->_else);
		while(itr <= (struct node**)ffly_vec_end(&__node->_else))
			emit(__compiler, *(itr++));
	}

	p = &end->next;
	sk->jmp = (struct obj***)stack_push(&p, sizeof(struct obj**));
	free_frame(__compiler, frame);
}

void static
emit_while(struct ffly_compiler *__compiler, struct node *__node) {
	struct obj *frame = create_frame(__compiler);
	__compiler->frame = frame;
	struct obj **top = &end->next;
	emit(__compiler, __node->cond);
	mdl_u8_t flag = 0x0;
	if (__node->cond->kind == _op_eq)
		flag = _flg_neq;
	else if (__node->cond->kind == _op_neq)
		flag = _flg_eq;
	else if (__node->cond->kind == _op_gt)
		flag = _flg_lt|_flg_eq;
	else if (__node->cond->kind == _op_lt)
		flag = _flg_gt|_flg_eq;

	struct obj *_obj = next_obj(__compiler, mk_op_cond_jump(rg_8l_u, flag));
	struct node **itr = NULL;
	___ffly_vec_nonempty(&__node->block) {
		itr = (struct node**)ffly_vec_begin(&__node->block);
		while(itr <= (struct node**)ffly_vec_end(&__node->block))
			emit(__compiler, *(itr++));
	}

	struct obj **p;
	struct obj *jmp = next_obj(__compiler, mk_op_jump());
	p = top;
	jmp->jmp = (struct obj***)stack_push(&p, sizeof(struct obj**));
	p = &end->next;
	_obj->jmp = (struct obj***)stack_push(&p, sizeof(struct obj**));
	free_frame(__compiler, frame);

	void **brk = __compiler->brk;
	while(brk != __compiler->brkp) {
		((struct obj*)*brk)->jmp = _obj->jmp;  
		brk++;
	}
	__compiler->brkp = __compiler->brk;
}

void static
emit_binop(struct ffly_compiler *__compiler, struct node *__node) {
	emit(__compiler, __node->l);
	emit(__compiler, __node->r);
	struct obj **l, **r;
	pop(__compiler, &r);
	pop(__compiler, &l);

	mdl_u8_t op = __node->kind;
	if (op == _op_eq || op ==  _op_neq || op == _op_gt || op == _op_lt)
		next_obj(__compiler, mk_op_compare(rg_8l_u, l, r));
}

void static
emit_func(struct ffly_compiler *__compiler, struct node *__node) {
	struct obj *jmp = next_obj(__compiler, mk_op_jump());
	__node->jmp = &end->next;
	__node->pair.p0 = &end->next;
	struct obj *frame = create_frame(__compiler);
	__compiler->frame = frame;
	struct node **itr = NULL;
	___ffly_vec_nonempty(&__node->args) {
		itr = (struct node**)ffly_vec_end(&__node->args);
		while(itr >= (struct node**)ffly_vec_begin(&__node->args)) {
			emit(__compiler, *itr);
			emit(__compiler, (*itr)->var);
			struct obj **to, **from;
			pop(__compiler, &to);
			pop(__compiler, &from);
			next_obj(__compiler, mk_op_copy((*itr)->var->_type->size, to, from));
			itr--;
		}
	}

	struct obj **ret_to;
	pop(__compiler, &ret_to);
	__compiler->ret_to = ret_to;

	___ffly_vec_nonempty(&__node->block) {
		itr = (struct node**)ffly_vec_begin(&__node->block);
		while(itr <= (struct node**)ffly_vec_end(&__node->block))
			emit(__compiler, *(itr++));
	}

	free_frame(__compiler, frame);
	struct obj *ret = next_obj(__compiler, mk_op_jump());
	ret->jmp = (struct obj***)ret_to;

	struct obj **p = &end->next;
	jmp->jmp = (struct obj***)stack_push(&p, sizeof(struct obj**));
	__node->pair.p1 = *jmp->jmp; 
}

void static
emit_func_call(struct ffly_compiler *__compiler, struct node *__node) {
	struct obj *frame = create_frame(__compiler);
	struct node **param = NULL;
	struct obj *ret = push(__compiler, NULL);

	___ffly_vec_nonempty(&__node->params) {
		param = (struct node**)ffly_vec_begin(&__node->params);
		while(!vec_deadstop(param, &__node->params)) {
			emit(__compiler, *param); 
			param++;
		}
	}

	struct obj *jmp = next_obj(__compiler, mk_op_jump());

	struct obj **p = __node->call->jmp;
	jmp->jmp = (struct obj***)stack_push(&p, sizeof(struct obj**));
	ret->_obj = (struct obj*)&end->next;
	free_frame(__compiler, frame);
}

void static
emit_struct_ref(struct ffly_compiler *__compiler, struct node *__node) {
	//emit(__compiler, __node->_struct);
  //  __node->_struct->_obj->off = __node->_type->off;
	struct obj *m = *(struct obj**)ffly_vec_at(&__node->_struct->fields, __node->_type->off);
	push(__compiler, m);
}

void static
emit_exit(struct ffly_compiler *__compiler, struct node *__node) {
	next_obj(__compiler, mk_exit());
}

void static
emit_incr(struct ffly_compiler *__compiler, struct node *__node) {
	emit(__compiler, __node->operand);
	struct obj **_obj;
	pop(__compiler, &_obj);
	next_obj(__compiler, mk_incr(_obj));
}

void static
emit_decr(struct ffly_compiler *__compiler, struct node *__node) {
	emit(__compiler, __node->operand);
	struct obj **_obj;
	pop(__compiler, &_obj);
	next_obj(__compiler, mk_decr(_obj));
}

void static
emit_call(struct ffly_compiler *__compiler, struct node *__node) {
	struct obj **no;
	emit(__compiler, __node->no);
	pop(__compiler, &no);

	struct node **p;
	struct obj **params[12] = {NULL};
	mdl_uint_t i = 0;
	___ffly_vec_nonempty(&__node->params) {
		p = (struct node**)ffly_vec_begin(&__node->params);
		while(!vec_deadstop(p, &__node->params)) {
			emit(__compiler, *p);
			pop(__compiler, params+(i++));
			p++;
		}
		params[i] = NULL;
	}

	i = 0;
	struct obj ***param = call(__compiler, no);
	while((*(param++) = params[i++]) != NULL);
}

void static
emit_addrof(struct ffly_compiler *__compiler, struct node *__node) {
	void *p;
	if (__node->operand->kind == _ast_func)
		p = &__node->operand->pair;
	else
		p = __node->operand->_obj;
	p = stack_push(&p, sizeof(void*));
	next_obj(__compiler, mk_op_assign(p, rg_64l_u->size, objpp(rg_64l_u)));
	push(__compiler, rg_64l_u);
}

void static
emit_ret(struct ffly_compiler *__compiler, struct node *__node) {
	if (__node->ret != NULL) {
		emit(__compiler, __node->ret);
		struct obj *reg = get_reg(convtk(__node->_type->kind));
		struct obj **from;
		pop(__compiler, &from);

		next_obj(__compiler, mk_op_copy(__node->_type->size, objpp(reg), from));
		push(__compiler, reg);
	}

	free_frame(__compiler, (struct obj*)__compiler->frame);
	struct obj *ret = next_obj(__compiler, mk_op_jump());
	ret->jmp = (struct obj***)__compiler->ret_to;
}

void static
emit_brk(struct ffly_compiler *__compiler, struct node *__node) {
	free_frame(__compiler, (struct obj*)__compiler->frame);    
	*(__compiler->brkp++) = (void*)next_obj(__compiler, mk_op_jump());
}

void emit(struct ffly_compiler *__compiler, struct node *__node) {
	switch(__node->kind) {
		case _ast_decl:
			emit_decl(__compiler, __node);
		break;
		case _ast_literal:
			emit_literal(__compiler, __node);
		break;
		case _ast_print_call:
			emit_print_call(__compiler, __node);
		break;
		case _ast_var:
			emit_var(__compiler, __node);
		break;
		case _ast_if:
			emit_if(__compiler, __node);
		break;
		case _ast_func:
			emit_func(__compiler, __node);
		break;
		case _ast_func_call:
			emit_func_call(__compiler, __node);
		break;
		case _ast_struct_ref:
			emit_struct_ref(__compiler, __node);
		break;
		case _ast_assign:
			emit_assign(__compiler, __node);
		break;
		case _ast_exit:
			emit_exit(__compiler, __node);
		break;
		case _ast_while:
			emit_while(__compiler, __node);
		break;
		case _ast_incr:
			emit_incr(__compiler, __node);
		break;
		case _ast_decr:
			emit_decr(__compiler, __node);
		break;
		case _ast_call:
			emit_call(__compiler, __node);
		break;
		case _ast_addrof:
			emit_addrof(__compiler, __node);
		break;
		case _ast_ret:
			emit_ret(__compiler, __node);
		break;
		case _ast_conv:
			emit_conv(__compiler, __node);
		break;
		case _op_cast:
			emit_cast(__compiler, __node);
		break;
		case _ast_brk:
			emit_brk(__compiler, __node);
		break;
		default:
			emit_binop(__compiler, __node);
	}
}

ffly_err_t
ffly_gen(struct ffly_compiler *__compiler, void **__top, ffly_byte_t **__stack) {
	if (!ffly_vec_size(&__compiler->nodes)) return FFLY_FAILURE;
	stack = __stack;
	rg_8l_u = __fresh(__compiler, 1);
	rg_8l_u->_type = _8l_u;

	rg_16l_u = __fresh(__compiler, 2);
	rg_16l_u->_type = _16l_u;

	rg_32l_u = __fresh(__compiler, 4);
	rg_32l_u->_type = _32l_u;

	rg_64l_u = __fresh(__compiler, 8);
	rg_64l_u->_type = _64l_u;

	___ffly_vec_nonempty(&__compiler->nodes) {
		struct node **p = (struct node**)ffly_vec_begin(&__compiler->nodes);
		while(!vec_deadstop(p, &__compiler->nodes))
			emit(__compiler, *(p++));
	}
	*__top = (void*)top;
	retok;
}
