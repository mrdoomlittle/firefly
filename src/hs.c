# include "hs.h"
# include "system/io.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../linux/fcntl.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "hs/lexer.h"
# include "hs/memalloc.h"
# include "hs/parser.h"
void ff_hs_init(ff_hsp __hs) {
	__hs->p = NULL;
	__hs->end = 0;
	__hs->off = 0;
}

void ff_hs_load(ff_hsp __hs, char const *__file) {
	int fd;

	fd = open(__file, O_RDONLY, 0);
	if (fd == -1) {
		// error
		return;
	}

	struct stat st;
	fstat(fd, &st);

	read(fd, __hs->p = (ff_u8_t*)__ffly_mem_alloc(st.st_size), st.st_size);
	__hs->end = st.st_size;
	ffly_printf("size: %u\n", st.st_size);
	close(fd);
}

void ff_hs_clean(ff_hsp __hs) {
	ff_hs_mem_clean();
	__ffly_mem_free(__hs->p);
}

# include "hs/gen.h"
ff_hs_objp ff_hs_build(ff_hsp __hs) {
	ff_u8_t tok;

	ff_hs_nodep n;
	ff_hs_nodep top = NULL;
	ff_hs_nodep end = NULL;
	hs_hash_init(&__hs->env);
	ffly_printf("building.\n");
	while(!ff_hs_at_eof(__hs)) {
		if (!(n = ff_hs_parse(__hs))) {
			ffly_printf("null.\n");
			break;
		}

		if (!top)
			top = n;
		if (end != NULL)
			end->next = n;
		end = n;
	}

	if (!end)
		return NULL;
	end->next = NULL;
	ffly_printf("%u-%u\n", __hs->off, __hs->end);
	
	return ff_hs_gen(top);
}

char const* nks(ff_u8_t __kind) {
	switch(__kind) {
		case _func:
			return "func";
		case _decl:
			return "decl";
	}
	return "what?";
}

char const* tids(ff_u8_t __id) {
	switch(__id) {
		case _object:
			return "object";
		case _model:
			return "model";
		case _struct:
			return "struct";
	}
	return "what?";
}

# include "physics/body.h"
# include "obj.h"
# include "model.h"
ff_u8_t static tsz[] = {
	sizeof(struct ffly_obj*),
	sizeof(ffly_model*),
	sizeof(ffly_phy_bodyp),
	sizeof(ff_uint_t),
	0,
	sizeof(ff_int_t)
};

ff_u8_t ff_hs_tsize(ff_u8_t __id) {
	return tsz[__id];
}

ff_u8_t ff_hs_nexttokis(ff_hsp __hs, ff_u8_t __kind, ff_u8_t __id) {
	ff_u8_t tok;

	tok = hs_nexttok(__hs);

	if (tok == __kind && hs_tok_val == __id) {
		return 1;
	}
	hs_rtok(tok);
	return 0;
}

ff_u8_t ff_hs_reckon(ff_hsp __hs, ff_u8_t __kind, ff_u8_t __id) {
	ff_u8_t tok;

	tok = hs_nexttok(__hs);
	if (tok == __kind && hs_tok_val == __id)
		return 1;
	ffly_printf("%u ? %u - %u ? %u\n", tok, __kind, hs_tok_val, __id);
	return 0;
}

# include "dep/str_cmp.h"
ff_u8_t ff_hs_perhaps_keywd(ff_u8_t __tok) {
	if (__tok == _tok_ident) {
		if (!ffly_str_cmp(hs_tok_data, "func")) {
			hs_tok_val = _keywd_func;
			goto _aff;
		} else if (!ffly_str_cmp(hs_tok_data, "object")) {
			hs_tok_val = _keywd_object;
			goto _aff;
		} else if (!ffly_str_cmp(hs_tok_data, "model")) {
			hs_tok_val = _keywd_model;
			goto _aff;
		} else if (!ffly_str_cmp(hs_tok_data, "ret")) {
			hs_tok_val = _keywd_ret;
			goto _aff;
		} else if (!ffly_str_cmp(hs_tok_data, "struct")) {
			hs_tok_val = _keywd_struct;
			goto _aff;
		} else if (!ffly_str_cmp(hs_tok_data, "uint_t")) {
			hs_tok_val = _keywd_uint_t;
			goto _aff;
		} else if (!ffly_str_cmp(hs_tok_data, "int_t")) {
			hs_tok_val = _keywd_int_t;
			goto _aff;
		} else if (!ffly_str_cmp(hs_tok_data, "body")) {
			hs_tok_val = _keywd_body;
			goto _aff;
		} else if (!ffly_str_cmp(hs_tok_data, "fset")) {
			hs_tok_val = _keywd_fset;
			goto _aff;
		} else if (!ffly_str_cmp(hs_tok_data, "fget")) {
			hs_tok_val = _keywd_fget;
			goto _aff;
		}
	}

	return __tok;
_aff:
	return _tok_keywd;
}

ff_u8_t static bufc;
ff_i8_t static pres = -1;

void ff_hs_ugetc(ff_hsp __hs, char __c) {
	bufc = __c;
	pres = 0;
}

char ff_hs_getc(ff_hsp __hs) {
	if (!pres) {
		pres = -1;
		return bufc;
	}
	return *(__hs->p+(__hs->off++));
}

ff_u8_t ff_hs_at_eof(ff_hsp __hs) {
	return __hs->off>=__hs->end;
}
/*
# include "hs/exec.h"
ff_err_t ffmain(int __argc, char const **__argv) {
	struct ff_hs h;
	ff_hs_init(&h);
	ff_hs_load(&h, "main.ffhs");
	ff_hs_objp o;
	o = ff_hs_build(&h);
	void *obj0, *obj1;
	void *fins[] = {
		&obj0,
		&obj1
	};

	if (o != NULL) {
		ff_hs_run(o, fins);
	}
	ffly_printf(">>>> %p, %p\n", obj0, obj1);
	ff_hs_clean(&h);
}*/
