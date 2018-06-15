# include "dus.h"
# include "../ffly_def.h"
# include "../stdio.h"
# include "../malloc.h"
void static emit(nodep);

objp static top = NULL;
objp static end = NULL;
objp static
next_obj(void) {
	objp o = ff_dus_obj_alloc();
	if (!top)
		top = o;
	if (end != NULL)
		end->next = o;
	o->next = NULL;
	end = o;
	return o;
}

void static
op_copy(objp *__src, objp *__dst, ff_uint_t __len) {
	objp o = next_obj();
	o->op = _op_copy;
	o->dst = __dst;
	o->src = __src;
	o->len = __len;
}

void static
op_assign(objp __to, ff_u8_t *__from, ff_uint_t __len) {
	objp o = next_obj();
	o->op = _op_assign;
	o->to = __to;
	o->p = __from;
	o->len = __len;
}

objp static
op_fresh(ff_uint_t __size) {
	objp o = next_obj();
	o->op = _op_fresh;
	o->size = __size;
	return o;
}

void static
op_free(objp __obj) {
	objp o = next_obj();
	o->op = _op_free;
}

void static
op_push(objp __obj) {
	objp o = next_obj();
	o->op = _op_push;
	o->p = __obj;
}

void static
op_pop(objp **__obj) {
	objp o = next_obj();
	o->op = _op_pop;
	*__obj = (objp*)&o->p;
}

void static
op_out(objp *__p) {
	objp o = next_obj();
	o->op = _op_out;
	o->p = __p;
}

void static
op_cas(objp *__obj, objp __m) {
	objp o = next_obj();
	o->op = _op_cas;
	o->p = __obj;
	o->dst = __m;
}

void static
op_set(objp *__src, objp *__dst) {
	objp o = next_obj();
	o->op = _op_set;
	o->src = __src;
	o->dst = __dst;
}

void static
op_syput(char *__name, ff_uint_t __len, objp __obj) {
	objp o = next_obj();
	o->op = _op_syput;
	o->len = __len;
	o->p = __obj;
	o->name = __name;
}

void static
op_shell(objp *__obj) {
	objp o = next_obj();
	o->op = _op_shell;
	o->p = __obj;
}

void push(objp __obj) {
	op_push(__obj);
}

objp* pop(void) {
	objp *ret;
	op_pop(&ret);
	return ret;
}

objp static*
objpp(objp  __obj) {
	__obj->objpp = __obj;
	return &__obj->objpp;
}

void static
emit_decl_init(nodep __node, objp __to) {
	emit(__node);
	objp *src;
	src = pop();
	op_copy(src, objpp(__to), __node->len);
}

void static
emit_decl(nodep __node) {
	nodep init = __node->init;
	nodep var = __node->var;
	objp m = op_fresh(init->len);
	emit_decl_init(init, m);
	var->_obj = m;
}

void static
emit_var(nodep __node) {
	push(__node->_obj);
}

void static
emit_out(nodep __node) {
	emit((nodep)__node->p);
	objp *o;
	o = pop();
	op_out(o);
}

void static
emit_assign(nodep __node) {
	nodep r = __node->r;
	emit(r);
	objp *src;
	src = pop();
	ff_uint_t len;
	objp m = op_fresh(len = r->_obj->size);
	op_copy(src, objpp(m), len);
	__node->l->_obj = m;
}

void static
emit_cas(nodep __node) {
	objp *o;
	emit((nodep)__node->p);
	o = pop();
	objp m = ff_dus_obj_alloc(); 
	op_cas(o, m);
	push(m);
	__node->_obj = m;
}

void static 
emit_syput(nodep __node) {
	op_syput(__node->name, __node->len, ((nodep)__node->p)->_obj);	
}

void static
emit_shell(nodep __node) {
	emit((nodep)__node->p);
	objp *o;
	o = pop();
	op_shell(o);
}

void static
emit_str(nodep __node) {
	objp m = op_fresh(__node->len);
	op_assign(m, __node->p, __node->len);
	push(m);
	__node->_obj = m;
}

char const *nkstr(ff_u8_t __kind) {
	switch(__kind) {
		case _str:		return "str";
		case _decl:		return "decl";
		case _var:		return "var";
		case _assign:	return "assign";
		case _out:		return "out";
		case _cas:		return "cas";
		case _syput:	return "syput";
		case _shell:	return "shell";
	}
	return "unknown";
}


void static
emit_set(nodep __node) {
	nodep l = __node->l;
	nodep r = __node->r;
	emit(r);
	emit(l);

	objp *src;
	objp *dst;
	dst = pop();
	src = pop();
	op_set(src, dst);
}

void
emit(nodep __node) {
	switch(__node->kind) {
		case _set:
			emit_set(__node);
		break;
		case _str:
			emit_str(__node);
		break;
		case _decl:
			emit_decl(__node);
		break;
		case _var:
			emit_var(__node);
		break;
		case _assign:
			emit_assign(__node);
		break;
		case _out:
			emit_out(__node);
		break;
		case _cas:
			emit_cas(__node);
		break;
		case _syput:
			emit_syput(__node);
		break;
		case _shell:
			emit_shell(__node);
		break;
	}
	//printf("%s\n", nkstr(__node->kind));
}

objp ff_dus_gen(nodep __top) {
	nodep cur = __top;
	while(cur != NULL) {
		emit(cur);
		cur = cur->next;
	}
	return top;
}
