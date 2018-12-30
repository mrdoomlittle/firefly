# include "gen.h"
# include "memalloc.h"
# include "../ffly_def.h"
# include "../system/io.h"
ff_hs_objp static top = NULL;
ff_hs_objp static end = NULL;
# ifndef __ffly_mscarcity
# define emitnode(__n) \
	emit[(__n)->kind](__n)
# else 
# define emitnode(__n) \
	emit(__n)
# endif

static struct link *link_head = NULL;
struct link {
	struct link *next;
	ff_hs_objp *src;
	ff_hs_objp *dst;
};

void static
linkit(ff_hs_objp *__src, ff_hs_objp *__dst) {
	struct link *p;
	p = (struct link*)ff_hs_mem_alloc(sizeof(struct link));
	p->src = __src;
	p->dst = __dst;
	p->next = link_head;
	link_head = p;
}

ff_hs_objp static
next_obj(void) {
	ff_hs_objp o;
	o = ff_hs_mem_alloc(sizeof(struct ff_hs_obj));
	if (!top)
		top = o;
	if (end != NULL)
		end->next = o;
	o->next = NULL;
	end = o;
	return o;
}

ff_hs_objp static
o_fresh(ff_uint_t __sz) {
	ff_hs_objp o;
	o = next_obj();
	o->op = _op_fresh;
	o->size = __sz;
	return o;
}

ff_hs_objp static
o_call(ff_hs_objp *__to) {
	ff_hs_objp o;
	o = next_obj();
	o->op = _op_call;
	o->to = *__to;
	return o;
}

ff_hs_objp static
o_ret(void) {
	ff_hs_objp o;
	o = next_obj();
	o->op = _op_ret;
	return o;
}

ff_hs_objp static
o_jmp(void) {
	ff_hs_objp o;
	o = next_obj();
	o->op = _op_jmp;
	return o;
}

ff_hs_objp static
o_rin(void) {
	ff_hs_objp o;
	o = next_obj();
	o->op = _op_rin;
	return o;
}

ff_hs_objp static
o_push(ff_hs_objp __obj) {
	ff_hs_objp o;
	o = next_obj();
	o->op = _op_push;
	o->obj = __obj;
	return o;
}

ff_hs_objp static
o_pop(void) {
	ff_hs_objp o;
	o = next_obj();
	o->op = _op_pop;
	return o;
}

ff_hs_objp static
o_fset(ff_hs_objp *__src, ff_hs_objp *__dst, ff_hs_objp *__sz) {
	ff_hs_objp o;
	o = next_obj();
	o->op = _op_fset;
	o->src = __src;
	o->dst = __dst;
	o->sz = __sz;
	return o;
}

ff_hs_objp static
o_fget(ff_hs_objp *__src, ff_hs_objp *__dst, ff_hs_objp *__sz) {
	ff_hs_objp o;
	o = next_obj();
	o->op = _op_fget;
	o->src = __src;
	o->dst = __dst;
	o->sz = __sz;
	return o;
}

ff_hs_objp static
o_assign(void *__src, ff_hs_objp __dst, ff_uint_t __size) {
	ff_hs_objp o;
	o = next_obj();
	o->op = _op_assign;
	o->to = __dst;
	o->p = __src;
	o->offset = 0;
	o->size = __size;
}

ff_hs_objp static
o_mim(ff_hs_objp __m, ff_uint_t __offset) {
	ff_hs_objp o;
	o = next_obj();
	o->op = _op_mim;
	o->obj = __m;
	o->offset = __offset;
}

ff_hs_objp static
o_copy(ff_hs_objp *__src, ff_hs_objp *__dst, ff_uint_t __size) {
	ff_hs_objp o;
	o = next_obj();
	o->op = _op_copy;
	o->src = __src;
	o->dst = __dst;
	o->size = __size;
}

# ifndef __ffly_mscarcity
static void(*emit[])(ff_hs_nodep);
# else
void static emit(ff_hs_nodep);
# endif
void static
emit_decl(ff_hs_nodep __n) {
	ff_uint_t size;
	ff_hs_nodep var;
	var = __n->var;
	__n->var->obj = o_fresh(var->t->size);
}

void static
emit_func(ff_hs_nodep __n) {
	ff_hs_objp j;
	j = o_jmp();
	__n->start = &end->next;
	
	ff_hs_nodep *cur, *_end, n;
	cur = (ff_hs_nodep*)ff_hs_vec_front(&__n->vec);
	_end = (ff_hs_nodep*)ff_hs_vec_rear(&__n->vec);
	while(cur != _end) {
		n = *(cur++);
		emitnode(n);
	}

	linkit(&end->next, &j->to);
}

void static
emit_func_call(ff_hs_nodep __n) {
	o_call(__n->func->start);	
}

void static
emit_ret(ff_hs_nodep __n) {
	o_ret();
}

void static
emit_rin(ff_hs_nodep __n) {
	ff_hs_nodep *cur, *_end, n;
	cur = (ff_hs_nodep*)ff_hs_vec_front(&__n->params);
	_end = (ff_hs_nodep*)ff_hs_vec_rear(&__n->params);
	
	ff_hs_objp *params[20];
	ff_hs_objp **param;
	param = params;
	while(cur != _end) {
		n = *(cur++);
		emitnode(n);
		*(param++) = &o_pop()->obj;
	}

	*param = NULL;

	ff_uint_t i;
	ff_hs_objp o;
	o = o_rin();
	o->params = (ff_hs_objp**)ff_hs_mem_alloc(((param-params)+1)*sizeof(ff_hs_objp*));
	param = params;

	i = 0;
	while(*param != NULL)
		o->params[i++] = *(param++);
	o->params[i] = NULL;
}

void static
emit_var(ff_hs_nodep __n) {
	o_push(__n->obj);
}

void static
emit_literal(ff_hs_nodep __n) {
	ff_hs_objp m;
	m = o_fresh(__n->t->size);
	o_assign(&__n->val, m, __n->t->size);
	o_push(m);
}

void static
emit_struct_ref(ff_hs_nodep __n) {
	ff_hs_objp m;
	m = o_mim(__n->struc->obj, __n->t->offset);
	o_push(m);
}

void static
emit_assign(ff_hs_nodep __n) {
	ff_hs_objp *l, *r;
	emitnode(__n->l);
	emitnode(__n->r);
	r = &o_pop()->obj;
	l = &o_pop()->obj;
	o_copy(r, l, __n->t->size);
}

void static
emit_fset(ff_hs_nodep __n) {
	ff_hs_objp *l, *r, *sz;
	emitnode(__n->l);
	emitnode(__n->r);
	emitnode(__n->sz);
	sz = &o_pop()->obj;
	r = &o_pop()->obj;
	l = &o_pop()->obj;
	o_fset(l, r, sz);
}

void static
emit_fget(ff_hs_nodep __n) {
	ff_hs_objp *l, *r, *sz;
	emitnode(__n->l);
	emitnode(__n->r);
	emitnode(__n->sz);
	sz = &o_pop()->obj;
	r = &o_pop()->obj;
	l = &o_pop()->obj;
	o_fget(l, r, sz);
}

# ifndef __ffly_mscarcity
static void(*emit[])(ff_hs_nodep) = {
	emit_func,
	emit_decl,
	emit_ret,
	emit_func_call,
	emit_rin,
	emit_var,
	emit_literal,
	emit_struct_ref,
	emit_assign,
	emit_fset,
	emit_fget
};
# else
void
emit(ff_hs_nodep __n) {
	switch(__n->kind) {
		case _func:
			emit_func(__n);
		break;
		case _decl:
			emit_decl(__n);
		break;
		case __ret:
			emit_ret(__n);
		break;
		case _func_call:
			emit_func_call(__n);
		break;
		case _rin:
			emit_rin(__n);
		break;
		case _var:
			emit_var(__n);
		break;
		case _literal:
			emit_literal(__n);
		break;
		case _struct_ref:
			emit_struct_ref(__n);
		break;
		case _assign:
			emit_assign(__n);
		break;
		case _fset:
			emit_fset(__n);
		break;
		case _fget:
			emit_fget(__n);
		break;
	}
}
# endif

ff_hs_objp ff_hs_gen(ff_hs_nodep __top) {
	ff_hs_nodep cur;

	cur = __top;
	while(cur != NULL) {
# ifndef __ffly_mscarcity
		emit[cur->kind](cur);
# else
		emit(cur);
# endif
		cur = cur->next;
	}

	struct link *l;

	l = link_head;
	while(l != NULL) {
		ffly_printf("link.\n");
		*l->dst = *l->src;
		l = l->next;
	}
	return top;
}
