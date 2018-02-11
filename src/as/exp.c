# include "as.h"
# include "../ffly_def.h"
# include "../data/str_len.h"
void putsymbol(symbolp __sy) {
	hash_put(&symbols, __sy->p, ffly_str_len(__sy->p), __sy);
}

symbolp getsymbol(char *__s) {
	return (symbolp)hash_get(&symbols, __s, ffly_str_len(__s));
}

# define isno(__c) (__c >= '0' && __c <= '9')

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
			case '$':
				p++;
				if (isno(nextc(p))) {
					// grama error
				}
				goto _no;
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
					mdl_uint_t len;
					mdl_u8_t sign;
					cur->p = _alloca(sizeof(mdl_uint_t));
					*(mdl_uint_t*)cur->p = read_no(p, &len, &sign);
					p+= len;
					cur->sort = SY_INT;
					if (sign)
						cur->flags |= SIGNED;
					cur->len = sizeof(mdl_uint_t);
				} else if (*p >= 'a' && *p <= 'z') _str: {
					mdl_uint_t len;
					cur->p = read_str(p, &len);	
					cur->sort = SY_STR;
					putsymbol(cur);
					p+= len;
				}
		}
	}

	cur->next = NULL;
	return head;
}
