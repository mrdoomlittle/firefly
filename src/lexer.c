# define __ffly_compiler_internal
# define __ffly_lexer
# include "compiler.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "dep/str_dup.h"
# include "dep/str_len.h"
# include "system/string.h"
char static
fetchc(struct ffly_compiler *__compiler) {
	if (at_eof(__compiler)) return '\0';
	return *(__compiler->file->p+__compiler->file->off);
}

char static
nextc(struct ffly_compiler *__compiler) {
    return *(__compiler->file->p+__compiler->file->off+1);
}

ff_bool_t static
is_next(struct ffly_compiler *__compiler, char __c) {
    return (*(__compiler->file->p+__compiler->file->off+1) == __c);
}

ff_bool_t static
is_prev(struct ffly_compiler *__compiler, char __c) {
    return (*(__compiler->file->p+__compiler->file->off-1) == __c);
}

ff_bool_t static
is_space(struct ffly_compiler *__compiler) {
    char c;
	if ((c = fetchc(__compiler)) == '\0') {
		// something may have gone terribly wong
	}

    ff_i8_t static comment = 0;
    if (comment>0) {
		if (c == '/' && is_prev(__compiler, '*'))
			comment = 0;
        return 1;
    } else if (comment<0) {
		if (c == '\n')
			comment = 0;
		return 1;
	}

    if (c == '/' && is_next(__compiler, '*')) {
        comment = 1;
        return 1;
    } else if (c == '/' && is_next(__compiler, '/')) {
		comment = -1;
		return 1;
	}
    
    return (c == ' ' || c == '\t');
}

ff_bool_t static
at_nl(struct ffly_compiler *__compiler) {
    return (fetchc(__compiler) == '\n');
}

char static*
read_ident(struct ffly_compiler *__compiler) {
	char *p = (char*)(__compiler->file->p+__compiler->file->off);
    char c = *p;
	while((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9')) {
		ffly_buff_put(&__compiler->sbuf, p++);
		ffly_buff_incr(&__compiler->sbuf);
        c = *p;
	}

	char end = '\0';
	ffly_buff_put(&__compiler->sbuf, &end);
	char *s = ffly_str_dupe((char*)ffly_buff_begin(&__compiler->sbuf));
	__compiler->file->off+= ffly_buff_off(&__compiler->sbuf);
	ffly_buff_off_reset(&__compiler->sbuf);
    cleanup(__compiler, (void*)s);
	return s;
}

char static* 
read_no(struct ffly_compiler *__compiler, ff_u8_t *__is_hex, ff_u8_t *__is_float) {
	char *p = (char*)(__compiler->file->p+__compiler->file->off);
    char c = *p;
	while((c >= '0' && c <= '9') || c == '.' || c == '-' || ffly_tolow(c) == 'x' || (ffly_tolow(c) >= 'a' && ffly_tolow(c) <= 'f')) {
        if (ffly_tolow(*p) == 'x') *__is_hex = 1;
        if (*p == '.') *__is_float = 1;
		ffly_buff_put(&__compiler->sbuf, p++);
		ffly_buff_incr(&__compiler->sbuf);
        c = *p;
	}

	char end = '\0';
	ffly_buff_put(&__compiler->sbuf, &end);
	char *s = ffly_str_dupe((char*)ffly_buff_begin(&__compiler->sbuf));
	__compiler->file->off+= ffly_buff_off(&__compiler->sbuf);
	ffly_buff_off_reset(&__compiler->sbuf);
    cleanup(__compiler, (void*)s);
	return s;
}

char static* 
read_str(struct ffly_compiler *__compiler) {
    char *p = (char*)(__compiler->file->p+__compiler->file->off);
    while(*p != '"') {
        ffly_buff_put(&__compiler->sbuf, p++);
        ffly_buff_incr(&__compiler->sbuf);
    }

    char end = '\0';
    ffly_buff_put(&__compiler->sbuf, &end);
    char *s = ffly_str_dupe((char*)ffly_buff_begin(&__compiler->sbuf));
    __compiler->file->off+= ffly_buff_off(&__compiler->sbuf);
    ffly_buff_off_reset(&__compiler->sbuf);
    cleanup(__compiler, (void*)s);
    return s;
}

void static
mk_keywd(struct token *__tok, ff_u8_t __id) {
	__tok->kind = TOK_KEYWORD;
	__tok->id = __id;
}

char static*
read_chr(struct ffly_compiler *__compiler) {
    return (char*)(__compiler->file->p+(__compiler->file->off++));
}

# define register_line(__compiler) \
    __compiler->file->line++; \
    __compiler->file->lo = __compiler->file->off+1;

# define incr_off __compiler->file->off++
struct token static*
read_token(struct ffly_compiler *__compiler) {
	struct token *tok;
	if (!(tok = (struct token*)__ffly_mem_alloc(sizeof(struct token)))) {
		// memory allocation failure
	}

	tok->p = NULL;
    if (at_nl(__compiler)) {
        register_line(__compiler);
        incr_off;
        tok->kind = TOK_NEWLINE;  
        return tok;
    }

	char c;
	switch(c = fetchc(__compiler)) {
        case '&':
            mk_keywd(tok, _ampersand);
            incr_off;
        break;
        case '\x27':
            incr_off;
            *tok = (struct token) {
                .kind = TOK_CHR,
                .p = (void*)read_chr(__compiler) 
            };
            incr_off;
        break;
        case '*':
            mk_keywd(tok, _astrisk);
            incr_off;
        break;
        case '+':
            if (is_next(__compiler, '+')) {
                mk_keywd(tok, _incr);
                incr_off;
            } else
                mk_keywd(tok, _plus);
            incr_off;
        break;
        case '"':
            incr_off;
            *tok = (struct token) {
                .kind = TOK_STR,
                .p = (void*)read_str(__compiler)
            };
            incr_off;
        break;
        case '%':
            mk_keywd(tok, _percent);
            incr_off;
        break;
        case ':':
            mk_keywd(tok, _colon);
            incr_off;
        break;
        case '.':
            if (is_next(__compiler, '.')) {
                incr_off;
                if (is_next(__compiler, '.')) {
                    mk_keywd(tok, _ellipsis);
                    incr_off;
                }
            } else
                mk_keywd(tok, _period);
            incr_off;
        break;
        case '-':
            if (nextc(__compiler) >= '0' && nextc(__compiler) <= '9') goto _no;
            if (is_next(__compiler, '-')) {
                mk_keywd(tok, _decr);
                incr_off;
            } else if (is_next(__compiler, '>')) {
                mk_keywd(tok, _r_arrow);
                incr_off;
            } else
                mk_keywd(tok, _minus);
            incr_off;
        break;
        case ',':
            mk_keywd(tok, _comma);
            incr_off;
        break;
        case '<':
            if (is_next(__compiler, '-')) {
                mk_keywd(tok, _l_arrow);
                incr_off;
            } else
                mk_keywd(tok, _lt);
            incr_off;
        break;
        case '>':
            mk_keywd(tok, _gt);
            incr_off;
        break;
        case '!':
            if (is_next(__compiler, '=')) {
                mk_keywd(tok, _neq);
                incr_off;
            }
            incr_off;
        break;
		case ';':
			mk_keywd(tok, _semicolon);
			incr_off;
		break;
		case '=':
            if (is_next(__compiler, '=')) {
                mk_keywd(tok, _eqeq);
                incr_off;
            } else
			    mk_keywd(tok, _eq);
			incr_off;
		break;
		case '(':
			mk_keywd(tok, _l_paren);
			incr_off;
		break;
		case ')':
			mk_keywd(tok, _r_paren);
			incr_off;
		break;
        case '{':
            mk_keywd(tok, _l_brace);
            incr_off;
        break;
        case '}':
            mk_keywd(tok, _r_brace);
            incr_off;
        break;
		default:
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
			*tok = (struct token) {
				.kind = TOK_IDENT,
				.p = (void*)read_ident(__compiler)
			};
		} else if (c >= '0' && c <= '9') goto _no;
		else {
		    incr_off;
		    tok->kind = TOK_NULL;
        }
        break;
        _no:
        {
            ff_u8_t is_float = 0, is_hex = 0;
            *tok = (struct token) {
                .kind = TOK_NO,
                .p = (void*)read_no(__compiler, &is_hex, &is_float)
            };
            tok->is_float = is_float;
            tok->is_hex = is_hex;
        }
	}
    
    tok->line = curl(__compiler)-1;
    tok->off = __compiler->file->off-1;
    tok->lo = curlo(__compiler);
	cleanup(__compiler, (void*)tok);
	return tok;
}

void ffly_ulex(struct ffly_compiler *__compiler, struct token *__tok) {
	if (!__tok) return;
	ffly_buff_put(&__compiler->iject_buff, (void*)&__tok);
	ffly_buff_incr(&__compiler->iject_buff);
}

struct token*
ffly_lex(struct ffly_compiler *__compiler, ff_err_t *__err) {
	if (ffly_buff_off(&__compiler->iject_buff)>0) {
		ffly_buff_decr(&__compiler->iject_buff);
		struct token *tok;
		ffly_buff_get(&__compiler->iject_buff, (void*)&tok);
		return tok;
	}

	while(is_space(__compiler) && !at_eof(__compiler)) {
        if (at_nl(__compiler)) {
            register_line(__compiler); 
        }
        incr_off;
    }

    if (at_eof(__compiler)) return NULL;

	struct token *tok;
	while(!(tok = read_token(__compiler)) && !at_eof(__compiler));
	return tok;
}
