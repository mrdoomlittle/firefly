# include "env.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "system/util/hash.h"
# include "dep/mem_dup.h"
# include "dep/mem_cmp.h"
# include "dep/str_len.h"
# include "dep/mem_cpy.h"
# include "system/io.h"
char static **env = NULL;
ff_uint_t static off = 0;
struct entry {
	ff_u8_t const *key;
	ff_uint_t len;
	char *p;
	ff_uint_t vl;
	ff_uint_t env, ol;
	struct entry *fd, *next;
};

static struct entry **table;
static struct entry *top = NULL;

void envinit(void) {
	table = (struct entry**)__ffly_mem_alloc(0x100*sizeof(struct entry*));
	struct entry **cur = table;
	while(cur != table+0x100)
		*(cur++) = NULL;
}

void envcleanup(void) {
	if (table != NULL)
		__ffly_mem_free(table);

	struct entry *cur = top, *bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->fd;
		__ffly_mem_free(bk->key);
		__ffly_mem_free(bk);
	}

	char **p = env;
	while(p != env+off) {
		//ffly_printf("%s\n", *p);
		__ffly_mem_free(*(p++));
	}

	if (env != NULL)
		__ffly_mem_free(env);
}

void envput(char const*, char const*);
void envload(char const **__envp) {
	char const **cur = __envp;
	char const *p;
	char buf[128];
	char *bufp;
	while(*cur != NULL) {
		p = *cur;
		bufp = buf;
		while(*p != '=')
			*(bufp++) = *(p++);
		*bufp = '\0';
		p++;
		envput(buf, p);
		cur++;
	}
}

static struct entry*
lookup(char const *__key) {
	ff_uint_t len = ffly_str_len(__key);
	ff_u64_t sum = ffly_hash(__key, len);
	struct entry *cur = *(table+(sum&0xff));
	while(cur != NULL) {
		if (cur->len == len) {
			if (!ffly_mem_cmp(cur->key, __key, len))
				return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

char const *envget(char const *__key) {
	struct entry *ent = lookup(__key);
	if (ent != NULL)
		return ent->p;
	return NULL;
}

ff_i8_t
envset(char const *__key, char const *__p) {
	struct entry *ent;
	ff_uint_t len = ffly_str_len(__p);
	if (!(ent = lookup(__key)))
		return -1;

	char **e = env+ent->env;
	if (len != ent->vl) {
		ff_int_t dif = (ff_int_t)len-(ff_int_t)ent->vl;
		ent->vl = len;
		ff_uint_t voff = ent->p-*e;
		ent->ol = ((ff_int_t)ent->ol)+dif;
		*e = (char**)__ffly_mem_realloc(*e, ent->ol+2);
		ent->p = (*e)+voff;
	}
	ffly_mem_cpy(ent->p, __p, len+1);
	return 0;
}

void
envput(char const *__key, char const *__p) {
	//ffly_printf("env, key: %s, val: %s\n", __key, __p);
	if (!envset(__key, __p))
		return;

	if (!env) {
		env = (char**)__ffly_mem_alloc(sizeof(char*));	
		off++;
	} else
		env = (char**)__ffly_mem_realloc(env, (++off)*sizeof(char*));

	struct entry *ent;
	ff_uint_t len = ffly_str_len(__key);
	ff_u64_t sum = ffly_hash(__key, len);	
		
	ent = (struct entry*)__ffly_mem_alloc(sizeof(struct entry));
	struct entry **tbl = table+(sum&0xff);
	ent->next = *tbl;
	*tbl = ent;
	ffly_mem_dup((void**)&ent->key, (void*)__key, len);
	
	ent->vl = ffly_str_len(__p);
	char *p;
	p = (*(env+(off-1)) = (char*)__ffly_mem_alloc(ent->vl+len+2));

	ffly_mem_cpy(p, __key, len);
	p+=len;
	*(p++) = '=';
	ent->p = p;
	ffly_mem_cpy(p, __p, ent->vl);
	p+=ent->vl;
	*p = '\0';

	ent->len = len;
	ent->fd = top;
	ent->env = off-1;
	ent->ol = ent->vl+len+1;
	top = ent;

}

