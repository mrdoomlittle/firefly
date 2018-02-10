# include <mdlint.h>
# include "ffly_def.h"
# include <unistd.h>
//void*(*ffly_allocp)(mdl_uint_t) = NULL;
//void(*ffly_freep)(void*) = NULL;
//void*(*ffly_reallocp)(void*, mdl_uint_t) = NULL;
//# include <malloc.h>
/*
void* malloc(int unsigned __bc) {
	
	return malloc(__bc);
}*/


void pr() {
	char const text[] = "hi\n";
	write(0, text, sizeof(text));
}

void free(void *__p) {
	char const text[] = "Hello\n";
	write(0, text, sizeof(text));
}
/*
void* realloc(void *__p, int unsigned __bc) {

}*/
