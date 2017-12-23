# define __ffly_script_internal
# include "script.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# include "system/file.h"
# include "data/str_cmp.h"
# include "data/mem_cpy.h"
ffly_err_t ffly_script_ld(struct ffly_script *__script, char *__file) {
	ffly_err_t err;
	FF_FILE *f = ffly_fopen(__file, FF_O_RDONLY, 0, &err);

	struct ffly_stat st;
	ffly_fstat(__file, &st);

	__script->p = (ffly_byte_t*)__ffly_mem_alloc(st.size);
	__script->end = __script->p+st.size;
	ffly_fread(f, __script->p, st.size);
	ffly_fclose(f);
}

char const* tokk_str(mdl_u8_t __kind) {
	switch(__kind) {
		case TOK_IDENT: return "ident";
		case TOK_KEYWORD: return "keyword";
		case TOK_NO: return "no";
	}

	return "unknown";
}

char const* tokid_str(mdl_u8_t __id) {
	switch(__id) {
		case _k_print: return "print";
		case _k_uint_t: return "uint_t";
		case _k_int_t: return "int_t";
		case _k_u64_t: return "u64_t";
		case _k_i64_t: return "i64_t";
		case _k_u32_t: return "u32_t";
		case _k_i32_t: return "i32_t";
		case _k_u16_t: return "u16_t";
		case _k_i16_t: return "i16_t";
		case _k_u8_t: return "u8_t";
		case _k_i8_t: return "i8_t";
		case _semicolon: return ";";
		case _eq: return "=";
	}
	return "unknown";
}

void pr_tok(struct token *__tok) {
	if (!__tok) return;
	ffly_printf("tok kind: %s\n", tokk_str(__tok->kind));
	ffly_printf("tok id: %s\n", tokid_str(__tok->id));
	ffly_printf("tokp: %s\n", (char*)__tok->p);
}

char const* opstr(mdl_u8_t __op) {
	switch(__op) {
		case _op_alloc: return "alloc";
		case _op_free: return "free";
		case _op_assign: return "assign";
		case _op_copy: return "copy";
		case _op_print: return "print";
	}
	return "unknown";
}

void pr_obj(struct obj *__obj) {
	ffly_printf("op: %s\n", opstr(__obj->opcode));
	if (__obj->opcode == _op_alloc) {
		ffly_printf("size: %u\n", __obj->_type->size);
	}
}

void op_alloc(struct obj *__obj) {
	pr_obj(__obj);
	__obj->p = __ffly_mem_alloc(__obj->_type->size);
}

void op_free(struct obj *__obj) {
	pr_obj(__obj);
	__ffly_mem_free(__obj->p);
}

void op_assign(struct obj *__obj) {
	pr_obj(__obj);
	ffly_mem_cpy(__obj->to->p, __obj->p, __obj->_type->size);
}

void op_copy(struct obj *__obj) {
	pr_obj(__obj);
	ffly_mem_cpy(__obj->to->p, __obj->from->p, __obj->_type->size);
}

// debug
void op_print(struct obj *__obj) {
	pr_obj(__obj);
	if (__obj->_type->kind == _uint_t) {
		ffly_printf("%lu\n", *(mdl_uint_t*)__obj->val->p);
		return;
	} else if (__obj->_type->kind == _int_t) {
		ffly_printf("%ld\n", *(mdl_int_t*)__obj->val->p);
		return;
	}

	switch(__obj->_type->kind) {
		case _u64_t:
			ffly_printf("%lu\n", *(mdl_u64_t*)__obj->val->p);
		break;
		case _i64_t:
			ffly_printf("%ld\n", *(mdl_i64_t*)__obj->val->p);
		break;
		case _u32_t:
			ffly_printf("%u\n", *(mdl_u32_t*)__obj->val->p);
		break;
		case _i32_t:
			ffly_printf("%d\n", *(mdl_i32_t*)__obj->val->p);
		break;
		case _u16_t:
			ffly_printf("%u\n", *(mdl_u16_t*)__obj->val->p);
		break;
		case _i16_t:
			ffly_printf("%d\n", *(mdl_i16_t*)__obj->val->p);
		break;
		case _u8_t:
			ffly_printf("%u\n", *(mdl_u8_t*)__obj->val->p);
		break;
		case _i8_t:
			ffly_printf("%d\n", *(mdl_i8_t*)__obj->val->p);
		break;
	}
}

void(*op[])(struct obj*) = {
	&op_alloc,
	&op_free,
	&op_assign,
	&op_copy,
	&op_print
};

ffly_err_t ffly_script_exec(struct ffly_script *__script) {
	struct obj *_obj = __script->top;
	while(_obj != NULL) {
		if (_obj->opcode >= _op_alloc && _obj->opcode <= _op_print)
			op[_obj->opcode](_obj);
		_obj = _obj->next;
	}
}

ffly_bool_t is_keyword(struct token *__tok, mdl_u8_t __id) {
	return (__tok->kind == TOK_KEYWORD && __tok->id == __id);
}

ffly_bool_t next_token_is(struct ffly_script *__script, mdl_u8_t __kind, mdl_u8_t __id) {
	struct token *tok = next_token(__script);
	if (!tok) return 0;
	if (tok->kind == __kind && tok->id == __id)
		return 1;
	ffly_script_ulex(__script, tok);
	return 0;
}

ffly_bool_t expect_token(struct ffly_script *__script, mdl_u8_t __kind, mdl_u8_t __id) {
	struct token *tok = next_token(__script);
	if (!tok) return 0;
	return (tok->kind == __kind && tok->id == __id);
}

ffly_err_t ffly_script_free(struct ffly_script *__script) {
	ffly_buff_de_init(&__script->sbuf);
	struct node **_node = (struct node**)ffly_vec_begin(&__script->nodes);
	if (ffly_vec_size(&__script->nodes)>0) {
		while(_node <= (struct node**)ffly_vec_end(&__script->nodes)) {
			__ffly_mem_free(*_node);
			_node++;
		}
	}

	ffly_vec_de_init(&__script->nodes);
	struct token **tok = (struct token**)ffly_vec_begin(&__script->toks);
	if (ffly_vec_size(&__script->toks)>0) {
		while(tok <= (struct token**)ffly_vec_end(&__script->toks)) {
			if ((*tok)->p != NULL)
				__ffly_mem_free((*tok)->p);
			__ffly_mem_free(*tok);
			tok++;
		}
	}

	ffly_vec_de_init(&__script->toks);
	ffly_buff_de_init(&__script->iject_buff);
}

struct token* peek_token(struct ffly_script *__script) {
	ffly_err_t err;
	struct token *tok = next_token(__script);
	ffly_script_ulex(__script, tok);
	return tok;
}

ffly_bool_t maybe_keyword(struct token*);
struct token* next_token(struct ffly_script *__script) {
	ffly_err_t err;
	struct token *tok = ffly_script_lex(__script, &err);
	maybe_keyword(tok);
	return tok;
}

void to_keyword(struct token *__tok, mdl_u8_t __id) {
	__tok->kind = TOK_KEYWORD;
	__tok->id = __id;
}

ffly_bool_t maybe_keyword(struct token *__tok) {
	if (!__tok) return 0;
	if (__tok->kind != TOK_IDENT || __tok->p == NULL) return 0;
	if (!ffly_str_cmp(__tok->p, "print"))
		to_keyword(__tok, _k_print);
	else if (!ffly_str_cmp(__tok->p, "uint_t"))
		to_keyword(__tok, _k_uint_t);
	else if (!ffly_str_cmp(__tok->p, "int_t"))
		to_keyword(__tok, _k_int_t);
	else if (!ffly_str_cmp(__tok->p, "u64_t"))
		to_keyword(__tok, _k_u64_t);
	else if (!ffly_str_cmp(__tok->p, "i64_t"))
		to_keyword(__tok, _k_i64_t);
	else if (!ffly_str_cmp(__tok->p, "u32_t"))
		to_keyword(__tok, _k_u32_t);
	else if (!ffly_str_cmp(__tok->p, "i32_t"))
		to_keyword(__tok, _k_i32_t);
	else if (!ffly_str_cmp(__tok->p, "u16_t"))
		to_keyword(__tok, _k_u16_t);
	else if (!ffly_str_cmp(__tok->p, "i16_t"))
		to_keyword(__tok, _k_i16_t);
	else if (!ffly_str_cmp(__tok->p, "u8_t"))
		to_keyword(__tok, _k_u8_t);
	else if (!ffly_str_cmp(__tok->p, "i8_t"))
		to_keyword(__tok, _k_i8_t);
	else {
		return 0;
	}
	return 1;
}

ffly_err_t ffly_script_prepare(struct ffly_script *__script) {
	ffly_buff_init(&__script->sbuf, 100, 1);
	ffly_vec_clear_flags(&__script->nodes);
	ffly_vec_clear_flags(&__script->toks);
	ffly_vec_set_flags(&__script->nodes, VEC_AUTO_RESIZE);
	ffly_vec_set_flags(&__script->toks, VEC_AUTO_RESIZE);

	ffly_vec_init(&__script->nodes, sizeof(struct node*));
	ffly_vec_init(&__script->toks, sizeof(struct token*));
	ffly_buff_init(&__script->iject_buff, 100, sizeof(struct token*));
	__script->off = 0;
	__script->top = NULL;
}

ffly_bool_t is_eof(struct ffly_script *__script) {
	return (__script->p+__script->off) >= __script->end;
}

int main() {
	struct ffly_script script;
	ffly_io_init();
	ffly_printf("loading script.\n");
	ffly_script_prepare(&script);
	ffly_script_ld(&script, "scripts/main.ff");
	ffly_printf("parsing script.\n");
	ffly_script_parse(&script);
	ffly_printf("generating script.\n");
	ffly_script_gen(&script);
	ffly_printf("executing script.\n");
	ffly_script_exec(&script);

	ffly_script_free(&script);
	ffly_io_closeup();
}


