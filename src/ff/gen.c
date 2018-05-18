# define __ffly_compiler_internal
# define __ffly_generator
# include "../compiler.h"
# include "../dep/str_len.h"
# include "../dep/mem_cpy.h"
# include "../dep/str_cpy.h"
# include "../memory/mem_free.h"
# include "../system/string.h"
# include "../dep/mem_dup.h"
# define align_to(__no, __to)(((__no)+((__to)-1))&((~(__to))+1))
void emit(ff_compilerp, struct node*);
void emit_load(ff_compilerp, ff_int_t, ff_u8_t);
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

ff_uint_t label_id = 0;
char* label() {
	char buf[64];
	char *p = buf;
	*(p++) = 'l';
	p+=ffly_nots(label_id++, p);
	ffly_mem_dup((void**)&p, buf, (p-buf)+1);
	return p;
}

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
	if (__node->input != NULL) {
		while(*(--__node->input) != NULL) {
			struct node *nod = (struct node*)*(__node->input--);
			char reg[16];

			*reg = '%';
			ffly_str_cpy(reg+1, (char*)*__node->input);

			emit(__compiler, nod);
			switch(nod->_type->size) {
				case 1:
					op(__compiler, "movb", "%ea", reg, NULL);
				break;
				case 2:
					op(__compiler, "movw", "%el", reg, NULL);
				break;
				case 4:
					op(__compiler, "movd", "%ael", reg, NULL);
				break;
				case 8:
					op(__compiler, "movq", "%rel", reg, NULL);
				break;
			}
		}
		__ffly_mem_free(__node->input);
	}
	out_s(__compiler, __node->p);
	__ffly_mem_free(__node->p);
}

char *func_end;
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

		out_s(__compiler, ";save %bp\n");
		push(__compiler, "%bp", 8);
		out_s(__compiler, "movq %sp, %bp\n");
		s_off = 0;

		ff_uint_t mass = 0;
		ffly_vecp var_pond = &__node->var_pond;
		struct node **itr;
		___ffly_vec_nonempty(var_pond) {
			itr = (struct node**)ffly_vec_begin(var_pond);
			while(itr <= (struct node**)ffly_vec_end(var_pond))
				mass+=(*(itr++))->_type->size;
		}

		ffly_vec_de_init(var_pond);

		ffly_vecp params = &__node->params;
		___ffly_vec_nonempty(params) {
			itr = (struct node**)ffly_vec_beg(params);
			while(itr <= (struct node**)ffly_vec_end(params)) {
				mass+=8;
				s_off_inc(8-((*itr)->_type->size));
				s_off_inc((*itr)->_type->size);
				(*itr)->s_off = s_off;
				itr++;
			}
		}
		
		ffly_vec_de_init(params);

		if (mass>0) {
			ffly_nots(mass, buf);
			op(__compiler, "asq", "%rlx", buf, NULL);
			op(__compiler, "subq", "%sp", "%rlx", "%sp");
		}
		
		func_end = label();
		ffly_vecp block = &__node->block;
		___ffly_vec_nonempty(block) {
			itr = (struct node**)ffly_vec_beg(block);
			while(itr <= (struct node**)ffly_vec_end(block))
				emit(__compiler, *(itr++));
		}

		ffly_vec_de_init(block);	

		p = buf;
		p+=ffly_str_cpy(p, ".l");
		*(p++) = ' ';
		p+=ffly_str_cpy(p, func_end);
		*p = '\n';
		__compiler->out(buf, (p-buf)+1);

		out_s(__compiler, "movq %bp, %sp\n");
		out_s(__compiler, ";reset %bp\n");
		pop(__compiler, "%bp", 8);
		out_s(__compiler, "ret\n");

		__ffly_mem_free(func_end);
	} else
		ffly_vec_de_init(&__node->params);
}

void static
emit_ret(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;
	emit(__compiler, __node->rtv);
	p+=ffly_str_cpy(p, "jmp");
	*(p++) = ' ';
	*(p++) = '#';
	p+=ffly_str_cpy(p, func_end);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
}

void static
emit_func_call(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;

	ffly_vecp args = &__node->args;
	struct node **itr;
	ff_uint_t off = s_off+8;
	___ffly_vec_nonempty(args) {
		itr = (struct node**)ffly_vec_beg(args);
		while(itr <= (struct node**)ffly_vec_end(args)) {
			struct node *nod = *itr;
			emit(__compiler, *(itr++));
			off+=8;
			emit_load(__compiler, off, nod->_type->size);
		}
	}
	
	ffly_vec_de_init(args);

	p+=ffly_str_cpy(p, "call");
	*(p++) = ' ';
	*(p++) = '$';
	p+=ffly_str_cpy(p, __node->p);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
}

void emit_load(ff_compilerp __compiler, ff_int_t __off, ff_u8_t __l) {	
	char buf[128];
	ff_i8_t neg;
	if ((neg = __off<0))
		__off = -__off;
	ffly_nots(__off, buf);
	op(__compiler, "asq", "%rlx", buf, NULL);
	if (neg)
		op(__compiler, "addq", "%bp", "%rlx", "%rlx");
	else
		op(__compiler, "subq", "%bp", "%rlx", "%rlx");

	switch(__l) {
		case 1:
			op(__compiler, "ldb", "%rlx", "%ae", NULL);
		break;
		case 2:
			op(__compiler, "ldw", "%rlx", "%el", NULL);
		break;
		case 4:
			op(__compiler, "ldd", "%rlx", "%ael", NULL);
		break;
		case 8:
			op(__compiler, "ldq", "%rlx", "%rel", NULL);
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
	struct node *l = __node->l;
	if (l->kind == _ast_deref) {
		push(__compiler, "%rel", 8);
		emit(__compiler, l->operand);
		out_s(__compiler, "movq %rel, %xes\n");
		pop(__compiler, "%rel", 8);

		switch(l->_type->size) {
			case 1:
				out_s(__compiler, "ldb %xes, %ea\n");
			break;
			case 2:
				out_s(__compiler, "ldw %xes, %el\n");
			break;
			case 4:
				out_s(__compiler, "ldd %xes, %ael\n");
			break;
			case 8:
				out_s(__compiler, "ldq %xes, %rel\n");
			break;
		}
	} else if (l->kind == _ast_struct_ref) {
		char buf[128];

		ffly_nots(l->_struct->s_off-l->_type->off, buf);
		op(__compiler, "asq", "%rlx", buf, NULL);
		op(__compiler, "subq", "%bp", "%rlx", "%rlx");

		switch(l->_type->size) {
			case 1:
				out_s(__compiler, "ldb %rlx, %ae\n");
			break;
			case 2:
				out_s(__compiler, "ldw %rlx, %el\n");
			break;
			case 4:
				out_s(__compiler, "ldd %rlx, %ael\n");
			break;
			case 8:
				out_s(__compiler, "ldq %rlx, %rel\n");
			break;
		}
	} else
		emit_load(__compiler, l->s_off, l->_type->size);
}

void emit_literal(ff_compilerp __compiler, struct node *__node) {
	out_s(__compiler, "asq %rel, 0\n");
	switch(__node->_type->size) {
		case 1:
			op(__compiler, "asb", "%ae", __node->p, NULL);
		break;
		case 2:
			op(__compiler, "asw", "%el", __node->p, NULL);
		break;
		case 4:
			op(__compiler, "asd", "%ael", __node->p, NULL);
		break;
		case 8:
			op(__compiler, "asq", "%rel", __node->p, NULL);
		break;
	}
}

void emit_out(ff_compilerp __compiler, struct node *__node) {
	emit(__compiler, __node->var);
	switch(__node->var->_type->size) {
		case 1:
			out_s(__compiler, "outb %ae\n");
		break;
		case 2:
			out_s(__compiler, "outw %el\n");
		break;
		case 4:
			out_s(__compiler, "outd %ael\n");
		break;
		case 8:
			out_s(__compiler, "outq %rel\n");
		break;
	}
}

void emit_var(ff_compilerp __compiler, struct node *__node) {
	out_s(__compiler, "asq %rel, 0\n");
	char buf[128];
	ffly_nots(__node->s_off, buf);
	op(__compiler, "asq", "%rlx", buf, NULL);

	if (__node->_type->kind == _array) {
		op(__compiler, "subq", "%bp", "%rlx", "%rel");
		return;
	} 

	op(__compiler, "subq", "%bp", "%rlx", "%rlx");
	switch(__node->_type->size) {
		case 1:
			op(__compiler, "stb", "%rlx", "%ae", NULL);
		break;
		case 2:
			op(__compiler, "stw", "%rlx", "%el", NULL);
		break;
		case 4:
			op(__compiler, "std", "%rlx", "%ael", NULL);
		break;
		case 8:
			op(__compiler, "stq", "%rlx", "%rel", NULL);
		break;
	}
}

void emit_exit(ff_compilerp __compiler, struct node *__node) {
	emit(__compiler, __node->code);
	out_s(__compiler, "exit %ae\n");
}

void emit_conv(ff_compilerp __compiler, struct node *__node) {
	out_s(__compiler, "asq %rel, 0\n");
	emit(__compiler, __node->operand);
}

void emit_label(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;
	p+=ffly_str_cpy(p, ".l");
	*(p++) = ' ';
	p+=ffly_str_cpy(p, __node->p);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
}

void emit_jmpto(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;
	p+=ffly_str_cpy(p, "jmp");
	*(p++) = ' ';
	*(p++) = '#';
	p+=ffly_str_cpy(p, __node->p);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
}

void emit_if(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;
	
	emit(__compiler, __node->cond);
	char *end = label();
	char const *i;
	if (__node->cond->kind == _op_eq)
		i = "jne";
	else if (__node->cond->kind == _op_neq)
		i = "je";

	p+=ffly_str_cpy(p, i);

	*(p++) = ' ';
	*(p++) = '#';
	p+=ffly_str_cpy(p, end);
	*(p++) = ',';
	*(p++) = ' ';
	p+=ffly_str_cpy(p, "%ll");
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);

 	ffly_vecp block = &__node->block;
	struct node **itr;
	___ffly_vec_nonempty(block) {
		itr = (struct node**)ffly_vec_beg(block);
		while(itr <= (struct node**)ffly_vec_end(block)) {
			emit(__compiler, *(itr++));
		}
	}

	ffly_vec_de_init(block);

	p = buf;
	p+=ffly_str_cpy(p, ".l");
	*(p++) = ' ';
	p+=ffly_str_cpy(p, end);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
	__ffly_mem_free(end);
}

void emit_addrof(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	ffly_nots(__node->operand->s_off, buf);
	op(__compiler, "asq", "%rel", buf, NULL);
	op(__compiler, "subq", "%bp", "%rel", "%rel");
}

void emit_binop(ff_compilerp __compiler, struct node *__node) {
	out_s(__compiler, "asq %rel, 0\n");
	emit(__compiler, __node->l);
	push(__compiler, "%rel", 8);
	out_s(__compiler, "asq %rel, 0\n");
	emit(__compiler, __node->r);
	out_s(__compiler, "movq %rel, %xes\n");
	pop(__compiler, "%rel", 8);

	ff_u8_t op = __node->kind;
	if (op == _op_eq || op == _op_neq)
		out_s(__compiler, "cmpq %rel, %xes, %ll\n");
	else {
		switch(op) {
			case _op_add:
				out_s(__compiler, "addq %rel, %xes, %rel\n");
			break;
			case _op_sub:
				out_s(__compiler, "subq %rel, %xes, %rel\n");
			break;
			default:
				ffly_printf("unknown op, got: %u\n", op);
		}
	}
}

void emit_deref(ff_compilerp __compiler, struct node *__node) {
	emit(__compiler, __node->operand);
	switch(__node->_type->size) {
		case 1:
			out_s(__compiler, "stb %rel, %ae\n");
		break;
		case 2:
			out_s(__compiler, "stw %rel, %el\n");
		break;
		case 4:
			out_s(__compiler, "std %rel, %ael\n");
		break;
		case 8:
			out_s(__compiler, "stq %rel, %rel\n");
		break;
	}
}

void emit_struct_ref(ff_compilerp __compiler, struct node *__node) {
	out_s(__compiler, "asq %rel, 0\n");
	char buf[128];
	ffly_nots(__node->_struct->s_off-__node->_type->off, buf);
	op(__compiler, "asq", "%rlx", buf, NULL);
	op(__compiler, "subq", "%bp", "%rlx", "%rlx");
	switch(__node->_type->size) {
		case 1:
			out_s(__compiler, "stb %rlx, %ae\n");
		break;
		case 2:
			out_s(__compiler, "stw %rlx, %el\n");
		break;
		case 4:
			out_s(__compiler, "std %rlx, %ael\n");
		break;
		case 8:
			out_s(__compiler, "stq %rlx, %rel\n");
		break;
	}
}

void emit(ff_compilerp __compiler, struct node *__node) {
	switch(__node->kind) {
		case _ast_if:
			emit_if(__compiler, __node);
		break;
		case _ast_label:
			emit_label(__compiler, __node);
		break;
		case _ast_jmpto:
			emit_jmpto(__compiler, __node);
		break;
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
		case _ast_exit:
			emit_exit(__compiler, __node);
		break;
		case _ast_conv:
			emit_conv(__compiler, __node);
		break;
		case _ast_addrof:
			emit_addrof(__compiler, __node);
		break;
		case _ast_deref:
			emit_deref(__compiler, __node);
		break;
		case _ast_struct_ref:
			emit_struct_ref(__compiler, __node);
		break;
		default:
			emit_binop(__compiler, __node);
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
