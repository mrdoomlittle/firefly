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
	cur->flags = 0x0;

	char buf[128];
	char *bufp;

	while(*p != '\0' && *p != ';') {
		while(*p == ' ') p++;
		if (*p == '\0') break;
		switch(*p) {
			case '#': {
				p++;
				ff_uint_t l;
				bufp = buf;
				while((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'z'))
					*(bufp++) = *(p++);
				*bufp = '\0';
				l = bufp-buf;
				cur->p = _memdup(buf, l+1);
				cur->sort = SY_LL;
				cur->len = l;
				break;
			}
			case '(':
				p++;
				cur->flags |= S_ADR;
			break;	
			case ')':
				p++;
			break;
			case '%': {
				p++;
				ff_uint_t l;
				bufp = buf;
				while(*p >= 'a' && *p <= 'z')
					*(bufp++) = *(p++);
				*bufp = '\0';
				l = bufp-buf;
				cur->p = _memdup(buf, l+1);
				cur->sort = SY_REG;
				cur->len = l;
				break;
			}
			case '$':
				p++;
				if (*p >= 'a' && *p <= 'z') {
					ff_uint_t l;
					bufp = buf;
					while((*p >= 'a' && *p <= 'z') || *p == '_' || (*p >= '0' && *p <= '9'))
						*(bufp++) = *(p++);
					*bufp = '\0';
					l = bufp-buf;
					cur->p = _memdup(buf, l+1);
					cur->sort = SY_LABEL;
					cur->len = l;
				} else if (isno(nextc(p))) {
					goto _no;
				}
			break;
			case ',': {
				p++;
				cur->next = _alloca(sizeof(struct symbol));
				cur = cur->next;
				cur->flags = 0x0;
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
				} else if (*p >= 'a' && *p <= 'z') {
					ff_uint_t l;
					bufp = buf;
					while((*p >= 'a' && *p <= 'z') || *p == '_' || (*p >= '0' && *p <= '9'))
						*(bufp++) = *(p++);
					*bufp = '\0';
					l = bufp-buf;
					cur->p = _memdup(buf, l+1);
					cur->sort = SY_UNKNOWN;
					cur->len = l;
				}
		}
	}

	cur->next = NULL;
	return head;
}
