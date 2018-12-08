# define __ffly_compiler_internal
# define __ffly_generator
# include "../../compiler.h"
# include "../../dep/str_len.h"
# include "../../dep/mem_cpy.h"
# include "../../dep/str_cpy.h"
# include "../../memory/mem_free.h"
# include "../../system/string.h"
# include "../../dep/mem_dup.h"
#define align_to(__no, __to)(((__no)+((__to)-1))&((~(__to))+1))
void static emit(ff_compilerp, struct node*);
void static emit_load(ff_compilerp, ff_int_t, ff_u8_t);
void static
out_s(ff_compilerp __compiler, char const *__s) {
	__compiler->out(__s, ffly_str_len(__s));
}

void static
out_op_s(ff_compilerp __compiler, char const *__s) {
	char t = '\t';
	__compiler->out(&t, 1);
	__compiler->out(__s, ffly_str_len(__s));
}

/*
	real address = %sp-s_adr
*/
ff_uint_t static s_off;

char const static pf[] = {
	'b', 'w', 'd', 'q'
};

char const static *_reg[][4] = {
	{"%ae", "%el", "%ael", "%rel"}
};

# define _r_ae_el_ael_rel 0

# define pbsz(__sz) \
	(pf[((__sz)>>1)-((__sz)>>3)])
# define rbsz(__sz, __rl) \
	(_reg[__rl][((__sz)>>1)-((__sz)>>3)])

# define s_off_dec(__by) s_off-=__by
# define s_off_inc(__by) s_off+=__by

ff_uint_t static label_id = 0;
char static* label() {
	char buf[64];
	char *p = buf;
	*(p++) = 'l';
	p+=ffly_nots(label_id++, p);
	ffly_mem_dup((void**)&p, buf, (p-buf)+1);
	return p;
}

void static
op(ff_compilerp __compiler, char const *__op, char __prefix, char const *__l, char const *__r, char const *__fr) {
	char buf[128];
	char *p = buf;
	*(p++) = '\t';

	p+=ffly_str_cpy(p, __op);
	if (!__l)
		goto _sk;
	*(p++) = __prefix;
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

void static
push(ff_compilerp __compiler, char const *__src, ff_u8_t __l) {
	char buf[128];
	ffly_nots(__l, buf);
	out_s(__compiler, "\t;push start\n");
	op(__compiler, "as", 'q', "%rlx", buf, NULL);
	op(__compiler, "sub", 'q', "%rlx", "%sp", NULL);
	op(__compiler, "ld", 'q', "%sp", __src, NULL);
	out_s(__compiler, "\t;push end\n");
	s_off_inc(__l);
}

void static
pop(ff_compilerp __compiler, char const *__dst, ff_u8_t __l) {
	char buf[128];
	ffly_nots(__l, buf);
	out_s(__compiler, "\t;pop start\n");
	op(__compiler, "st", 'q', "%sp", __dst, NULL);
	op(__compiler, "as", 'q', "%rlx", buf, NULL);
	op(__compiler, "add", 'q', "%rlx", "%sp", NULL);
	out_s(__compiler, "\t;pop end\n");
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
			ff_u8_t sz;

			sz = nod->_type->size;
			op(__compiler, "mov", pbsz(sz), rbsz(sz, _r_ae_el_ael_rel), reg, NULL);
		}
		__ffly_mem_free(__node->input);
	}
	out_s(__compiler, __node->p);
	__ffly_mem_free(__node->p);
}

char static *func_end;
void static
emit_func(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;

	ff_u8_t *flags = &__node->flags;
	if ((*flags&_func_gbl)>0)
		p+=ffly_str_cpy(p, "\t.globl");
	else if ((*flags&_func_exr)>0)
		p+=ffly_str_cpy(p, "\t.extern");
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

		out_s(__compiler, "\t;save %bp\n");
		push(__compiler, "%bp", 8);
		out_op_s(__compiler, "movq %sp, %bp\n");
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
			out_s(__compiler, "\t;place %sp at end\n");
			op(__compiler, "as", 'q', "%rlx", buf, NULL);
			op(__compiler, "sub", 'q',"%rlx", "%sp", NULL);
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

		out_op_s(__compiler, "movq %bp, %sp\n");
		out_s(__compiler, "\t;reset %bp\n");
		pop(__compiler, "%bp", 8);
		out_op_s(__compiler, "ret\n");

		__ffly_mem_free(func_end);
	} else
		ffly_vec_de_init(&__node->params);
}

void static
emit_ret(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;
	*(p++) = '\t';
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

	*(p++) = '\t';
	p+=ffly_str_cpy(p, "call");
	*(p++) = ' ';
	*(p++) = '$';
	p+=ffly_str_cpy(p, __node->p);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
}

void emit_load(ff_compilerp __compiler, ff_int_t __off, ff_u8_t __l) {	
	out_s(__compiler, "\t;load start\n");
	char buf[128];
	ff_i8_t neg;
	if ((neg = __off<0))
		__off = -__off;
	ffly_nots(__off, buf);
	op(__compiler, "as", 'q', "%xes", buf, NULL);
	out_op_s(__compiler, "movq %bp, %rlx\n");
	if (neg)
		op(__compiler, "add", 'q', "%xes", "%rlx", NULL);
	else
		op(__compiler, "sub", 'q', "%xes", "%rlx", NULL);

	op(__compiler, "ld", pbsz(__l), "%rlx", rbsz(__l, _r_ae_el_ael_rel), NULL);
	out_s(__compiler, "\t;load end\n");
}

void static emit_decl_init(ff_compilerp __compiler, struct node *__node, ff_uint_t __off) {
	emit(__compiler, __node);
	emit_load(__compiler, __off, __node->_type->size);
}

void static emit_decl(ff_compilerp __compiler, struct node *__node) {
	s_off_inc(__node->var->_type->size);
	__node->var->s_off = s_off;
	if (__node->init != NULL)
		emit_decl_init(__compiler, __node->init, s_off);
}

void static emit_assign(ff_compilerp __compiler, struct node *__node) {
	emit(__compiler, __node->r);
	struct node *l = __node->l;
	if (l->kind == _ast_deref) {
		push(__compiler, "%rel", 8);
		emit(__compiler, l->operand);
		out_op_s(__compiler, "movq %rel, %rlx\n");
		pop(__compiler, "%rel", 8);

		ff_u8_t sz;
		sz = l->_type->size;

		op(__compiler, "ld", pbsz(sz), "%rlx", rbsz(sz, _r_ae_el_ael_rel), NULL);
	} else if (l->kind == _ast_struct_ref) {
		char buf[128];

		ffly_nots(l->_struct->s_off-l->_type->off, buf);
		op(__compiler, "as", 'q', "%xes", buf, NULL);
		out_op_s(__compiler, "movq %bp, %rlx\n");
		op(__compiler, "sub", 'q', "%xes", "%rlx", NULL);

		ff_u8_t sz;
		sz = l->_type->size;
		op(__compiler, "ld", pbsz(sz), "%rlx", rbsz(sz, _r_ae_el_ael_rel), NULL);
	} else
		emit_load(__compiler, l->s_off, l->_type->size);
}

void static emit_literal(ff_compilerp __compiler, struct node *__node) {
	out_op_s(__compiler, "asq %rel, 0\n");
	ff_u8_t sz;
	sz = __node->_type->size;

	op(__compiler, "as", pbsz(sz), rbsz(sz, _r_ae_el_ael_rel), __node->p, NULL);
}

void static emit_out(ff_compilerp __compiler, struct node *__node) {
	emit(__compiler, __node->var);
	ff_u8_t sz;
	sz = __node->var->_type->size;
	char buf[9];
	char *p;
	p = buf;
	p+=ffly_str_cpy(p, "out");
	*(ff_u16_t*)p = (pbsz(sz)|' '<<8);
	p+=2;
	p+=ffly_str_cpy(p, rbsz(sz, _r_ae_el_ael_rel));
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
}

void static emit_var(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	ffly_nots(__node->s_off, buf);
	if (__node->_type->kind == _array) {
		op(__compiler, "as", 'q', "%rlx", buf, NULL);
		out_op_s(__compiler, "movq %bp, %rel\n");
		op(__compiler, "sub", 'q', "%rlx", "%rel", NULL);
		return;
	} 

	op(__compiler, "as", 'q', "%rel", buf, NULL);
	out_op_s(__compiler, "movq %bp, %rlx\n");

	op(__compiler, "sub", 'q', "%rel", "%rlx", NULL);

	out_op_s(__compiler, "asq %rel, 0\n");
	ff_u8_t sz;
	sz = __node->_type->size;
	op(__compiler, "st", pbsz(sz), "%rlx", rbsz(sz, _r_ae_el_ael_rel), NULL);
}

void static emit_exit(ff_compilerp __compiler, struct node *__node) {
	emit(__compiler, __node->code);
	out_op_s(__compiler, "exit %ae\n");
}

void static emit_conv(ff_compilerp __compiler, struct node *__node) {
	out_op_s(__compiler, "asq %rel, 0\n");
	emit(__compiler, __node->operand);
}

void static emit_label(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;
	p+=ffly_str_cpy(p, ".l");
	*(p++) = ' ';
	p+=ffly_str_cpy(p, __node->p);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
}

void static emit_jmpto(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	char *p = buf;
	p+=ffly_str_cpy(p, "jmp");
	*(p++) = ' ';
	*(p++) = '#';
	p+=ffly_str_cpy(p, __node->p);
	*p = '\n';
	__compiler->out(buf, (p-buf)+1);
}

void static emit_if(ff_compilerp __compiler, struct node *__node) {
	out_s(__compiler, "\t;if start\n");
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
	out_s(__compiler, "\t;if end\n");
}

void static emit_addrof(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	ffly_nots(__node->operand->s_off, buf);
	op(__compiler, "as", 'q', "%xes", buf, NULL);
	out_op_s(__compiler, "movq %bp, %rel\n");
	out_op_s(__compiler, "subq %xes, %rel\n");
}

void static emit_binop(ff_compilerp __compiler, struct node *__node) {
	out_s(__compiler, "\t;binary op start\n");
	out_op_s(__compiler, "asq %rel, 0\n");
	emit(__compiler, __node->l);
	push(__compiler, "%rel", 8);
	out_op_s(__compiler, "asq %rel, 0\n");
	emit(__compiler, __node->r);
	out_op_s(__compiler, "movq %rel, %xes\n");
	pop(__compiler, "%rel", 8);

	ff_u8_t op = __node->kind;
	if (op == _op_eq || op == _op_neq)
		out_op_s(__compiler, "cmpq %rel, %xes\n");
	else {
		switch(op) {
			case _op_add:
				out_op_s(__compiler, "addq %xes, %rel\n");
			break;
			case _op_sub:
				out_op_s(__compiler, "subq %xes, %rel\n");
			break;
			default:
				ffly_printf("unknown op, got: %u\n", op);
		}
	}
	out_s(__compiler, "\t;binary op end\n");
}

void static emit_deref(ff_compilerp __compiler, struct node *__node) {
	emit(__compiler, __node->operand);
	ff_u8_t sz;
	sz = __node->_type->size;
	op(__compiler, "st", pbsz(sz), "%rel", rbsz(sz, _r_ae_el_ael_rel), NULL);
}

void static emit_struct_ref(ff_compilerp __compiler, struct node *__node) {
	char buf[128];
	ffly_nots(__node->_struct->s_off-__node->_type->off, buf);
	op(__compiler, "as", 'q', "%rel", buf, NULL);
	out_op_s(__compiler, "movq %bp, %rlx\n");
	out_op_s(__compiler, "subq %rel, %rlx\n");
	out_op_s(__compiler, "asq %rel, 0\n");
	
	ff_u8_t sz;
	sz = __node->_type->size;
	op(__compiler, "st", pbsz(sz), "%rlx", rbsz(sz, _r_ae_el_ael_rel), NULL);
}

# include "../../gen.h"
void
emit(ff_compilerp __compiler, struct node *__node) {
# ifndef __ffly_mscarcity
	void(*func)(ff_compilerp, struct node*);
	if ((func = *(emit_tbl+__node->kind)) != NULL) { 
		func(__compiler, __node);
		return;
	}
# endif

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

void static
start(ff_compilerp __compiler) {
	out_s(__compiler, "\t.region text\n");
}

void static
final(ff_compilerp __compiler) {
	out_s(__compiler, ".endof\n");
}

# ifndef __ffly_mscarcity
struct emit_s {
	void(*src)(ff_compilerp, struct node*), (**dst)(ff_compilerp, struct node*);
};

static struct emit_s emit_funcs[] = {
	{emit_if,			emit_tbl+_ast_if},
	{emit_label,		emit_tbl+_ast_label},
	{emit_jmpto,		emit_tbl+_ast_jmpto},
	{emit_var,			emit_tbl+_ast_var},
	{emit_func,			emit_tbl+_ast_func},
	{emit_as,			emit_tbl+_ast_as},
	{emit_ret, 			emit_tbl+_ast_ret},
	{emit_func_call,	emit_tbl+_ast_func_call},
	{emit_decl,			emit_tbl+_ast_decl},
	{emit_assign,		emit_tbl+_ast_assign},
	{emit_literal,		emit_tbl+_ast_literal},
	{emit_out,			emit_tbl+_ast_out},
	{emit_exit,			emit_tbl+_ast_exit},
	{emit_conv,			emit_tbl+_ast_conv},
	{emit_addrof,		emit_tbl+_ast_addrof},
	{emit_deref,		emit_tbl+_ast_deref},
	{emit_struct_ref,	emit_tbl+_ast_struct_ref}
};
# endif

# include "../gen.h"
# include "../../system/io.h"
void ffly_ff_resin(void) {
	ffly_fprintf(ffly_log, "loading resin generator.\n");
# ifndef __ffly_mscarcity
	ff_uint_t n, i;
	n = sizeof(emit_funcs)/sizeof(struct emit_s);
	i = 0;
	for(;i!=n;i++) {
		struct emit_s *e;
		e = emit_funcs+i;
		*e->dst = e->src;
	}
# endif
	ffly_ff_gen_start = start;
	ffly_ff_gen_final = final;
	ffly_ff_emit = emit;
}
