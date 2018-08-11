# include "parser.h"
# include "lexer.h"
# include "../ffly_def.h"
# include "../system/io.h"
# include "memalloc.h"
# include "../system/string.h"
# define newnode \
	(ff_hs_nodep)ff_hs_mem_alloc(sizeof(struct ff_hs_node))
# define newtype \
	(ff_hs_typep)ff_hs_mem_alloc(sizeof(struct ff_hs_type))


struct ff_hs_type static *uint_t = &(struct ff_hs_type){.id=_uint_t, .size=sizeof(ff_uint_t)};
struct ff_hs_type static *int_t = &(struct ff_hs_type){.id=_int_t, .size=sizeof(ff_int_t)};

ff_u8_t static
is_type(void) {
	return (tok_val == _keywd_object || tok_val == _keywd_model
		|| tok_val == _keywd_struct || tok_val == _keywd_body);
}

void static decl_spec(ff_hsp, ff_hs_typep*, ff_u8_t);
ff_hs_nodep static parser_decl(ff_hsp);

ff_hs_nodep static
parser_ret_stmt(ff_hsp __hs) {
	ff_hs_nodep n;
	n = newnode;
	n->kind = __ret;
	return n;
}

void static
parser_compound_stmt(ff_hsp __hs, ff_hs_vecp __vec) {
	ff_hs_nodep n;
	ff_u8_t tok;

	n = NULL;
	if (ff_hs_nexttokis(__hs, _tok_keywd, _l_brace)) {	
	_again:
		if (ff_hs_nexttokis(__hs, _tok_keywd, _r_brace)) {
			return;
		}
    	tok = ff_hs_perhaps_keywd(nexttok(__hs));
		if (tok == _tok_keywd) {
			if (is_type()) {
				rtok(tok);
				n = parser_decl(__hs);
			}

			switch(tok_val) {
				case _keywd_ret:
					n = parser_ret_stmt(__hs);
				break;
			}
		}
		if (n != NULL)
			*(ff_hs_nodep*)ff_hs_vec_push(__vec) = n;
		goto _again;
	}
}

ff_hs_nodep static
parser_func(ff_hsp __hs) {
	char const *name;

	nexttok(__hs);
	name = (char const*)tok_data;
	ff_hs_nodep n;
	n = newnode;
	hash_put(&__hs->env, name, tok_sz-1, n);
	struct ff_hs_vec body;

	ffly_printf("func: %s\n", name);
	ff_hs_vec_init(&body, sizeof(ff_hs_nodep));

	if (!ff_hs_reckon(__hs, _tok_keywd, _l_bracket)) {
		// error
		ffly_printf("misssing left side bracket.\n");
	}

	if (!ff_hs_reckon(__hs, _tok_keywd, _r_bracket)) {
		// error
		ffly_printf("missing right side bracket.\n");
	}

	if (!ff_hs_reckon(__hs, _tok_keywd, _semicolon)) {
		// error
		ffly_printf("missing semicolon.\n");
	}

	parser_compound_stmt(__hs, &body);	
	
	n->vec = body;
	n->kind = _func;
	return n;
}

void
parser_struct_decl(ff_hsp __hs, struct hash *__fields, ff_uint_t *__size) {
	if (!ff_hs_reckon(__hs, _tok_keywd, _l_brace)) {
		ffly_printf("missing left side brace.\n");
	}

	ff_uint_t size;
	
	size = 0;
	if (!ff_hs_nexttokis(__hs, _tok_keywd, _r_brace)) {
		ff_hs_typep t;
		ff_u8_t tok;
	_again:
		decl_spec(__hs, &t, ff_hs_perhaps_keywd(nexttok(__hs)));
		t->offset = size;
		size+=t->size;

		nexttok(__hs);
		hash_put(__fields, (ff_u8_t const*)tok_data, tok_sz-1, t);
		

		ffly_printf("field: %s\n", tok_data);
		if (ff_hs_nexttokis(__hs, _tok_keywd, _comma)) {
			goto _again;
		}

		if (!ff_hs_reckon(__hs, _tok_keywd, _r_brace)) {
			ffly_printf("missing right side brace.\n");
		}
	}

	*__size = size;
}

void
parser_struct_spec(ff_hsp __hs, ff_hs_typep __t) {
	nexttok(__hs);
	ffly_printf("struct %s\n", tok_data);
	hash_put(&__hs->env, (ff_u8_t const*)tok_data, tok_sz-1, __t);
	struct hash fields;
	hash_init(&fields);
	
	ff_uint_t size;
	parser_struct_decl(__hs, &fields, &size);
	__t->fields = fields;
	__t->size = size;
	ffly_printf("struct size: %u\n", size);
}

void
decl_spec(ff_hsp __hs, ff_hs_typep *__ty, ff_u8_t __tok) {
	if (__tok == _tok_keywd) {
		ff_hs_typep t;
		*__ty = (t = newtype);
		switch(tok_val) {
			case _keywd_int_t:
				t->id = _int_t;
				t->size = ff_hs_tsize(t->id);
			break;
			case _keywd_uint_t:
				t->id = _uint_t;
				t->size = ff_hs_tsize(t->id);
			break;
			case _keywd_object:
				t->id = _object;
				t->size = ff_hs_tsize(t->id);
			break;
			case _keywd_model:
				t->id = _model;
				t->size = ff_hs_tsize(t->id);
			break;
			case _keywd_body:
				t->id = _body;
				t->size = ff_hs_tsize(t->id);
			break;
			case _keywd_struct:
				t->id = _struct;
				parser_struct_spec(__hs, t);
			break;
		}
	} else 
		ffly_printf("error. %u\n", __tok);
}

ff_hs_nodep
parser_decl(ff_hsp __hs) {
	ff_hs_typep t;

	t = NULL;
	decl_spec(__hs, &t, ff_hs_perhaps_keywd(nexttok(__hs)));
	if (ff_hs_nexttokis(__hs, _tok_keywd, _semicolon))
		return NULL;
	if (!t) {
		ffly_printf("type error.\n");
		return NULL;
	}

	char const *name;
	nexttok(__hs);
	name = (char const*)tok_data;

	ffly_printf("func: %s, type: %s, name: %s\n", nks(_decl), tids(t->id), name);

	ff_hs_nodep n;
	n = newnode;
	n->var = newnode;
	n->var->kind = _var;
	n->var->t = t;
	n->kind = _decl;
	hash_put(&__hs->env, name, tok_sz-1, n->var);
	return n;
}

ff_hs_nodep
parser_func_call(ff_hsp __hs) {
	char const *name;
	ff_u8_t tok;
	tok = nexttok(__hs);

	name = (char const*)tok_data;
	ff_hs_nodep n;
	n = newnode;
	n->func = (ff_hs_nodep)hash_get(&__hs->env, name, tok_sz-1);
	n->kind = _func_call;
	return n;
}

ff_u8_t static
is_rin(ff_hsp __hs) {
	if (tok_sz == 2 && *(char*)tok_data == 'r') {
		if (!ff_hs_reckon(__hs, _tok_keywd, _l_brace)) {
			return 0;
		}
		return 1;	
	}
	return 0;
}

void static
read_no(ff_hsp __hs, ff_hs_nodep *__n) {
	ff_hs_nodep n;
	n = newnode;
	n->kind = _literal;
	*(n->t = newtype) = *uint_t;
	n->val = ffly_stno((char const*)tok_data);
	*__n = n;
}

ff_hs_nodep
parser_primary_exp(ff_hsp __hs) {
	ff_u8_t tok;
	tok = nexttok(__hs);

	ff_hs_nodep n;
	if (tok == _tok_ident) {
		n = (ff_hs_nodep)hash_get(&__hs->env, (ff_u8_t const*)tok_data, tok_sz-1);
		if (!n) {
			// error
			ffly_printf("no such function or var.\n");
			goto _fail;
		}
	} else if (tok == _tok_no) {
		read_no(__hs, &n);		
	} else {
		ffly_printf("unknown.\n");
		goto _fail;
	}
	return n;
_fail:
	rtok(tok);
	return NULL;
}

void
parser_struct_field(ff_hsp __hs, ff_hs_nodep *__n, ff_hs_nodep __struc) {
	if (!__struc) {
		ffly_printf("error.\n");
	}
	nexttok(__hs);
	ff_hs_typep t;
	t = (ff_hs_typep)hash_get(&__struc->t->fields, (ff_u8_t const*)tok_data, tok_sz-1);
	if (!t) {
		ffly_printf("no field called %s found.\n", tok_data);
	}

	ff_hs_nodep n;
	n = newnode;
	n->kind = _struct_ref;
	n->t = t;
	n->struc = __struc;
	*__n = n;
}

ff_hs_nodep parser_exp(ff_hsp);

ff_hs_nodep
parser_assign_exp(ff_hsp __hs, ff_hs_nodep __l) {
	ff_hs_nodep n;
	n = newnode;
	n->kind = _assign;
	n->l = __l;
	n->r = parser_exp(__hs);
	n->t = __l->t;
	return n;
}

ff_hs_nodep
parser_exp(ff_hsp __hs) {
	ff_hs_nodep n;
	ff_u8_t tok;
	tok = nexttok(__hs);
	ffly_printf("---> %s\n", tok_data);
	if (is_rin(__hs)) {
		n = newnode;
		n->kind = _rin;
		struct ff_hs_vec params;
		ff_hs_vec_init(&params, sizeof(ff_hs_nodep));
	_again:
		if (ff_hs_nexttokis(__hs, _tok_keywd, _r_brace)) {
			n->params = params;
			return n;
		}
		{
			ff_hs_nodep n;
			n = parser_exp(__hs);
			if (n != NULL)
				*(ff_hs_nodep*)ff_hs_vec_push(&params) = n;
		}
		if (ff_hs_nexttokis(__hs, _tok_keywd, _comma)) {

		}
		goto _again;
	}
	rtok(tok);

	n = parser_primary_exp(__hs);

	if (ff_hs_nexttokis(__hs, _tok_keywd, _period)) {
		parser_struct_field(__hs, &n, n);
	}

	if (ff_hs_nexttokis(__hs, _tok_keywd, _eq)) {
		n = parser_assign_exp(__hs, n);
	}
	return n;
}

ff_hs_nodep
parser_fset(ff_hsp __hs) {
	ff_hs_nodep n;
	n = newnode;
	n->kind = _fset;
	n->l = parser_exp(__hs);
	if (!ff_hs_reckon(__hs, _tok_keywd, _comma)) {

	}
	n->r = parser_exp(__hs);
	if (!ff_hs_reckon(__hs, _tok_keywd, _comma)) {

	}
	n->sz = parser_exp(__hs);
	return n;
}

ff_hs_nodep
parser_fget(ff_hsp __hs) {
	ff_hs_nodep n;
	n = newnode;
	n->kind = _fget;
	n->l = parser_exp(__hs);
	if (!ff_hs_reckon(__hs, _tok_keywd, _comma)) {

	}
	n->r = parser_exp(__hs);
	if (!ff_hs_reckon(__hs, _tok_keywd, _comma)) {

	}
	n->sz = parser_exp(__hs);
	return n;
}

# include "../dep/str_cmp.h"
ff_hs_nodep
ff_hs_parse(ff_hsp __hs) {
	ff_u8_t tok;
	ff_hs_nodep n;

	tok = ff_hs_perhaps_keywd(nexttok(__hs));
	if (tok == _null)
		return NULL;
	ffly_printf("tok, %u, %u, %u, %s\n", tok, tok_val, tok_sz, (char const*)tok_data);	
	if (tok == _tok_keywd) {
		switch(tok_val) {
			case _keywd_func:
				n = parser_func(__hs);
				goto _sk;
			case _pound:
				n = parser_func_call(__hs);
				goto _sk;
			case _keywd_fset:
				n = parser_fset(__hs);
				goto _sk;
			case _keywd_fget:
				n = parser_fget(__hs);
				goto _sk;
		}

		if (is_type()) {
			rtok(tok);
			n = parser_decl(__hs);
		} else
			ffly_printf("unknown, %u\n", tok_val);
	} else {
		rtok(tok);
		n = parser_exp(__hs);
		if (!n) {
			ffly_printf("error.\n");
			return NULL;
		}
	}
_sk:
	return n;
}
