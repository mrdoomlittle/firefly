# include <mdlint.h>
# include "ffly_def.h"
void*(*ffly_allocp)(mdl_uint_t) = NULL;
void(*ffly_freep)(void*) = NULL;
void*(*ffly_reallocp)(void*, mdl_uint_t) = NULL;
//# include <malloc.h>
//void* malloc(int unsigned __bc) {
//	while(1);
//}


void free(void *__p) {

	printf("HI.\n");
}
/*
void* realloc(void *__p, int unsigned __bc) {

}*/
