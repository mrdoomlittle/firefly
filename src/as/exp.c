# include "as.h"
# include "../ffly_def.h"
# include "../dep/str_len.h"
void putsymbol(symbolp __sy) {
	hash_put(&symbols, __sy->p, __sy->len, __sy);
}

symbolp getsymbol(char *__s) {
	return (symbolp)hash_get(&symbols, __s, ffly_str_len(__s));
}

# define isno(__c) ((__c >= '0' && __c <= '9') || __c == '-')

# define is_next(__p, __c) \
	(*(__p+1) == __c)

# define nextc(__p) (*(__p+1))
symbolp eval(char *__s) {
	char *p = __s;
	if (*p == '\n' || *p == '\0') return NULL;
	symbolp head = _alloca(sizeof(struct symbol));
	symbolp cur = head;

	while(*p != '\0') {
		while(*p == ' ') p++;
		switch(*p) {
			case '%': {
				p++;
				mdl_uint_t l;
				cur->p = read_str(p, &l);
				cur->sort = SY_REG;
				cur->len = l;
				p+=l;
				break;
			}
			case '$':
				p++;
				if (*p >= 'a' && *p <= 'z') {
					goto _str;
				} else if (isno(nextc(p))) {
					goto _no;
				}
			break;
			case ',': {
				p++;
				cur->next = _alloca(sizeof(struct symbol));
				cur = cur->next;
				if (*p >= 'a' && *p <= 'z') {
					goto _str;
				}
				break;
			}
			default:
				if (isno(*p)) _no: {
					mdl_uint_t l;
					mdl_u8_t sign;
					cur->p = _alloca(sizeof(mdl_u64_t));
					*(mdl_u64_t*)cur->p = read_no(p, &l, &sign);
					p+=l;
					cur->sort = SY_INT;
					if (sign)
						cur->flags |= SIGNED;
					cur->len = sizeof(mdl_uint_t);
				} else if (*p >= 'a' && *p <= 'z') _str: {
					mdl_uint_t l;
					cur->p = read_str(p, &l);	
					cur->sort = SY_STR;
					cur->len = l;
					putsymbol(cur);
					p+=l;
				}
		}
	}

	cur->next = NULL;
	return head;
}
