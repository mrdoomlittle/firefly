# include "dus.h"
# include "../ffly_def.h"
# include "../stdio.h"
# include "../malloc.h"
void static emit(dus_nodep);

dus_objp static top = NULL;
dus_objp static end = NULL;
dus_objp static
next_obj(void) {
	dus_objp o = ff_dus_obj_alloc();
	if (!top)
		top = o;
	if (end != NULL)
		end->next = o;
	o->next = NULL;
	end = o;
	return o;
}

void static
op_copy(dus_objp *__src, dus_objp *__dst, ff_uint_t __len) {
	dus_objp o = next_obj();
	o->op = _dus_op_copy;
	o->dst = __dst;
	o->src = __src;
	o->len = __len;
}

void static
op_assign(dus_objp __to, ff_u8_t *__from, ff_uint_t __len) {
	dus_objp o = next_obj();
	o->op = _dus_op_assign;
	o->to = __to;
	o->p = __from;
	o->len = __len;
}

dus_objp static
op_fresh(ff_uint_t __size) {
	dus_objp o = next_obj();
	o->op = _dus_op_fresh;
	o->size = __size;
	return o;
}

void static
op_free(dus_objp __obj) {
	dus_objp o = next_obj();
	o->op = _dus_op_free;
}

void static
op_push(dus_objp __obj) {
	dus_objp o = next_obj();
	o->op = _dus_op_push;
	o->p = __obj;
}

void static
op_pop(dus_objp **__obj) {
	dus_objp o = next_obj();
	o->op = _dus_op_pop;
	*__obj = (dus_objp*)&o->p;
}

void static
op_out(dus_objp *__p) {
	dus_objp o = next_obj();
	o->op = _dus_op_out;
	o->p = __p;
}

void static
op_cas(dus_objp *__obj, dus_objp __m) {
	dus_objp o = next_obj();
	o->op = _dus_op_cas;
	o->p = __obj;
	o->dst = __m;
}

void static
op_set(dus_objp *__src, dus_objp *__dst) {
	dus_objp o = next_obj();
	o->op = _dus_op_set;
	o->src = __src;
	o->dst = __dst;
}

void static
op_syput(char *__name, ff_uint_t __len, dus_objp __obj) {
	dus_objp o = next_obj();
	o->op = _dus_op_syput;
	o->len = __len;
	o->p = __obj;
	o->name = __name;
}

void static
op_shell(dus_objp *__obj) {
	dus_objp o = next_obj();
	o->op = _dus_op_shell;
	o->p = __obj;
}

void push(dus_objp __obj) {
	op_push(__obj);
}

dus_objp* pop(void) {
	dus_objp *ret;
	op_pop(&ret);
	return ret;
}

dus_objp static*
dus_objpp(dus_objp  __obj) {
	__obj->objpp = __obj;
	return &__obj->objpp;
}

void static
emit_decl_init(dus_nodep __node, dus_objp __to) {
	emit(__node);
	dus_objp *src;
	src = pop();
	op_copy(src, dus_objpp(__to), __node->len);
}

void static
emit_decl(dus_nodep __node) {
	dus_nodep init = __node->init;
	dus_nodep var = __node->var;
	dus_objp m;
	if (init != NULL) {
		m = op_fresh(init->len);
		emit_decl_init(init, m);
	} else
		m = ff_dus_obj_alloc();
	var->_obj = m;
}

void static
emit_var(dus_nodep __node) {
	push(__node->_obj);
}

void static
emit_out(dus_nodep __node) {
	emit((dus_nodep)__node->p);
	dus_objp *o;
	o = pop();
	op_out(o);
}

void static
emit_assign(dus_nodep __node) {
	dus_nodep r = __node->r;
	emit(r);
	dus_objp *src;
	src = pop();
	ff_uint_t len;
	dus_objp m = op_fresh(len = r->_obj->size);
	op_copy(src, dus_objpp(m), len);
	__node->l->_obj = m;
}

void static
emit_cas(dus_nodep __node) {
	dus_objp *o;
	emit((dus_nodep)__node->p);
	o = pop();
	dus_objp m = ff_dus_obj_alloc(); 
	op_cas(o, m);
	push(m);
	__node->_obj = m;
}

void static 
emit_syput(dus_nodep __node) {
	op_syput(__node->name, __node->len, ((dus_nodep)__node->p)->_obj);	
}

void static
emit_shell(dus_nodep __node) {
	emit((dus_nodep)__node->p);
	dus_objp *o;
	o = pop();
	op_shell(o);
}

void static
emit_str(dus_nodep __node) {
	dus_objp m = op_fresh(__node->len);
	op_assign(m, __node->p, __node->len);
	push(m);
	__node->_obj = m;
}

char const *nkstr(ff_u8_t __kind) {
	switch(__kind) {
		case _dus_str:		return "str";
		case _dus_decl:		return "decl";
		case _dus_var:		return "var";
		case _dus_assign:	return "assign";
		case _dus_out:		return "out";
		case _dus_cas:		return "cas";
		case _dus_syput:	return "syput";
		case _dus_shell:	return "shell";
	}
	return "unknown";
}


void static
emit_set(dus_nodep __node) {
	dus_nodep l = __node->l;
	dus_nodep r = __node->r;
	emit(r);
	emit(l);

	dus_objp *src;
	dus_objp *dst;
	dst = pop();
	src = pop();
	op_set(src, dst);
}

void
emit(dus_nodep __node) {
	switch(__node->kind) {
		case _dus_set:
			emit_set(__node);
		break;
		case _dus_str:
			emit_str(__node);
		break;
		case _dus_decl:
			emit_decl(__node);
		break;
		case _dus_var:
			emit_var(__node);
		break;
		case _dus_assign:
			emit_assign(__node);
		break;
		case _dus_out:
			emit_out(__node);
		break;
		case _dus_cas:
			emit_cas(__node);
		break;
		case _dus_syput:
			emit_syput(__node);
		break;
		case _dus_shell:
			emit_shell(__node);
		break;
	}
	//printf("%s\n", nkstr(__node->kind));
}

dus_objp ff_dus_gen(dus_nodep __top) {
	dus_nodep cur = __top;
	while(cur != NULL) {
		emit(cur);
		cur = cur->next;
	}
	return top;
}
