# define __ffly_script_internal
# include "script.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# include "system/file.h"
# include "data/str_cmp.h"
# include "data/mem_cpy.h"
# include "data/mem_set.h"
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
	if (__obj->opcode == _op_alloc || __obj->opcode == _op_assign || __obj->opcode == _op_copy) {
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
	ffly_mem_cpy((*__obj->to)->p, __obj->p, __obj->_type->size);
}

void op_copy(struct obj *__obj) {
	pr_obj(__obj);
	ffly_mem_cpy((*__obj->to)->p, (*__obj->from)->p, __obj->_type->size);
}

void op_compare(struct obj *__obj) {
    mdl_u8_t l[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    mdl_u8_t r[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    ffly_mem_cpy(l, (*__obj->l)->p, (*__obj->l)->_type->size);
    ffly_mem_cpy(r, (*__obj->r)->p, (*__obj->r)->_type->size);
    mdl_u8_t *flags = (mdl_u8_t*)__obj->flags->p;
    *flags = 0x0;
    ffly_printf("r %u, l: %u\n", *(mdl_u64_t*)l, *(mdl_u64_t*)r);
    switch((*__obj->l)->_type->kind) {
        case _u64_t: case _u32_t: case _u16_t: case _u8_t:
            if (*(mdl_u64_t*)l == *(mdl_u64_t*)r) *flags |= _flg_eq; else *flags |= _flg_neq;
            if (*(mdl_u64_t*)l > *(mdl_u64_t*)r) *flags |= _flg_gt;
            if (*(mdl_u64_t*)l < *(mdl_u64_t*)r) *flags |= _flg_lt;
        break;
        case _i64_t: case _i32_t: case _i16_t: case _i8_t:
            if (*(mdl_u64_t*)l > (1<<((*__obj->l)->_type->size*8))>>1) {
                *(mdl_u64_t*)l |= (~(mdl_u64_t)0)>>((*__obj->l)->_type->size*8);
            }
            if (*(mdl_u64_t*)r > (1<<((*__obj->r)->_type->size*8))>>1) {
                *(mdl_u64_t*)r |= (~(mdl_u64_t)0)>>((*__obj->r)->_type->size*8);
            }

            if (*(mdl_i64_t*)l == *(mdl_i64_t*)r) *flags |= _flg_eq; else *flags |= _flg_neq;
            if (*(mdl_i64_t*)l > *(mdl_i64_t*)r) *flags |= _flg_gt;
            if (*(mdl_i64_t*)l < *(mdl_i64_t*)r) *flags |= _flg_lt;
        break;
    }
}

void op_jump(struct obj *__objp) {
    struct obj *_obj = *(struct obj**)__objp;
    if (!_obj->jmp) return;
    if (!*_obj->jmp) return;
    *(struct obj**)__objp = **_obj->jmp;
    ffly_printf("jump.\n");
}

void op_cond_jump(struct obj *__objp) {
    struct obj *_obj = *(struct obj**)__objp;
    ffly_printf("------------------- %u\n", *(mdl_u8_t*)_obj->flags->p);
    if (((*(mdl_u8_t*)_obj->flags->p)&_obj->cond)>0)
        op_jump(__objp);
}

// debug
void op_print(struct obj *__obj) {
	pr_obj(__obj);
    struct obj *val = *__obj->val;
    ffly_printf("--> %p\n", val);
	if (__obj->_type->kind == _uint_t) {
		ffly_printf("%lu\n", *(mdl_uint_t*)val->p);
		return;
	} else if (__obj->_type->kind == _int_t) {
		ffly_printf("%ld\n", *(mdl_int_t*)val->p);
		return;
	}

	switch(__obj->_type->kind) {
		case _u64_t:
			ffly_printf("%lu\n", *(mdl_u64_t*)val->p);
		break;
		case _i64_t:
			ffly_printf("%ld\n", *(mdl_i64_t*)val->p);
		break;
		case _u32_t:
			ffly_printf("%u\n", *(mdl_u32_t*)val->p);
		break;
		case _i32_t:
			ffly_printf("%d\n", *(mdl_i32_t*)val->p);
		break;
		case _u16_t:
			ffly_printf("%u\n", *(mdl_u16_t*)val->p);
		break;
		case _i16_t:
			ffly_printf("%d\n", *(mdl_i16_t*)val->p);
		break;
		case _u8_t:
			ffly_printf("%u\n", *(mdl_u8_t*)val->p);
		break;
		case _i8_t:
			ffly_printf("%d\n", *(mdl_i8_t*)val->p);
		break;
	}
}

void op_zero(struct obj *__obj) {
    ffly_mem_set((*__obj->dst)->p, 0x0, (*__obj->dst)->_type->size);
}

struct obj *stack[20];
struct obj **top = stack;

void op_push(struct obj *__obj) {
    *(top++) = __obj->_obj;
    ffly_printf("push.\n");
}

void op_pop(struct obj *__obj) {
    __obj->_obj = *(--top);
    ffly_printf("pop.\n");
}

void(*op[])(struct obj*) = {
	&op_alloc,
	&op_free,
	&op_assign,
	&op_copy,
	&op_print,
    &op_compare,
    &op_jump,
    &op_cond_jump,
    &op_zero,
    &op_push,
    &op_pop
};

ffly_err_t ffly_script_exec(struct ffly_script *__script) {
	struct obj *_obj = (struct obj*)__script->top;
	while(_obj != NULL) {
        usleep(10000);
		if (_obj->opcode >= _op_alloc && _obj->opcode <= _op_pop) {
            mdl_u8_t isjmp = (_obj->opcode == _op_jump || _obj->opcode == _op_cond_jump);
            if (!isjmp)
			    op[_obj->opcode](_obj);
            else {
                struct obj *p = _obj;
                op[_obj->opcode]((struct obj*)&p);
                if (p != _obj) {
                    _obj = p;
                    continue;
                }
            }
            if (!_obj) break;
        }
		_obj = _obj->next;
	}
}

ffly_bool_t is_keyword(struct token *__tok, mdl_u8_t __id) {
	return (__tok->kind == TOK_KEYWORD && __tok->id == __id);
}

ffly_off_t toklo(struct token *__tok) {
    return __tok->lo;
}

ffly_off_t curlo(struct ffly_script *__script) {
    return __script->lo;
}

ffly_bool_t next_token_is(struct ffly_script *__script, mdl_u8_t __kind, mdl_u8_t __id) {
	struct token *tok = next_token(__script);
	if (!tok) return 0;
	if (tok->kind == __kind && tok->id == __id)
		return 1;
	ffly_script_ulex(__script, tok);
	return 0;
}

mdl_uint_t tokcol(struct token *__tok) {
    return __tok->off-toklo(__tok);
}

ffly_bool_t expect_token(struct ffly_script *__script, mdl_u8_t __kind, mdl_u8_t __id) {
	struct token *tok = next_token(__script);
	if (!tok) return 0;
	mdl_u8_t ret_val;
    if (!(ret_val = (tok->kind == __kind && tok->id == __id))) {
        if (__kind == TOK_KEYWORD) {
            switch(__id) {
                case _l_brace:
                    ffly_fprintf(ffly_out, "%u;%u: expected left side brace, got %s.\n", tokl(tok), tokcol(tok), tokid_str(tok->id));
                break;
                case _r_brace:
                    ffly_fprintf(ffly_out, "%u;%u: expected right side brace, got %s.\n", tokl(tok), tokcol(tok), tokid_str(tok->id));
                break;
                case _semicolon:
                    ffly_fprintf(ffly_out, "%u;%u: expected semicolon, got %s.\n", tokl(tok), tokcol(tok), tokid_str(tok->id));
                break;
                case _l_paren:
                    ffly_fprintf(ffly_out, "%u;%u: expected left side paren, got %s.\n", tokl(tok), tokcol(tok), tokid_str(tok->id));
                break;
                case _r_paren:
                    ffly_fprintf(ffly_out, "%u;%u: expected right side paren, got %s.\n", tokl(tok), tokcol(tok), tokid_str(tok->id));
                break;
            }
        }
    }
    return ret_val;
}

ffly_err_t ffly_script_free(struct ffly_script *__script) {
	struct node **_node = (struct node**)ffly_vec_begin(&__script->nodes);
	if (ffly_vec_size(&__script->nodes)>0) {
		while(_node <= (struct node**)ffly_vec_end(&__script->nodes)) {
            if ((*_node)->_type != NULL)
                __ffly_mem_free((*_node)->_type);
			__ffly_mem_free(*_node);
			_node++;
		}
	}

	struct token **tok = (struct token**)ffly_vec_begin(&__script->toks);
	if (ffly_vec_size(&__script->toks)>0) {
		while(tok <= (struct token**)ffly_vec_end(&__script->toks)) {
			if ((*tok)->p != NULL)
				__ffly_mem_free((*tok)->p);
			__ffly_mem_free(*tok);
			tok++;
		}
	}

    ffly_map_de_init(&__script->env);
    ffly_buff_de_init(&__script->sbuf);
    ffly_vec_de_init(&__script->nodes);
	ffly_vec_de_init(&__script->toks);
	ffly_buff_de_init(&__script->iject_buff);
    ffly_script_gen_free();

    struct obj *_obj = (struct obj*)__script->top, *prev = NULL;
    while(_obj != NULL) {
        if (prev != NULL)
            __ffly_mem_free(prev);
        prev = _obj;
        _obj = _obj->next;
    }
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

mdl_uint_t tokl(struct token *__tok) {
    return __tok->line;
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
    else if (!ffly_str_cmp(__tok->p, "if"))
        to_keyword(__tok, _k_if);
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
    else if (!ffly_str_cmp(__tok->p, "fn"))
        to_keyword(__tok, _k_fn);
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
    ffly_map_init(&__script->env);
	__script->off = 0;
	__script->top = NULL;
    __script->line = 0;
    __script->lo = 0;
}

ffly_err_t ffly_script_build(struct ffly_script *__script) {
    ffly_err_t err;
    if (_err(err = ffly_script_parse(__script)))
        return err;
    if (_err(err = ffly_script_gen(__script)))
        return err;
    return FFLY_SUCCESS;
}

ffly_bool_t is_eof(struct ffly_script *__script) {
	return (__script->p+__script->off) >= __script->end;
}

mdl_uint_t curl(struct ffly_script *__script) {
    return __script->line;
}

void skip_token(struct ffly_script *__script) {
    next_token(__script);
}

ffly_err_t ffly_script_save_bin(struct ffly_script *__script, char *__file) {
/*
    ffly_err_t err;
    FF_FILE *f = ffly_fopen(__file, FF_O_WRONLY|FF_O_TRUNC|FF_O_CREAT, FF_S_IRUSR|FF_S_IWUSR, &err);
    ffly_fseek(f, sizeof(mdl_u32_t), FF_SEEK_SET);
    mdl_u32_t top = ffly_fseek(f, 0, FF_SEEK_CUR);
    mdl_u32_t off = top;

    mdl_u32_t prev;
    struct obj *_obj = (struct obj*)__script->top;
    struct obj o;
    while(_obj != NULL) {
        ffly_printf("\n");
        pr_obj(_obj);
        _obj->off = off;
        o = *_obj;
        off+= sizeof(struct obj);
        ffly_fseek(f, off, FF_SEEK_SET);
        if (_obj->_type != NULL) {            
            o._type = (void*)off;
            ffly_fwrite(f, _obj->_type, sizeof(struct type));
            off+= sizeof(struct type);

            if (_obj->p != NULL) {
                o.p = (void*)off;
                ffly_fwrite(f, _obj->p, _obj->_type->size);
                ffly_printf("data: %u\n", *(mdl_u16_t*)_obj->p);
                off+= _obj->_type->size;
            }

            ffly_printf("types, off: %u\n", (mdl_u32_t)o._type);
        }

        if (_obj->to != NULL) {
            ffly_printf("--to-- %u\n", _obj->to->off);
            o.to = (void*)_obj->to->off;
        }
        if (_obj->from != NULL) {
            ffly_printf("--from-- %u\n", _obj->from->off);
            o.from = (void*)_obj->from->off;
        }
        if (_obj->val != NULL) {
            ffly_printf("--val-- %u\n", _obj->val->off);
            o.val = (void*)_obj->val->off;
        }
        if (_obj->l != NULL) {
            o.l = (void*)_obj->l->off;
        }
        if (_obj->r != NULL) {
            o.r = (void*)_obj->r->off;
        }
        if (_obj->opcode == _op_jump || _obj->opcode == _op_cond_jump) { 
            o.jmp = !*_obj->jmp?NULL:(void**)(*_obj->jmp)->off;
        }

        if (_obj->flags != NULL) {
            o.flags = (void*)_obj->flags->off;
        }

        if ((void*)_obj != __script->top) {
            void *off = (void*)_obj->off;
            ffly_fseek(f, prev, FF_SEEK_SET);
            ffly_fwrite(f, &off, sizeof(void*));
        }

        ffly_printf("obj, off: %u\n", _obj->off);
        ffly_fseek(f, _obj->off, FF_SEEK_SET); 
        ffly_fwrite(f, &o, sizeof(struct obj));
        prev = _obj->off+offsetof(struct obj, next);
        _obj = _obj->next;
    }

    ffly_fseek(f, 0, FF_SEEK_SET);
    ffly_fwrite(f, &top, sizeof(mdl_u32_t));
    ffly_fclose(f);
*/
}

# include "system/bin_tree.h"
ffly_err_t ffly_script_ld_bin(struct ffly_script *__script, char *__file) {
/*
    ffly_err_t err;
    FF_FILE *f = ffly_fopen(__file, FF_O_RDONLY, 0, &err);
    struct ffly_bin_tree objs;

    ffly_bin_tree_init(&objs);

    mdl_u32_t off;
    ffly_fread(f, &off, sizeof(mdl_u32_t));
    struct obj *_obj, *prev = NULL;
    struct obj *top = NULL;

    _next:
    ffly_printf("\n");
    _obj = (struct obj*)__ffly_mem_alloc(sizeof(struct obj));
    if (!top)
        top = _obj;
    ffly_fseek(f, off, FF_SEEK_SET);
    ffly_fread(f, _obj, sizeof(struct obj));
    ffly_bin_tree_insert(&objs, off, _obj);
    if (_obj->_type != NULL) {
        ffly_fseek(f, (mdl_u32_t)_obj->_type, FF_SEEK_SET);
        _obj->_type = (struct obj*)__ffly_mem_alloc(sizeof(struct type));
        ffly_fread(f, _obj->_type, sizeof(struct type));
    
        ffly_printf("---> size: %u\n", _obj->_type->size);
        if (_obj->p != NULL) {
            ffly_fseek(f, (mdl_u32_t)_obj->p, FF_SEEK_SET);
            if (_obj->_type->size < 100) { // in case 
                _obj->p = __ffly_mem_alloc(_obj->_type->size);
                ffly_fread(f, _obj->p, _obj->_type->size);
            }
            ffly_printf("data: %u\n", *(mdl_u16_t*)_obj->p);
        }
    }

    if (_obj->to != NULL)
        ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->to, (void**)&_obj->to);
    if (_obj->from != NULL)
        ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->from, (void**)&_obj->from);
    if (_obj->val != NULL)
        ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->val, (void**)&_obj->val); 
    if (_obj->l != NULL)
        ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->l, (void**)&_obj->l);
    if (_obj->r != NULL)
        ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->r, (void**)&_obj->r);
    
    if (_obj->opcode == _op_jump || _obj->opcode == _op_cond_jump) {
        struct obj **p = (struct obj**)__ffly_mem_alloc(sizeof(struct obj*));
        if (_obj->jmp != NULL) {
            ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->jmp, (void**)p);
        } else
            *p = NULL;
        _obj->jmp = p;
    }

    if (_obj->flags != NULL)
        ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->flags, (void**)&_obj->flags);

    pr_obj(_obj);
    if (prev != NULL)
        prev->next = _obj;
    prev = _obj;

    if (_obj->next != NULL) {
        off = (mdl_u32_t)_obj->next;
        goto _next;
    }
    _obj->next = NULL;

    __script->top = (void*)top;
    ffly_fclose(f);
    ffly_bin_tree_de_init(&objs);
*/
}
//# define LOAD
int main() {
	struct ffly_script script;
	ffly_io_init();
	ffly_printf("loading script.\n");
	ffly_script_prepare(&script);
# ifndef LOAD
	ffly_script_ld(&script, "scripts/main.ff");
	ffly_printf("building script.\n");
	if (_err(ffly_script_build(&script))) {
        ffly_printf("failed to build.\n");
        return -1;
    }
	ffly_printf("executing script.\n");
	ffly_script_exec(&script);
//    ffly_script_save_bin(&script, "test.bin");
# else
    ffly_script_ld_bin(&script, "test.bin");
    ffly_script_exec(&script);
# endif

	ffly_script_free(&script);
	ffly_io_closeup();
}


