# define __ffly_dict_internal
# include "dict.h"
# include "../ffly_def.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/str_len.h"
# include "../dep/str_dup.h"
# include "../dep/str_cmp.h"

/* not tested */
		
mdl_u64_t static
sum(char const *__key, mdl_u8_t __len) {
	mdl_u64_t ret = 0xffffffffffffffff;
	char const *p = __key;
	char const *end = p+__len;
	while(p != end) {
		ret = ret^*(p++);
		ret = ret>>56|ret<<8;
	}
	return ret;
}

ffly_err_t ffly_dict_init(ffly_dictp __dict) {
	ffly_lat_prepare(&__dict->lat);
	__dict->head = NULL;
}

void* ffly_dict_head(ffly_dictp __dict) {
	return (void*)__dict->head;
}

void ffly_dict_fd(void **__p) {
	*__p = (void*)((entryp)*__p)->fd;
}

void const* ffly_dict_getp(void *__p) {
	return ((entryp)__p)->p;
}

ffly_err_t ffly_dict_put(ffly_dictp __dict, char const *__key, void const *__p) {
	mdl_uint_t l = ffly_str_len(__key);
	mdl_u64_t val = sum(__key, l);

	entryp p;
	p = (entryp)__ffly_mem_alloc(sizeof(struct entry));
	p->next = (entryp)ffly_lat_get(&__dict->lat, val);

	p->key = (char const*)ffly_str_dupe(__key);
	p->p = __p;

	p->fd = __dict->head;
	__dict->head = p;
	ffly_lat_put(&__dict->lat, val, p);
}

void const* ffly_dict_get(ffly_dictp __dict, char const *__key, ffly_err_t *__err) {
	mdl_uint_t l = ffly_str_len(__key);
	mdl_u64_t val = sum(__key, l);

	entryp cur = (entryp)ffly_lat_get(&__dict->lat, val);	
	while(cur != NULL) {
		if (!ffly_str_cmp(cur->key, __key)) return cur->p;
		cur = cur->next;
	}
	return NULL;
}

ffly_err_t ffly_dict_de_init(ffly_dictp __dict) {
	void *cur = ffly_lat_head(&__dict->lat);
	while(cur != NULL) {
		entryp p = (entryp)ffly_lat_getp(cur);
		__ffly_mem_free((void*)p->key);
		__ffly_mem_free(p);
		ffly_lat_fd(&cur);
	}

	ffly_lat_free(&__dict->lat);
}
