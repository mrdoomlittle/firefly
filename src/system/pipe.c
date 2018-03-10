# include "pipe.h"
# include "shm.h"
# include "../data/bcopy.h"
# include "../linux/ipc.h"
# include "../linux/stat.h"
# ifndef __ffly_module
# include "io.h"
# else
# include "../mod/io.h"
# endif
# include "../mode.h"
// needs testing
/*
	NOTE: cleanup

*/

struct hdr {
	mdl_uint_t size;
};

# define STRT 0x1
# define STOP 0x2
# define DUMP 0x4
# define OK 0x8
struct pipe {
	mdl_uint_t shm_id;
	mdl_u8_t *bits;
	struct hdr *h;
	void *buf, *p;
	mdl_uint_t width;
	mdl_u8_t flags;
};

# define is_flag(__flags, __flag) \
	((__flags&__flag) == __flag)
# define clr_bit(__bits, __bit) \
	(*__bits) ^= *__bits&__bit;
# define set_bit(__bits, __bit)\
	(*__bits) |= __bit;
# define is_bit(__bits, __bit) \
	((*__bits&__bit) == __bit)
# define get_pipe(__id) (pipes+__id)
# define MAX 5
struct pipe static pipes[MAX];
struct pipe static *next = pipes;
mdl_uint_t ffly_pipe_get_shmid(mdl_uint_t __id) {
	return get_pipe(__id)->shm_id;
}

void ffly_pipe_wrl(mdl_u64_t __val, mdl_u8_t __l, mdl_uint_t __pipe) {
	ffly_pipe_write(&__val, __l, __pipe);
}

mdl_u64_t ffly_pipe_rdl(mdl_u8_t __l, mdl_uint_t __pipe) {
	mdl_u64_t ret = 0;
	ffly_pipe_read(&ret, __l, __pipe);
	return ret;
}

mdl_uint_t ffly_pipe(mdl_uint_t __width, mdl_u8_t __flags, mdl_uint_t __id) {
	mdl_uint_t id = next-pipes;
	struct pipe *pi = next++;
	pi->flags = __flags;
	int flags = 0;
	if (is_flag(__flags, FF_PIPE_CREAT))
		flags |= IPC_CREAT;
	if (is_flag(__flags, FF_PIPE_SHMM))
		pi->shm_id = __id;

	pi->p = ffly_shmget(&pi->shm_id, __width+sizeof(struct hdr)+1, flags|S_IRWXU|S_IRWXO|S_IRWXG, is_flag(__flags, FF_PIPE_SHMM)?FF_SHM_MCI:0);
	if (!pi->p) {
		ffly_fprintf(ffly_out, "pipe failure.\n");
	}
	mdl_u8_t *p = (mdl_u8_t*)pi->p;
	pi->bits = p++;
	*pi->bits = 0x0;
	pi->h = (struct hdr*)p;
	p+= sizeof(struct hdr);
	pi->buf = (void*)p;
	pi->width = __width;
	return id;
}

void ffly_pipe_listen(mdl_uint_t __id) {
	struct pipe *pi = get_pipe(__id);
	while(!is_bit(pi->bits, OK));	
	clr_bit(pi->bits, OK);
}

void ffly_pipe_connect(mdl_uint_t __id) {
	struct pipe *pi = get_pipe(__id);
	set_bit(pi->bits, OK); 
	while(is_bit(pi->bits, OK));
}

void ffly_pipe_write(void *__buf, mdl_uint_t __size, mdl_uint_t __id) {
	struct pipe *pi = get_pipe(__id);
	__ffmod_debug
		ffly_printf("write. %u\n", *pi->bits);
	set_bit(pi->bits, STRT);

	mdl_u8_t *p = (mdl_u8_t*)__buf;
	mdl_u8_t *end = p+__size;
	while(p < end) {
		mdl_uint_t left = __size-(p-(mdl_u8_t*)__buf);
		mdl_uint_t size = left>pi->width?pi->width:left;
		ffly_bcopy(pi->buf, p, size);
		p+=size;
		pi->h->size = size;
		__ffmod_debug
			ffly_printf("size: %u\n", pi->h->size);
		set_bit(pi->bits, DUMP);
		__ffmod_debug
			ffly_printf("waiting for peer.\n");
		while(is_bit(pi->bits, DUMP));
		__ffmod_debug
			ffly_printf("okay.\n");
	}

	set_bit(pi->bits, STOP);
	while(!is_bit(pi->bits, OK));  
	clr_bit(pi->bits, OK);
}

void ffly_pipe_read(void *__buf, mdl_uint_t __size, mdl_uint_t __id) {
	struct pipe *pi = get_pipe(__id);
	while(is_bit(pi->bits, OK));
	__ffmod_debug
		ffly_printf("read. %u\n", *pi->bits);
	while(!is_bit(pi->bits, STRT));
	__ffmod_debug
		ffly_printf("got start bit.\n");
	clr_bit(pi->bits, STRT);

	mdl_u8_t *p = (mdl_u8_t*)__buf;
	mdl_u8_t *end = p+__size;
	while(!is_bit(pi->bits, STOP)) {
		if (is_bit(pi->bits, DUMP) && p < end) {
			mdl_uint_t size = pi->h->size;
			__ffmod_debug
				ffly_printf("size: %u\n", size);
			ffly_bcopy(p, pi->buf, size);  
			p+=size;
			clr_bit(pi->bits, DUMP); 
		}
	}

	__ffmod_debug
		ffly_printf("got end bit.\n");
	clr_bit(pi->bits, STOP);
	set_bit(pi->bits, OK);
}

void ffly_pipe_close(mdl_uint_t __id) {
	struct pipe *pi = get_pipe(__id);
	ffly_shmdt(pi->shm_id);
	if (is_flag(pi->flags, FF_PIPE_CREAT)) 
		ffly_shm_free(pi->shm_id);
	ffly_shm_cleanup(pi->shm_id);
	__ffmod_debug
		ffly_printf("id: %u\n", pi->shm_id);
}
