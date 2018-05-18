# include "../resin.h"
# include "../stdio.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../ffef.h"
# include "mm.h"
ff_u8_t static *bin = NULL, *end = NULL;
ff_u32_t static ip;
ff_u8_t static
fetch_byte(ff_off_t __off) {
	if (bin+ip+__off >= end) return 0x0;
	return *(bin+ip+__off);
}

void static
ip_incr(ff_uint_t __by) {
	ip+=__by;
}

ff_addr_t static
get_ip() {
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
} __attribute__((packed));

# include "../dep/mem_cpy.h"
struct ffly_resin ctx;
void* ring(ff_u8_t __no, void *__arg_p) {
	ff_u8_t *arg = (ff_u8_t*)__arg_p;
	switch(__no) {
		case 0x0: {// set stack pointer
			ff_u64_t sp = ctx.stack_size;
			ff_resin_sst(&ctx, &sp, 0, sizeof(ff_u64_t));
			break;
		}
		case 0x1: {
			// mem_read
			struct arg_s *p = (struct arg_s*)ff_resin_resolv_adr(&ctx, *(ff_addr_t*)arg);
			ffly_mem_cpy(ff_resin_resolv_adr(&ctx, p->src), p->p, p->size);
			break;
		}
		case 0x2: {
			// mem_write
			struct arg_s *p = (struct arg_s*)ff_resin_resolv_adr(&ctx, *(ff_addr_t*)arg);
			ffly_mem_cpy(p->p, ff_resin_resolv_adr(&ctx, p->src), p->size);
			break;
		}
		case 0x3: {
			// mem_mmap
			struct arg_s *p = (struct arg_s*)ff_resin_resolv_adr(&ctx, *(ff_addr_t*)arg);
			ffly_printf("size: %u, %u\n", p->size, *(ff_addr_t*)arg);
			p->p = ff_resin_mmap(p->size);	
			break;
		}
		case 0x4: {
			ff_resin_munmap(*(void**)arg);
			// mem_munmap
			break;
		}
	}
	printf("ring ring hello?\n");
}

struct ffly_resin ctx = {
	.stack_size = 700,
	.fetch_byte = fetch_byte,
	.ip_incr = ip_incr,
	.get_ip = get_ip,
	.set_ip = set_ip,
	.rin = ring
};

void ffres_exec(void *__bin, void *__end, void(*__prep)(void*, void*), void *__hdr, ff_u32_t __entry) {
	bin = (ff_u8_t*)__bin;
	end = (ff_u8_t*)__end;
	ip = __entry;
	ff_resin_init(&ctx);
	if (__prep != NULL)
		__prep(__hdr, (void*)&ctx);
	ff_err_t exit_code;
	ff_resin_exec(&ctx, &exit_code);
	printf("exit code: %d\n", exit_code);
	ff_resin_de_init(&ctx);
}
