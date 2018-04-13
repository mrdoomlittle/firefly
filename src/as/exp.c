# include "as.h"
# include "../ffly_def.h"

# define isno(__c) ((__c >= '0' && __c <= '9') || __c == '-')
# define is_next(__p, __c) \
	(*(__p+1) == __c)

# define nextc(__p) (*(__p+1))
symbolp eval(char *__s) {
	char *p = __s;
	if (*p == '\n' || *p == '\0' || *p == ';') return NULL;
	symbolp head = _alloca(sizeof(struct symbol));
	symbolp cur = head;

	while(*p != '\0' && *p != ';') {
		while(*p == ' ') p++;
		switch(*p) {
			case '%': {
				p++;
				ff_uint_t l;
				cur->p = read_str(p, &l);
				cur->sort = SY_REG;
				cur->len = l;
				p+=l;
				break;
			}
			case '$':
				p++;
				if (*p >= 'a' && *p <= 'z') {
					ff_uint_t l;
					cur->p = read_str(p, &l);
					cur->sort = SY_LABEL;
					cur->len = l;
					p+=l;
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
					ff_uint_t l;
					ff_u8_t sign;
					cur->p = _alloca(sizeof(ff_u64_t));
					*(ff_u64_t*)cur->p = read_no(p, &l, &sign);
					p+=l;
					cur->sort = SY_INT;
					if (sign)
						cur->flags |= SIGNED;
					cur->len = sizeof(ff_uint_t);
				} else if (*p >= 'a' && *p <= 'z') _str: {
					ff_uint_t l;
					cur->p = read_str(p, &l);	
					cur->sort = SY_STR;
					cur->len = l;
					p+=l;
				}
		}
	}

	cur->next = NULL;
	return head;
}
