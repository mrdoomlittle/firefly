# define __ffly_script_internal
# include "script.h"
# include "types/bool_t.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "data/str_dupe.h"
# include "data/str_len.h"
# include "system/string.h"
char static fetchc(struct ffly_script *__script) {
	return *(__script->p+__script->off);
}

char static nextc(struct ffly_script *__script) {
    return *(__script->p+__script->off+1);
}

ffly_bool_t is_next(struct ffly_script *__script, char __c) {
    return (*(__script->p+__script->off+1) == __c);
}

ffly_bool_t is_prev(struct ffly_script *__script, char __c) {
    return (*(__script->p+__script->off-1) == __c);
}

ffly_bool_t is_space(struct ffly_script *__script) {
    char c = fetchc(__script);
    mdl_u8_t static comment = 0;
    if (comment) {
        if (c == '/' && is_prev(__script, '*')) {
            comment = 0;
        }
        return 1;
    }

    if (c == '/' && is_next(__script, '*')) {
        comment = 1;
        return 1;
    }
    
    return (c == ' ' || c == '\t');
}

ffly_bool_t is_newline(struct ffly_script *__script) {
    return (fetchc(__script) == '\n');
}

char static* read_ident(struct ffly_script *__script) {
	char *itr = (char*)(__script->p+__script->off);
    char c = *itr;
	while((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9')) {
		ffly_buff_put(&__script->sbuf, itr++);
		ffly_buff_incr(&__script->sbuf);
        c = *itr;
	}

	char end = '\0';
	ffly_buff_put(&__script->sbuf, &end);
	char *s = ffly_str_dupe((char*)ffly_buff_begin(&__script->sbuf));
	__script->off+= ffly_buff_off(&__script->sbuf);
	ffly_buff_off_reset(&__script->sbuf);
    cleanup(__script, (void*)s);
	return s;
}

char static* read_no(struct ffly_script *__script, mdl_u8_t *__is_hex, mdl_u8_t *__is_float) {
	char *itr = (char*)(__script->p+__script->off);
    char c = *itr;
	while((c >= '0' && c <= '9') || c == '.' || c == '-' || ffly_tolow(c) == 'x' || (ffly_tolow(c) >= 'a' && ffly_tolow(c) <= 'f')) {
        if (ffly_tolow(*itr) == 'x') *__is_hex = 1;
        if (*itr == '.') *__is_float = 1;
		ffly_buff_put(&__script->sbuf, itr++);
		ffly_buff_incr(&__script->sbuf);
        c = *itr;
	}

	char end = '\0';
	ffly_buff_put(&__script->sbuf, &end);
	char *s = ffly_str_dupe((char*)ffly_buff_begin(&__script->sbuf));
	__script->off+= ffly_buff_off(&__script->sbuf);
	ffly_buff_off_reset(&__script->sbuf);
    cleanup(__script, (void*)s);
	return s;
}

char static* read_str(struct ffly_script *__script) {
    char *itr = (char*)(__script->p+__script->off);
    while(*itr != '"') {
        ffly_buff_put(&__script->sbuf, itr++);
        ffly_buff_incr(&__script->sbuf);
    }

    char end = '\0';
    ffly_buff_put(&__script->sbuf, &end);
    char *s = ffly_str_dupe((char*)ffly_buff_begin(&__script->sbuf));
    __script->off+= ffly_buff_off(&__script->sbuf);
    ffly_buff_off_reset(&__script->sbuf);
    cleanup(__script, (void*)s);
    return s;
}

void make_keyword(struct token *__tok, mdl_u8_t __id) {
	__tok->kind = TOK_KEYWORD;
	__tok->id = __id;
}

char* read_chr(struct ffly_script *__script) {
    return (char*)(__script->p+(__script->off++));
}

# define register_line(__script) \
    __script->line++; \
    __script->lo = __script->off+1;

static struct token* read_token(struct ffly_script *__script) {
	struct token *tok = (struct token*)__ffly_mem_alloc(sizeof(struct token));
	tok->p = NULL;
    if (is_newline(__script)) {
        register_line(__script);
        __script->off++;
        tok->kind = TOK_NEWLINE;  
        return tok;
    }

	switch(fetchc(__script)) {
        case '&':
            make_keyword(tok, _ampersand);
            __script->off++;
        break;
        case '\x27':
            __script->off++;
            *tok = (struct token) {
                .kind = TOK_CHR,
                .p = (void*)read_chr(__script) 
            };
            __script->off++;
        break;
        case '*':
            make_keyword(tok, _astrisk);
            __script->off++;
        break;
        case '+':
            if (is_next(__script, '+')) {
                make_keyword(tok, _incr);
                __script->off++;
            }
            __script->off++;
        break;
        case '"':
            __script->off++;
            *tok = (struct token) {
                .kind = TOK_STR,
                .p = (void*)read_str(__script)
            };
            __script->off++;
        break;
        case '%':
            make_keyword(tok, _percent);
            __script->off++;
        break;
        case ':':
            make_keyword(tok, _colon);
            __script->off++;
        break;
        case '.':
            if (is_next(__script, '.')) {
                __script->off++;
                if (is_next(__script, '.')) {
                    make_keyword(tok, _ellipsis);
                    __script->off++;
                }
            } else
                make_keyword(tok, _period);
            __script->off++;
        break;
        case '-':
            if (nextc(__script) >= '0' && nextc(__script) <= '9') goto _no;
            if (is_next(__script, '-')) {
                make_keyword(tok, _decr);
                __script->off++;
            } else if (is_next(__script, '>')) {
                make_keyword(tok, _r_arrow);
                __script->off++;
            }
            __script->off++;
        break;
        case ',':
            make_keyword(tok, _comma);
            __script->off++;
        break;
        case '<':
            if (is_next(__script, '-')) {
                make_keyword(tok, _l_arrow);
                __script->off++;
            } else
                make_keyword(tok, _lt);
            __script->off++;
        break;
        case '>':
            make_keyword(tok, _gt);
            __script->off++;
        break;
        case '!':
            if (is_next(__script, '=')) {
                make_keyword(tok, _neq);
                __script->off++;
            }
            __script->off++;
        break;
		case ';':
			make_keyword(tok, _semicolon);
			__script->off++;
		break;
		case '=':
            if (is_next(__script, '=')) {
                make_keyword(tok, _eqeq);
                __script->off++;
            } else
			    make_keyword(tok, _eq);
			__script->off++;
		break;
		case '(':
			make_keyword(tok, _l_paren);
			__script->off++;
		break;
		case ')':
			make_keyword(tok, _r_paren);
			__script->off++;
		break;
        case '{':
            make_keyword(tok, _l_brace);
            __script->off++;
        break;
        case '}':
            make_keyword(tok, _r_brace);
            __script->off++;
        break;
		default:
		if ((fetchc(__script) >= 'a' && fetchc(__script) <= 'z') || (fetchc(__script) >= 'A' && fetchc(__script) <= 'Z') || fetchc(__script) == '_') {
			*tok = (struct token) {
				.kind = TOK_IDENT,
				.p = (void*)read_ident(__script)
			};
		} else if (fetchc(__script) >= '0' && fetchc(__script) <= '9') goto _no;
		else {
		    __script->off++;
		    tok->kind = TOK_NULL;
        }
        break;
        _no:
        {
            mdl_u8_t is_float = 0, is_hex = 0;
            *tok = (struct token) {
                .kind = TOK_NO,
                .p = (void*)read_no(__script, &is_hex, &is_float)
            };
            tok->is_float = is_float;
            tok->is_hex = is_hex;
        }
	}
    
    tok->line = curl(__script)-1;
    tok->off = __script->off-1;
    tok->lo = curlo(__script);
	cleanup(__script, (void*)tok);
	return tok;
}

void ffly_script_ulex(struct ffly_script *__script, struct token *__tok) {
	if (!__tok) return;
	ffly_buff_put(&__script->iject_buff, (void*)&__tok);
	ffly_buff_incr(&__script->iject_buff);
}

struct token* ffly_script_lex(struct ffly_script *__script, ffly_err_t *__err) {
	if (ffly_buff_off(&__script->iject_buff)>0) {
		ffly_buff_decr(&__script->iject_buff);
		struct token *tok;
		ffly_buff_get(&__script->iject_buff, (void*)&tok);
		return tok;
	}

	while(is_space(__script) && !is_eof(__script)) {
        if (is_newline(__script)) {
            register_line(__script); 
        }
        __script->off++;
    }

    if (is_eof(__script)) return NULL;

	struct token *tok;
	while(!(tok = read_token(__script)) && !is_eof(__script));
	return tok;
}
