# include "tls.h"
# include "../linux/mman.h"
# include "../linux/prctl.h"
# include "../ffly_def.h"
# define TLS_SIZE 2048
# include "error.h"
ff_uint_t tls_off = 8;
ff_err_t
ffly_tls_new(void) {
	void *p;
	p = mmap(NULL, TLS_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	if (p == (void*)-1) {
		reterr;		
	}
	if (arch_prctl(ARCH_SET_FS, (ff_u64_t)p) == -1) {

	}
	*(void**)p = p;
	retok;
}

void ffly_tls_destroy(void) {
	void *p;
	if (arch_prctl(ARCH_GET_FS, (ff_u64_t)&p) == -1) {
	}
	munmap(p, TLS_SIZE);
}

ff_uint_t ffly_tls_alloc(ff_uint_t __size) {
	ff_uint_t ret = tls_off;
	// align to 8bytes
	tls_off+=((__size>>3)+((__size&((~(ff_u64_t)0)>>(64-3)))>0));
	return ret;	
}

/*
	list of functions to call to init tls 
*/
static void(*init[26])(void);
static void(**end)(void) = init;


void ffly_tls_toinit(void(*__func)(void)) {
	*(end++) = __func;
}

void ffly_tls_init(void) {
	void(**cur)(void) = init;
	while(cur != end)
		(*(cur++))();
}

void ffly_tls_set(ff_u64_t __val, ff_uint_t __off) {
	ff_u32_t off = __off*8;
	__asm__("movl %0, %%eax\n\t"
			"movq %1, %%rdx\n\t"
			"movq %%rdx, %%fs:(%%eax)" : : "m"(off), "m"(__val) : "rax", "rdx");
}

ff_u64_t ffly_tls_get(ff_uint_t __off) {
	ff_u32_t off = __off*8;
	ff_u64_t ret;
	__asm__("movl %1, %%eax\n\t"
			"movq %%fs:(%%eax), %%rdx\n\t"
			"movq %%rdx, %0" : "=m"(ret) : "m"(off) : "rax", "rdx");
	return ret;
}

/*
	get pointer of data at x
*/
void *ffly_tls_getp(ff_uint_t __off) {
	ff_u8_t *ret;
	__asm__("movq %%fs:0, %%rax\n\t"
			"movq %%rax, %0" : "=m"(ret) : : "rax");
	return ret+(__off*8);
}
