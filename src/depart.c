# include "memory/mem_free.h"

/*
	rename
*/
void static *list[200] = {NULL};
void static **next = list;

void ffly_depart(void *__p) {
	if (!__p) {
		void **cur = list;
		while(*cur != NULL)
			__ffly_mem_free(*(cur++));
		next = list;
		return;
	}

	*(next++) = __p;
	*next = NULL;
}
