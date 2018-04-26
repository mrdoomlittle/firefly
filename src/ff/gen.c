# define __ffly_compiler_internal
# define __ffly_generator
# include "../compiler.h"
# include "../dep/str_len.h"
# include "../dep/mem_cpy.h"
# include "../dep/str_cpy.h"
# include "../memory/mem_free.h"
# include "../system/string.h"
void emit(ff_compilerp, struct node*);
void static
out_s(ff_compilerp __compiler, char const *__s) {
	__compiler->out(__s, ffly_str_len(__s));
}

/*
	real address = %sp-s_adr
*/
ff_uint_t static s_off;


# define s_off_dec(__by) s_off-=__by
# define s_off_inc(__by) s_off+=__by

void static
op(ff_compilerp __compiler, char const *__op, char const *__l, char const *__r, char const *__fr) {
	char buf[128];
	char *p = buf;
	p+=ffly_str_cpy(p, __op);
	if (!__l)
		goto _sk;
	*(p++) = ' ';
	p+=ffly_str_cpy(p, __l);
	if (!__r)
		goto _sk;
	*(p++) = ',';
	*(p++) = ' ';
	p+=ffly_str_cpy(p, __r);
	if (!__fr)
		goto _sk;
	*(p++) = ',';
	*(p++) = ' ';
	p+=ffly_str_cpy(p, __fr);
_sk:
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
}

void push(ff_compilerp __compiler, char const *__src, ff_u8_t __l) {
	char buf[128];
	ffly_nots(__l, buf);
	op(__compiler, "asq", "%rlx", buf, NULL);
	op(__compiler, "subq", "%sp", "%rlx", "%sp");
	op(__compiler, "ldq", "%sp", __src, NULL);
	s_off_inc(__l);
}

void pop(ff_compilerp __compiler, char const *__dst, ff_u8_t __l) {
	char buf[128];
	ffly_nots(__l, buf);
	op(__compiler, "stq", "%sp", __dst, NULL);
	op(__compiler, "asq", "%rlx", buf, NULL);
	op(__compiler, "addq", "%sp", "%rlx", "%sp");
	s_off_dec(__l);
}

void static
emit_as(ff_compilerp __compiler, struct node *__node) {
	out_s(__compiler, __node->p);
	__ffly_mem_free(__node->p);
}

void static
emit_func(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;

	ff_u8_t *flags = &__node->flags;
	if ((*flags&_func_gbl)>0)
		p+=ffly_str_cpy(p, ".globl");
	else if ((*flags&_func_exr)>0)
		p+=ffly_str_cpy(p, ".extern");
	*(p++) = ' ';
	p+=ffly_str_cpy(p, __node->p);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);

	if ((*flags&_func_def)>0) {
		p = buf;
		p+=ffly_str_cpy(p, __node->p);
		*(p++) = ':';
		*p = '\n';
		__compiler->out(buf, (p-buf)+1);

		push(__compiler, "%bp", 8);
		out_s(__compiler, "movq %sp, %bp\n");
		s_off = 0;

		ff_uint_t mass = 0;
		struct node **itr;
		___ffly_vec_nonempty(&__node->var_pond) {
			itr = (struct node**)ffly_vec_begin(&__node->var_pond);
			while(itr <= (struct node**)ffly_vec_end(&__node->var_pond))
				mass+=(*(itr++))->_type->size;
		}

		ffly_vec_de_init(&__node->var_pond);
		if (mass>0) {
			ffly_nots(mass, buf);
			op(__compiler, "asq", "%rlx", buf, NULL);
			op(__compiler, "subq", "%sp", "%rlx", "%sp");
		}

		ffly_vecp block = __node->_block;
		if (block != NULL) {
			struct node **itr;
			___ffly_vec_nonempty(block) {
				itr = (struct node**)ffly_vec_beg(block);
				while(itr <= (struct node**)ffly_vec_end(block))
					emit(__compiler, *(itr++));
			}
			ffly_vec_de_init(block);
			__ffly_mem_free(block);
		}
		out_s(__compiler, "movq %bp, %sp\n");
		pop(__compiler, "%bp", 8);
		out_s(__compiler, "ret\n");
	}
}

void static
emit_ret(ff_compilerp __compiler, struct node *__node) {

}

void static
emit_func_call(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;

	p+=ffly_str_cpy(p, "call");
	*(p++) = ' ';
	*(p++) = '$';
	p+=ffly_str_cpy(p, __node->p);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
}


void emit_load(ff_compilerp __compiler, ff_uint_t __off, ff_u8_t __l) {	
	char buf[128];
	ffly_nots(__off, buf);
	op(__compiler, "asq", "%rlx", buf, NULL);
	op(__compiler, "subq", "%bp", "%rlx", "%rlx");
	switch(__l) {
		case 1:
			op(__compiler, "ldb", "%rlx", "%al", NULL);
		break;
		case 2:
			op(__compiler, "ldw", "%rlx", "%ax", NULL);
		break;
		case 4:
			op(__compiler, "ldd", "%rlx", "%eax", NULL);
		break;
		case 8:
			op(__compiler, "ldq", "%rlx", "%rax", NULL);
		break;
	}
}

void emit_decl_init(ff_compilerp __compiler, struct node *__node, ff_uint_t __off) {
	emit(__compiler, __node);
	emit_load(__compiler, __off, __node->_type->size);
}

void emit_decl(ff_compilerp __compiler, struct node *__node) {
	s_off_inc(__node->var->_type->size);
	__node->var->s_off = s_off;
	if (__node->init != NULL)
		emit_decl_init(__compiler, __node->init, s_off);
}

void emit_assign(ff_compilerp __compiler, struct node *__node) {
	emit(__compiler, __node->r);
	emit_load(__compiler, __node->l->s_off, __node->l->_type->size);
}

void emit_literal(ff_compilerp __compiler, struct node *__node) {
	switch(__node->_type->size) {
		case 1:
			op(__compiler, "asb", "%al", __node->p, NULL);
		break;
		case 2:
			op(__compiler, "asw", "%ax", __node->p, NULL);
		break;
		case 4:
			op(__compiler, "asd", "%eax", __node->p, NULL);
		break;
		case 8:
			op(__compiler, "asq", "%rax", __node->p, NULL);
		break;
	}
}

void emit_out(ff_compilerp __compiler, struct node *__node) {
	emit(__compiler, __node->var);
	switch(__node->var->_type->size) {
		case 1:
			op(__compiler, "outb", "%al", __node->p, NULL);
		break;
		case 2:
			op(__compiler, "outw", "%ax", __node->p, NULL);
		break;
		case 4:
			op(__compiler, "outd", "%eax", __node->p, NULL);
		break;
		case 8:
			op(__compiler, "outq", "%rax", __node->p, NULL);
		break;
	}
}

void emit_var(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	ffly_nots(__node->s_off, buf);
	op(__compiler, "asq", "%rlx", buf, NULL);
	op(__compiler, "subq", "%bp", "%rlx", "%rlx");
	switch(__node->_type->size) {
		case 1:
			op(__compiler, "stb", "%rlx", "%al", NULL);
		break;
		case 2:
			op(__compiler, "stw", "%rlx", "%ax", NULL);
		break;
		case 4:
			op(__compiler, "std", "%rlx", "%eax", NULL);
		break;
		case 8:
			op(__compiler, "stq", "%rlx", "%rax", NULL);
		break;
	}
}

void emit(ff_compilerp __compiler, struct node *__node) {
	switch(__node->kind) {
		case _ast_var:
			emit_var(__compiler, __node);
		break;
		case _ast_func:
			emit_func(__compiler, __node);
		break;
		case _ast_as:
			emit_as(__compiler, __node);
		break;
		case _ast_ret:
			emit_ret(__compiler, __node);
		break;
		case _ast_func_call:
			emit_func_call(__compiler, __node);
		break;
		case _ast_decl:
			emit_decl(__compiler, __node);
		break;
		case _ast_assign:
			emit_assign(__compiler, __node);
		break;
		case _ast_literal:
			emit_literal(__compiler, __node);
		break;
		case _ast_out:
			emit_out(__compiler, __node);
		break;
	}
}


ff_err_t
ffly_ff_gen(ff_compilerp __compiler) {
	if (!ffly_vec_size(&__compiler->nodes)) return FFLY_FAILURE;

	out_s(__compiler, ".region text\n");
	___ffly_vec_nonempty(&__compiler->nodes) {
		struct node **p = (struct node**)ffly_vec_begin(&__compiler->nodes);
		while(!vec_deadstop(p, &__compiler->nodes))
			emit(__compiler, *(p++));
	}
	out_s(__compiler, ".endof\n");
	retok;
}
