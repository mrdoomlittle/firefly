# include "as.h"
# include "../ffly_def.h"

#define isno(__c) ((__c >= '0' && __c <= '9') || __c == '-')
#define is_next(__p, __c) \
	(*(__p+1) == __c)

# define nextc(__p) (*(__p+1))
symbolp ff_as_eval(char *__s) {
	char *p = __s;
	if (*p == '\n' || *p == '\0' || *p == ';') return NULL;
	symbolp head = ff_as_al(sizeof(struct symbol));
	symbolp cur = head;
	cur->flags = 0x0;

	char buf[128];
	char *bufp;

	while(*p != '\0' && *p != ';') {
		while(*p == ' ' || *p == '\t') p++;
		if (*p == '\0') break;
		switch(*p) {
			/*
				local label
			*/
			case '#': {
				p++;
				ff_uint_t l;
				bufp = buf;
				while((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'z'))
					*(bufp++) = *(p++);
				*bufp = '\0';
				l = bufp-buf;
				cur->p = ff_as_memdup(buf, l+1);
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
				while((*p >= 'a' && *p <= 'z') || (*p >= '0' && *p <= '9'))
					*(bufp++) = *(p++);
				*bufp = '\0';
				l = bufp-buf;
				cur->p = ff_as_memdup(buf, l+1);
				cur->sort = SY_REG;
				cur->len = l;
				break;
			}
			/*
				default label
			*/
			case '$':
				p++;
				if (*p >= 'a' && *p <= 'z') {
					ff_uint_t l;
					bufp = buf;
					while((*p >= 'a' && *p <= 'z') || *p == '_' || (*p >= '0' && *p <= '9'))
						*(bufp++) = *(p++);
					*bufp = '\0';
					l = bufp-buf;
					cur->p = ff_as_memdup(buf, l+1);
					cur->sort = SY_LABEL;
					cur->len = l;
				} else if (isno(nextc(p))) {
					goto _no;
				}
			break;
			case ',': {
				p++;
				cur->next = ff_as_al(sizeof(struct symbol));
				cur = cur->next;
				cur->flags = 0x0;
				break;
			}
			default:
				if (isno(*p)) _no: {
					ff_uint_t l;
					ff_u8_t sign;
					cur->p = ff_as_al(sizeof(ff_u64_t));
					*(ff_u64_t*)cur->p = ff_as_read_no(p, &l, &sign);
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
					cur->p = ff_as_memdup(buf, l+1);
					cur->sort = SY_UNKNOWN;
					cur->len = l;
				}
		}
	}

	cur->next = NULL;
	return head;
}
