# include "bole.h"
# include "dep/str_len.h"
# include "dep/str_dup.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# include "system/file.h"
# include "system/string.h"
/*
	TODO:
		remake after naming
*/

enum {
	_tok_ident,
	_tok_keyword,
	_tok_str,
	_tok_no,
	_tok_chr
};

enum {
	_tilde,
	_colon,
	_comma,
	_l_bracket,
	_r_bracket,
	_l_brace,
	_r_brace
};

// macro?
ff_bool_t static is_space(char __c) {
	return (__c == ' ' || __c == '\n' || __c == '\t');
}

char const* ffly_bole_str(void const *__p) {
	return (char*)((struct ffly_bole_val*)__p)->p;
}
char ffly_bole_chr(void const *__p) {
	return *(char*)((struct ffly_bole_val*)__p)->p;
}

ff_u64_t ffly_bole_64l_u(void const *__p) {
	return *(ff_u64_t*)((struct ffly_bole_val*)__p)->p;}
ff_u32_t ffly_bole_32l_u(void const *__p) {
	return *(ff_u32_t*)((struct ffly_bole_val*)__p)->p;}
ff_u16_t ffly_bole_16l_u(void const *__p) {
	return *(ff_u16_t*)((struct ffly_bole_val*)__p)->p;}
ff_u8_t ffly_bole_8l_u(void const *__p) {
	return *(ff_u8_t*)((struct ffly_bole_val*)__p)->p;}
ff_i64_t ffly_bole_64l_s(void const *__p) {
	return *(ff_i64_t*)((struct ffly_bole_val*)__p)->p;}
ff_i32_t ffly_bole_32l_s(void const *__p) {
	return *(ff_i32_t*)((struct ffly_bole_val*)__p)->p;}
ff_i16_t ffly_bole_16l_s(void const *__p) {
	return *(ff_i16_t*)((struct ffly_bole_val*)__p)->p;}
ff_i8_t ffly_bole_8l_s(void const *__p) {
	return *(ff_i8_t*)((struct ffly_bole_val*)__p)->p;}

/*
	deposit all into ffbole
*/
void ffly_bole_depos(struct ffly_bole *__bole, ffbolep __d) {
	__d->entities = __bole->entities;
	__d->free = __bole->free;
	__d->env = __bole->env;
}

void const*
ffly_bole_get(ffbolep __bole, char const *__name) {
	ff_err_t err;
	return ffly_map_get(&__bole->env, (ff_u8_t const*)__name, ffly_str_len(__name), &err);
}

void const*
ffly_bole_struc_get(void const *__p, char const *__name) {
	ff_err_t err;
	struct ffly_bole_struct *p = (struct ffly_bole_struct*)__p;
	return ffly_map_get(&p->fields, (ff_u8_t const*)__name, ffly_str_len(__name), &err);
}

void const*
ffly_bole_arr_elem(void const *__p, ff_uint_t __no) {
	return *(void**)ffly_vec_at(&((struct ffly_bole_arr*)__p)->data, __no);
}

ff_uint_t ffly_bole_arr_len(void const *__p) {
	return ((struct ffly_bole_arr*)__p)->l;
}

struct token {
	ff_u8_t kind, id;
	void *p, *end;
};

struct token static* read_token(struct ffly_bole*, ff_err_t*);
struct token static* peek_token(struct ffly_bole*, ff_err_t*);
ff_bool_t static
expect_token(struct ffly_bole *__bole, ff_u8_t __kind, ff_u8_t __id, ff_err_t *__err) {
	struct token *tok = read_token(__bole, __err);
	if (_err(*__err) || !tok) return 0;
	ff_u8_t ret_val;
	if (!(ret_val = (tok->kind == __kind && tok->id == __id))) {
		if (tok->kind == __kind) {
			switch(__id) {
				case _colon:
					ffly_fprintf(ffly_out, "expected colon.\n");
				break;
				case _comma:
					ffly_fprintf(ffly_out, "expected comma.\n");
				break;
				case _l_bracket:
					ffly_fprintf(ffly_out, "expected left side bracket.\n");
				break;
				case _r_bracket:
					ffly_fprintf(ffly_out, "expected right side bracket.\n");
				break;
			}
		}
	}
	*__err = FFLY_SUCCESS;
	return ret_val;
}

ff_err_t static push_free(struct ffly_bole *__bole, void *__p) {
	void **p;
	ffly_vec_push_back(&__bole->free, (void**)&p);
	*p = __p;
}

ff_err_t static
push_entity(struct ffly_bole *__bole, struct ffly_bole_entity *__entity) {
	struct ffly_bole_entity **p;
	ffly_vec_push_back(&__bole->entities, (void**)&p);
	*(*p = __ffly_mem_alloc(sizeof(struct ffly_bole_entity))) = *__entity;
}

ff_bool_t static is_eof(struct ffly_bole *__bole) {
	return (__bole->p+__bole->off) >= __bole->end;
}

/*
	TODO:
		macros
*/
void static mk_ident(struct token *__tok, char *__s) {
	*__tok = (struct token){.kind=_tok_ident, .p=(void*)__s};
}
void static mk_keyword(struct token *__tok, ff_u8_t __id) {
	*__tok = (struct token){.kind=_tok_keyword, .id=__id, .p=NULL};
}
void static mk_str(struct token *__tok, char *__s) {
	*__tok = (struct token){.kind=_tok_str, .p=(void*)__s};
}
void static mk_no(struct token *__tok, char *__s) {
	*__tok = (struct token){.kind=_tok_no, .p=(void*)__s};
}
void static mk_chr(struct token *__tok, char *__s) {
	*__tok = (struct token){.kind=_tok_chr, .p=(void*)__s};
}

char static*
read_ident(struct ffly_bole *__bole, char **__end) {
	char *p = (char*)(__bole->p+__bole->off);
	char c = *p;
	while((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9')) {
		ffly_buff_put(&__bole->sbuf, p++);
		ffly_buff_incr(&__bole->sbuf);
		c = *p;
	}

	char end = '\0';
	ffly_buff_put(&__bole->sbuf, &end);
	char *s = (char*)ffly_str_dupe((char*)ffly_buff_begin(&__bole->sbuf));
	ff_uint_t l = ffly_buff_off(&__bole->sbuf);
	*__end = s+l;
	__bole->off+= l;
	ffly_buff_off_reset(&__bole->sbuf);
	return s;
}

char static*
read_no(struct ffly_bole *__bole, char **__end) {
	char *p = (char*)(__bole->p+__bole->off);
	char c = *p;
	while((c >= '0' && c <= '9') || c == '.' || c == '-') {
		ffly_buff_put(&__bole->sbuf, p++);
		ffly_buff_incr(&__bole->sbuf);
		c = *p;
	}

	char end = '\0';
	ffly_buff_put(&__bole->sbuf, &end);
	char *s = (char*)ffly_str_dupe((char*)ffly_buff_begin(&__bole->sbuf));
	ff_uint_t l = ffly_buff_off(&__bole->sbuf);
	*__end = s+l;
	__bole->off+= l;
	ffly_buff_off_reset(&__bole->sbuf);
	return s;
}

char static*
read_str(struct ffly_bole *__bole, char **__end) {
	char *p = (char*)(__bole->p+__bole->off);
	while(*p != '"') {
		ffly_buff_put(&__bole->sbuf, p++);
		ffly_buff_incr(&__bole->sbuf);
	}

	char end = '\0';
	ffly_buff_put(&__bole->sbuf, &end);
	char *s = (char*)ffly_str_dupe((char*)ffly_buff_begin(&__bole->sbuf));
	ff_uint_t l = ffly_buff_off(&__bole->sbuf);
	*__end = s+l; 
	__bole->off+= l;
	ffly_buff_off_reset(&__bole->sbuf);
	return s;
}

/*
	TODO:
		macro
*/
char static*
read_chr(struct ffly_bole *__bole) {
	__bole->off++;
	return (char*)(__bole->p+(__bole->off-1));
}

ff_err_t static
uread_token(struct ffly_bole *__bole, struct token *__tok) {
	ff_err_t err;
	ffly_buff_put(&__bole->tokbuf, (void*)&__tok);
	ffly_buff_incr(&__bole->tokbuf);
	return FFLY_SUCCESS;
}	

char static
fetchc(struct ffly_bole *__bole) {
	return *(__bole->p+__bole->off);
}

# ifndef __ffly_mscarcity
# define _ 0xff
ff_u8_t static map[] = {
/*	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	*/
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,						// 0
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,						// 1
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_comma,	_,	_,	_,					// 2
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_colon,	_,	_,	_,	_,	_,					// 3
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,						// 4
/*	0	1	2	3	4	5	6	7	8	9	10	11			12	13			14	15	*/
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_l_bracket,	_,	_r_bracket,	_,	_,		// 5
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,						// 6
/*	0	1	2	3	4	5	6	7	8	9	10	11			12	13			14		15	*/
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_l_brace,	_,	_r_brace,	_tilde,	_,	// 7

	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,						// 8
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,						// 9
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,						// 10
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,						// 11
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,						// 12
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,						// 13
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,						// 14
	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,	_,						// 15
};
# endif

struct token static*
read_token(struct ffly_bole *__bole, ff_err_t *__err) {
	ff_err_t err;
	if (ffly_buff_off(&__bole->tokbuf)>0){
		struct token *tok;
		ffly_buff_decr(&__bole->tokbuf);
		ffly_buff_get(&__bole->tokbuf, (void*)&tok);
		*__err = FFLY_SUCCESS;
		return tok;
	}

	char c, *end;
_again:
	while(is_space(fetchc(__bole)) && !is_eof(__bole)) __bole->off++;
	if (fetchc(__bole) == '#' && !is_eof(__bole)) {
		while(fetchc(__bole) != '\n') {
			if (is_eof(__bole)) goto _eof;
			__bole->off++;
		}
		goto _again;
	}

	if (is_eof(__bole)) {
		_eof:
		*__err = FFLY_SUCCESS;
		return NULL;
	}

	struct token *tok, **p;
	if (!(tok = (struct token*)__ffly_mem_alloc(sizeof(struct token)))) {
		//err
	}

	c = fetchc(__bole);
# ifndef __ffly_mscarcity
	ff_u8_t i;
	if ((i = map[(ff_u8_t)c]) != _) {
		mk_keyword(tok, i);
		__bole->off++;
		goto _sk;
	}
# endif
	switch(c) {
		case '"':
			__bole->off++;
			mk_str(tok, read_str(__bole, &end));
			__bole->off++;
		break;
		case '\x27':
			__bole->off++;
			mk_chr(tok, read_chr(__bole));
			__bole->off++;
		break;
# ifdef __ffly_mscarcity
		case '~':
			mk_keyword(tok, _tilde);
			__bole->off++;
		break;
		case '{':
			mk_keyword(tok, _l_brace);
			__bole->off++;
		break;
		case '}':
			mk_keyword(tok, _r_brace);
			__bole->off++;
		break;
		case ':':
			mk_keyword(tok, _colon);
			__bole->off++;
		break;
		case ',':
			mk_keyword(tok, _comma);
			__bole->off++;
		break;
		case '[':
			mk_keyword(tok, _l_bracket);
			__bole->off++;
		break;
		case ']':
			mk_keyword(tok, _r_bracket);
			__bole->off++;
		break;
# endif
		default:
			if ((c >= 'a' && c <= 'z') || c == '_')
				mk_ident(tok, read_ident(__bole, &end));
			else if ((c >= '0' && c <= '9') || c == '-')
				mk_no(tok, read_no(__bole, &end));
			else {
				__ffly_mem_free(tok);
				*__err = FFLY_FAILURE;
				return NULL;
			}
	}
# ifndef __ffly_mscarcity
_sk:
# endif
	tok->end = end;
	ffly_vec_push_back(&__bole->toks, (void**)&p);
	*p = tok;
	*__err = FFLY_SUCCESS;
	return tok;
}

ff_bool_t static
next_token_is(struct ffly_bole *__bole, ff_u8_t __kind, ff_u8_t __id, ff_err_t *__err) {
	struct token *tok = read_token(__bole, __err);
	*__err = FFLY_SUCCESS;
	if (tok->kind == __kind && tok->id == __id) return 1;
	*__err = uread_token(__bole, tok);
	return 0;
}

ff_byte_t static*
read_literal(struct ffly_bole *__bole, ff_u8_t *__kind, ff_err_t *__err) {
	struct token *tok = read_token(__bole, __err);
	if (_err(*__err)) {

	}

	*__err = FFLY_SUCCESS;
	if (tok->kind == _tok_no) {
		ff_u8_t neg = (*(char*)tok->p == '-');
		void *p;
		ff_u64_t no = ffly_stno(((char*)tok->p)+neg);
		if (neg)
			no = -no; 
		if (next_token_is(__bole, _tok_keyword, _tilde, __err)) {
			struct token *type = read_token(__bole, __err);
			ff_u8_t sign = *(((char*)type->end)-1) == 's';
			char *p = ((char*)type->p)+1;
			ff_uint_t size = (char*)type->p-(char*)type->end;
			if (size != 6 && size != 5) {
				// err
			}
			if (*p	== '6' && *(p+1) == '4') goto _64l;
			else if (*p  == '3' && *(p+1) == '2') goto _32l;
			else if (*p  == '1' && *(p+1) == '6') goto _16l;
			else if (*p  == '8') goto _8l;
			else {
				// err
			}
		}

		if (no >= 0 && no <= (ff_u8_t)~0) {
		_8l:
			*(ff_u8_t*)(p = __ffly_mem_alloc(sizeof(ff_u8_t))) = no;
			*__kind = neg?_ffly_bole_8l_s:_ffly_bole_8l_u;
			return (ff_byte_t*)p;
		} else if (no > (ff_u8_t)~0 && no <= (ff_u16_t)~0) {
		_16l:
			*(ff_u16_t*)(p = __ffly_mem_alloc(sizeof(ff_u16_t))) = no;
			*__kind = neg?_ffly_bole_16l_s:_ffly_bole_16l_u;
			return (ff_byte_t*)p;
		} else if (no > (ff_u16_t)~0 && no <= (ff_u32_t)~0) {
		_32l:
			*(ff_u32_t*)(p = __ffly_mem_alloc(sizeof(ff_u32_t))) = no;
			*__kind = neg?_ffly_bole_32l_s:_ffly_bole_32l_u;
			return (ff_byte_t*)p;
		} else if (no > (ff_u32_t)~0 && no <= ~(ff_u64_t)0) {
		_64l:
			*(ff_u64_t*)(p = __ffly_mem_alloc(sizeof(ff_u64_t))) = no;
			*__kind = neg?_ffly_bole_64l_s:_ffly_bole_64l_u;
			return (ff_byte_t*)p;
		}
		return NULL;
	}

	if (tok->kind == _tok_str)
		*__kind = _ffly_bole_str;
	else if (tok->kind == _tok_chr)
		*__kind = _ffly_bole_chr;
	return (ff_byte_t*)ffly_str_dupe((char*)tok->p); 
}

ff_err_t static
read_val(struct ffly_bole *__bole, struct ffly_bole_val **__val) {
	struct ffly_bole_val *val = (struct ffly_bole_val*)__ffly_mem_alloc(sizeof(struct ffly_bole_val));
	ff_err_t err = FFLY_SUCCESS;
	val->p = read_literal(__bole, &val->kind, &err);
	if (!val->p) {
		ffly_fprintf(ffly_err, "got null.\n");
		return FFLY_FAILURE;
	}

	if (_err(err)) return err;
	*__val = val;
	return FFLY_SUCCESS;
}

ff_err_t static
read_arr(struct ffly_bole *__bole, struct ffly_vec *__data, ff_uint_t *__l) {
	ff_err_t err;
	if (next_token_is(__bole, _tok_keyword, _r_bracket, &err)) {
		return FFLY_SUCCESS;
	}

	ffly_vec_set_flags(__data, VEC_AUTO_RESIZE);
	ffly_vec_init(__data, sizeof(void*));
	_again:
	(*__l)++;
	if (next_token_is(__bole, _tok_keyword, _l_bracket, &err)) {
		struct ffly_bole_arr **arr;
		ffly_vec_push_back(__data, (void**)&arr);

		*arr = (struct ffly_bole_arr*)__ffly_mem_alloc(sizeof(struct ffly_bole_arr));
		if (_err(err = read_arr(__bole, &(*arr)->data, &(*arr)->l))) {
			ffly_errmsg("failed to process sub array.\n");
			return err;
		}

		struct ffly_bole_entity ent = {
			.kind = _ffly_bole_arr,
			.p = arr
		};
		
		push_entity(__bole, &ent);
		goto _again;
	}

	{
		struct token *tok = peek_token(__bole, &err);
		struct ffly_bole_val *val;
		if (_err(err = read_val(__bole, &val))) {
			ffly_fprintf(ffly_out, "failed to read value.\n");
			return err;
		}

		void **p;
		ffly_vec_push_back(__data, (void**)&p);
		*p = val;

		struct ffly_bole_entity ent = {
			.kind = _ffly_bole_val,
			.p = val
		};

		push_entity(__bole, &ent);
	}

	if (next_token_is(__bole, _tok_keyword, _comma, &err)) {	
		goto _again;
	}

	if (!next_token_is(__bole, _tok_keyword, _r_bracket, &err)) {
		ffly_errmsg("missing right hand bracket on array.\n");
		return FFLY_FAILURE;
	}

	if (_err(err)) {
		ffly_errmsg("error has occurred while processing array.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

ff_err_t static
read_struct(struct ffly_bole *__bole, ffly_mapp __fields) {
	ff_err_t err;
	ffly_map_init(__fields, _ffly_map_127);
	if (!next_token_is(__bole, _tok_keyword, _r_brace, &err)) {
	_again:
		{
			struct token *name = read_token(__bole, &err);
			if (!expect_token(__bole, _tok_keyword, _colon, &err)) {
			
			}
			struct ffly_bole_val *val;
			read_val(__bole, &val);

			struct ffly_bole_entity ent;
			ent = (struct ffly_bole_entity) {
				.kind = _ffly_bole_val,
				.p = val
			};

			push_entity(__bole, &ent);

			struct ffly_bole_var *var = (struct ffly_bole_var*)__ffly_mem_alloc(sizeof(struct ffly_bole_var));
			*var = (struct ffly_bole_var) {
				.val = val,
				.name = name->p
			};

			ent = (struct ffly_bole_entity){
				.kind = _ffly_bole_var,
				.p = var
			};

			push_entity(__bole, &ent);
			ffly_printf("struct field: %s:%u\n", name->p, ffly_str_len((char*)name->p));
			ffly_map_put(__fields, (ff_u8_t*)name->p, ffly_str_len((char*)name->p), val);
		}		

		if (!expect_token(__bole, _tok_keyword, _comma, &err)) {

		}

		if (!next_token_is(__bole, _tok_keyword, _r_brace, &err))
			goto _again;
	}
}

ff_err_t static
read_decl(struct ffly_bole *__bole) {
	ff_err_t err;
	struct token *name = read_token(__bole, &err);
	if (_err(err)) return FFLY_FAILURE;
	if (!expect_token(__bole, _tok_keyword, _colon, &err)) {
		if (_err(err))
			return err;
		ffly_errmsg("missing colon separator.\n");
		return FFLY_FAILURE;
	}

	void *p;	
	if (next_token_is(__bole, _tok_keyword, _l_bracket, &err)) {
		struct ffly_bole_arr *arr = (struct ffly_bole_arr*)__ffly_mem_alloc(sizeof(struct ffly_bole_arr));
		if (_err(err = read_arr(__bole, &arr->data, &arr->l))) {
			ffly_fprintf(ffly_err, "failed to read array.\n");
			__ffly_mem_free(arr);
			return err;
		}

		arr->name = (char*)name->p;
		p = (void*)arr;
		struct ffly_bole_entity ent = {
			.kind = _ffly_bole_arr,
			.p = arr
		};

		push_entity(__bole, &ent);
	} else if (next_token_is(__bole, _tok_keyword, _l_brace, &err)) {
		struct ffly_bole_struct *struc = (struct ffly_bole_struct*)__ffly_mem_alloc(sizeof(struct ffly_bole_struct));
		read_struct(__bole, &struc->fields);
		struct ffly_bole_entity ent = {
			.kind = _ffly_bole_struct,
			.p = struc
		};

		p = (void*)struc;
		push_entity(__bole, &ent);
	} else {
		struct ffly_bole_val *val;
		if (_err(err = read_val(__bole, &val))) {
			ffly_fprintf(ffly_err, "failed to read value.\n");
			return err;
		}

		struct ffly_bole_entity ent;
		ent = (struct ffly_bole_entity){
			.kind = _ffly_bole_val,
			.p = val
		};

		push_entity(__bole, &ent);

		struct ffly_bole_var *var = (struct ffly_bole_var*)__ffly_mem_alloc(sizeof(struct ffly_bole_var));
		*var = (struct ffly_bole_var) {
			.val = val,
			.name = (char*)name->p
		};

		p = (void*)val;
		ent = (struct ffly_bole_entity){
			.kind = _ffly_bole_var,
			.p = var
		};
		
		push_entity(__bole, &ent);
	}

	ffly_map_put(&__bole->env, (ff_u8_t*)name->p, ffly_str_len((char*)name->p), p);
	if (!expect_token(__bole, _tok_keyword, _comma, &err)) {
		ffly_errmsg("expect error.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ff_err_t
ffly_bole_init(struct ffly_bole *__bole) {
	ff_err_t err;
	if (_err(err = ffly_buff_init(&__bole->sbuf, 56, 1))) {
		ffly_fprintf(ffly_err, "failed to init buff.\n");
		return err;
	}

	ffly_vec_set_flags(&__bole->toks, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__bole->toks, sizeof(struct token*)))) {
		ffly_fprintf(ffly_err, "failed to init vec.\n");
		return err;
	}

	if (_err(err = ffly_buff_init(&__bole->tokbuf, 8, sizeof(struct token*)))) {
		ffly_fprintf(ffly_err, "failed to init buff.\n");
		return err;
	}

	ffly_vec_set_flags(&__bole->entities, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__bole->entities, sizeof(struct ffly_bole_entity*)))) {
		ffly_fprintf(ffly_err, "failed to init vec.\n");
		return err;
	}

	ffly_vec_set_flags(&__bole->free, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__bole->free, sizeof(void*)))) {
		ffly_fprintf(ffly_err, "failed to init vec.\n");
		return err;
	}

	if (_err(err = ffly_map_init(&__bole->env, _ffly_map_127))) {
		ffly_fprintf(ffly_err, "failed to init map.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

struct token static*
peek_token(struct ffly_bole *__bole, ff_err_t *__err) {
	struct token *tok = read_token(__bole, __err);
	if (_err(*__err)) return NULL;
	*__err = uread_token(__bole, tok);
	return tok;
}


// rename to ffly_bole_process
ff_err_t
ffly_bole_read(struct ffly_bole *__bole) {
	ff_err_t err;
	struct token *tok;
	while(!is_eof(__bole)) {
		tok = peek_token(__bole, &err);
		if (!tok) break;

		if (_err(err)) {
			ffly_fprintf(ffly_err, "failed to peek token.\n");
			return FFLY_FAILURE;
		}
		if (tok->kind == _tok_ident) {
			if (_err(err = read_decl(__bole))) {
				ffly_fprintf(ffly_err, "failed to read decl.\n");
			}
		} else
			break;
		if (_err(err))
			return err;
	}
	return FFLY_SUCCESS;
}

# include "dep/mem_set.h"
ff_err_t
ffly_bole_ld(struct ffly_bole *__bole, char const *__file) {
	ff_err_t err;
	FF_FILE *f = ffly_fopen(__file, FF_O_RDONLY, 0, &err);
	struct ffly_stat st;
	ffly_fstat(__file, &st);

	__bole->p = (ff_byte_t*)__ffly_mem_alloc(st.size);
	ffly_fread(f, __bole->p, st.size);
	__bole->end = __bole->p+st.size;
	ffly_fclose(f);
	__bole->off = 0;
	return FFLY_SUCCESS; 
}

ff_err_t
ffbole_free(ffbolep __bole) {
	ff_err_t err;
	if (ffly_vec_size(&__bole->entities)>0) {
		struct ffly_bole_entity **itr = (struct ffly_bole_entity**)ffly_vec_begin(&__bole->entities);
		struct ffly_bole_entity *ent;
		while(itr <= (struct ffly_bole_entity**)ffly_vec_end(&__bole->entities)) {
			ent = *itr;
			switch(ent->kind) {
				case _ffly_bole_val:
					__ffly_mem_free(((struct ffly_bole_val*)ent->p)->p);
				break;
				case _ffly_bole_var:
				break;
				case _ffly_bole_arr:
					ffly_vec_de_init(&((struct ffly_bole_arr*)ent->p)->data);
				break;
				case _ffly_bole_struct:
					ffly_map_de_init(&((struct ffly_bole_struct*)ent->p)->fields);
				break;
			}
			__ffly_mem_free(ent->p);
			__ffly_mem_free(ent);
			itr++;
		}
	}

	if (_err(err = ffly_vec_de_init(&__bole->entities))) {
		ffly_fprintf(ffly_err, "failed to de-init vec.\n");
		return err;
	}

	if (ffly_vec_size(&__bole->free)>0) {
		void **itr = (void**)ffly_vec_begin(&__bole->free);
		while(itr <= (void**)ffly_vec_end(&__bole->free))
			__ffly_mem_free(*(itr++));
	}

	if (_err(err = ffly_vec_de_init(&__bole->free))) {
		ffly_fprintf(ffly_err, "failed to de-init vec.\n");
		return err;
	}

	if (_err(err = ffly_map_de_init(&__bole->env))) {
		ffly_fprintf(ffly_err, "failed to de-init map.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

ff_err_t
ffly_bole_free(struct ffly_bole *__bole) {
	ff_err_t err;
	if (_err(err = ffly_buff_de_init(&__bole->sbuf))) {
		ffly_fprintf(ffly_err, "failed to de-init buff.\n");
		return err;
	}

	if (ffly_vec_size(&__bole->toks)>0) {
		struct token *tok;
		struct token **itr = (struct token**)ffly_vec_begin(&__bole->toks);
		while(itr <= (struct token**)ffly_vec_end(&__bole->toks)) {
			tok = *(itr++);
			if (tok->p != NULL) {
				if ((ff_u8_t*)tok->p > __bole->end || (ff_u8_t*)tok->p < __bole->p)
					__ffly_mem_free(tok->p);
			}
			__ffly_mem_free(tok);
		}
	}

	if (_err(err = ffly_vec_de_init(&__bole->toks))) {
		ffly_fprintf(ffly_err, "failed to de-init vec.\n");
		return err;
	}

	if (_err(err = ffly_buff_de_init(&__bole->tokbuf))) {
		ffly_fprintf(ffly_err, "failed to de-init buff.\n");
		return err;
	}

	if (__bole->p != NULL)
		__ffly_mem_free(__bole->p);
	return FFLY_SUCCESS;
}

ff_u64_t
ffly_bole_int_u(void const *__val) {
	struct ffly_bole_val *val = (struct ffly_bole_val*)__val;
	switch(val->kind) {
		case _ffly_bole_64l_u:
			return *(ff_u64_t*)val->p;
		case _ffly_bole_32l_u:
			return *(ff_u32_t*)val->p;
		case _ffly_bole_16l_u:
			return *(ff_u16_t*)val->p;
		case _ffly_bole_8l_u: 
			return *(ff_u8_t*)val->p;
	}
	return 0;
}

ff_i64_t
ffly_bole_int_s(void const *__val) {
	struct ffly_bole_val *val = (struct ffly_bole_val*)__val;
	switch(val->kind) {
		case _ffly_bole_64l_s:
			return *(ff_i64_t*)val->p;
		case _ffly_bole_32l_s:
			return *(ff_i32_t*)val->p;
		case _ffly_bole_16l_s:
			return *(ff_i16_t*)val->p;
		case _ffly_bole_8l_s:
			return *(ff_i8_t*)val->p;
	}
	return 0;
}

void static print_val(struct ffly_bole_val *__val) {
	if (__val->kind == _ffly_bole_str)
		ffly_printf("%s\n", (char*)__val->p);
	else if (__val->kind == _ffly_bole_chr)
		ffly_printf("%c\n", *(char*)__val->p);
	else if (__val->kind == _ffly_bole_64l_u)
		ffly_printf("%lu\n", *(ff_u64_t*)__val->p);
	else if (__val->kind == _ffly_bole_32l_u)
		ffly_printf("%u\n", *(ff_u32_t*)__val->p);
	else if (__val->kind == _ffly_bole_16l_u)
		ffly_printf("%u\n", *(ff_u16_t*)__val->p);
	else if (__val->kind == _ffly_bole_8l_u)
		ffly_printf("%u\n", *(ff_u8_t*)__val->p);
	else if (__val->kind == _ffly_bole_64l_s)
		ffly_printf("%ld\n", *(ff_i64_t*)__val->p);
	else if (__val->kind == _ffly_bole_32l_s)
		ffly_printf("%d\n", *(ff_i32_t*)__val->p);
	else if (__val->kind == _ffly_bole_16l_s)
		ffly_printf("%d\n", *(ff_i16_t*)__val->p);
	else if (__val->kind == _ffly_bole_8l_s)
		ffly_printf("%d\n", *(ff_i8_t*)__val->p);
	else
		ffly_printf("unknown.\n");
}

//# define DEBUG
# ifdef DEBUG

ff_err_t ffmain(int __argc, char const *__argv[]) {
	struct ffly_bole bole;
	ffly_bole_init(&bole);
	ffly_bole_ld(&bole, "test.bole");
	ffly_bole_read(&bole);
	ffbole cf;
	ffly_bole_depos(&bole, &cf);

	void const *info = ffly_bole_get(&cf, "info");
	if (!info) {
		ffly_fprintf(ffly_out, "(null)\n");
		return -1;
	}

	void const *no = ffly_bole_get(&cf, "no");

	ffly_printf("val: %u\n", ffly_bole_int_u(no));

	print_val((struct ffly_bole_val*)info);
//	  print_val((struct ffly_bole_val*)no);
//	  void *in = ffly_bole_get_arr_elem(&bole, (void*)arr, 0);
//	  print_val((struct ffly_bole_val*)ffly_bole_get_arr_elem(&bole, in, 0));
 //   print_val((struct ffly_bole_val*)ffly_bole_get_arr_elem(&bole, arr, 1));
  //  print_val((struct ffly_bole_val*)ffly_bole_get_arr_elem(&bole, arr, 2));
   // print_val((struct ffly_bole_val*)ffly_bole_get_arr_elem(&bole, arr, 3));

	ffly_bole_free(&bole);
	ffbole_free(&cf);	 
}
# endif
