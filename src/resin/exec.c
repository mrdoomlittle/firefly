# include "../resin.h"
# include "../stdio.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../remf.h"
# include "mm.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
static void(*get)(ff_uint_t, ff_uint_t, void*);
ff_u32_t static end;
ff_u32_t static ip;

#define PAGE_SHIFT 4
#define PAGE_SIZE (1<<PAGE_SHIFT)
struct page {
	void *p;
	ff_uint_t n;
};

struct page **pages = NULL;

void static
ldpage(struct page *__page) {
	ff_uint_t off;
	off = __page->n<<PAGE_SHIFT;
	ff_uint_t d;
	if ((d = end-off)<PAGE_SIZE) {
		get(off, d, __page->p);
		return;
	}

	get(off, PAGE_SIZE, __page->p);
}

struct page static* getpage(ff_uint_t __n) {
	struct page **pp, *p;
	pp = pages+__n;

	if (!(p = *pp)) {
		p = *pp = (struct page*)__ffly_mem_alloc(sizeof(struct page));
		p->p = __ffly_mem_alloc(PAGE_SIZE);	
		p->n = __n;
	}

	ldpage(p);
	return p;
}

void static
__get(ff_uint_t __offset, ff_uint_t __size, void *__dst) {

}

/*
	TODO:
		read by page and not single bytes

		keep pages until limit is reached and oldest page or least touched
		should be removed to make space for new one
*/

ff_u8_t static
fetch_byte(ff_off_t __off) {
	if (ip+__off >= end) return 0x0;

	ff_u8_t byte;
	get(ip+__off, 1, &byte);
	return byte;
}

void static
ip_incr(ff_uint_t __by) {
	ip+=__by;
}

ff_addr_t static
get_ip(void) {
	return ip;
}

void static
set_ip(ff_addr_t __to) {
	ip = __to;
}

struct arg_s {
	void *p;
	ff_u64_t src;
	ff_u32_t size;
	ff_u8_t what;
	ff_u64_t init_arg;
} __attribute__((packed));

# include "../dep/mem_cpy.h"
# include "../init.h"
static struct ffly_resin ctx;

// needs cleaning
void* ring(ff_u8_t __no, void *__arg_p) {
	struct arg_s *arg;
	if (__arg_p != NULL) {
		ffly_printf("this error is nothing to panic about, resin/exec.c.\n");
		arg = (struct arg_s*)ff_resin_resolv_adr(&ctx, *(ff_addr_t*)__arg_p);
	} else
		arg = NULL;
	switch(__no) {
		case 0x0: {// set stack pointer
			ff_u64_t sp = ctx.stack_size;
			ff_resin_sst(&ctx, &sp, 0, sizeof(ff_u64_t));
			ctx.sp = ctx.stack_size;
			break;
		}
		case 0x1: {
			// mem_read
			ffly_mem_cpy(ff_resin_resolv_adr(&ctx, arg->src), arg->p, arg->size);
			break;
		}
		case 0x2: {
			// mem_write
			ffly_mem_cpy(arg->p, ff_resin_resolv_adr(&ctx, arg->src), arg->size);
			break;
		}
		case 0x3: {
			// mem_mmap
			arg->p = ff_resin_mmap(arg->size);	
			break;
		}
		case 0x4: {
			ff_resin_munmap(*(void**)__arg_p);
			// mem_munmap
			break;
		}
		case 0x5: {
			//init	
			if (arg != NULL) {
				__init_arg__ = (struct init_arg*)ff_resin_resolv_adr(&ctx, arg->init_arg);
				printf("init what? %u\n", arg->what);

				ffly_init(arg->what);
			}
			break;
		}
	}
	printf("ring ring hello?, %u, %p\n", __no, __arg_p);
}

static struct ffly_resin ctx = {
	.stack_size = 700,
	.fetch_byte = fetch_byte,
	.ip_incr = ip_incr,
	.get_ip = get_ip,
	.set_ip = set_ip,
	.rin = ring
};

void static init(void) {
	ff_uint_t pac;

	pac = (end+(PAGE_SIZE-1))>>PAGE_SHIFT;
	pages = (struct page**)__ffly_mem_alloc(pac*sizeof(struct page*));
	
	ff_uint_t i;
	i = 0;
	for(;i != pac;i++)
		*(pages+i) = NULL;
}

void static de_init(void) {

}

void ffres_exec(void(*__get)(ff_uint_t, ff_uint_t, void*), ff_u32_t __end, void(*__prep)(void*, void*), void *__hdr, ff_u32_t __entry) {
	get = __get;
	end = __end;
	ip = __entry;
	init();
	ff_resin_init(&ctx);
	if (__prep != NULL)
		__prep(__hdr, (void*)&ctx);
	ff_err_t exit_code;
	ff_resin_exec(&ctx, &exit_code);
	printf("exit code: %d\n", exit_code);
	ff_resin_de_init(&ctx);
	de_init();
}
