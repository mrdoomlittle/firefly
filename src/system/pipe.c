# include "pipe.h"
# include "shm.h"
# include "../dep/bcopy.h"
# ifdef __fflib
# include "../linux/ipc.h"
# include "../linux/stat.h"
# else
# include <sys/ipc.h>
# include <sys/stat.h>
# endif
# ifndef __ffly_module
# include "io.h"
# else
# include "../mod/io.h"
# endif
# include "../mode.h"
# include "err.h"
# include "mutex.h"
// needs testing
/*
	NOTE: cleanup
	TODO:
		add timeout
*/

typedef struct hdr {
	ff_u16_t size;
} *hdrp;

# define STRT 0x1
# define STOP 0x2
# define DUMP 0x4
# define OK 0x8
struct pipe {
	ff_uint_t shm_id;
	ff_u8_t *bits;
	hdrp h;
	void *buf, *p;
	ff_uint_t width;
	ff_u8_t flags;
	ff_i8_t to_shut;
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
struct pipe static *dead[MAX];
struct pipe static **reuse = dead;

ff_mlock_t static lock = FFLY_MUTEX_INIT;
ff_uint_t ffly_pipe_get_shmid(ff_uint_t __id) {
	return get_pipe(__id)->shm_id;
}

ff_err_t
ffly_pipe_wrl(ff_u64_t __val, ff_u8_t __l, ff_uint_t __pipe) {
	return ffly_pipe_write(&__val, __l, __pipe);
}

ff_u64_t
ffly_pipe_rdl(ff_u8_t __l, ff_uint_t __pipe, ff_err_t *__err) {
	ff_u64_t ret = 0;
	*__err = ffly_pipe_read(&ret, __l, __pipe);
	return ret;
}

void ffly_pipe_shutoff(ff_uint_t __id) {
	struct pipe *pi = get_pipe(__id);
	pi->to_shut = 0;
}

void static
discard(struct pipe *__pi) {
	ffly_mutex_lock(&lock);
	if (__pi == next-1)
		next--;
	else
		*(reuse++) = __pi;
	ffly_mutex_unlock(&lock);
}

ff_uint_t
ffly_pipe(ff_uint_t __width, ff_u8_t __flags,
	ff_uint_t __id, ff_err_t *__err)
{
	ff_uint_t id;
	ffly_mutex_lock(&lock);
	if (reuse>dead)
		id = *(--reuse)-pipes;
	else
		id = (next++)-pipes;
	ffly_mutex_unlock(&lock);
	struct pipe *pi = pipes+id;
	pi->flags = __flags;
	pi->to_shut = -1;
	int flags = 0;
	if (is_flag(__flags, FF_PIPE_CREAT))
		flags |= IPC_CREAT;
	if (is_flag(__flags, FF_PIPE_SHMM))
		pi->shm_id = __id;

	pi->p = ffly_shmget(&pi->shm_id, __width+sizeof(struct hdr)+1, 
		flags|S_IRWXU|S_IRWXO|S_IRWXG, is_flag(__flags, FF_PIPE_SHMM)?FF_SHM_MCI:0);
	if (!pi->p) {
		ffly_fprintf(ffly_out, "pipe failure.\n");
		discard(pi);
		*__err = FFLY_FAILURE;
		return 0;
	}
	ff_u8_t *p = (ff_u8_t*)pi->p;
	pi->bits = p++;
	if (is_flag(__flags, FF_PIPE_CREAT))
		*pi->bits = 0x0;
	pi->h = (hdrp)p;
	p+=sizeof(struct hdr);
	pi->buf = (void*)p;
	pi->width = __width;
	*__err = FFLY_SUCCESS;
	return id;
}

# include "nanosleep.h"
ff_err_t
ffly_pipe_listen(ff_uint_t __id) {
	struct pipe *pi = get_pipe(__id);
	ff_i8_t *to_shut = &pi->to_shut;
	while(!is_bit(pi->bits, OK)) {
		if (!*to_shut)
			reterr;
	}
	clr_bit(pi->bits, OK);
	retok;
}

ff_err_t
ffly_pipe_connect(ff_uint_t __id) {
	struct pipe *pi = get_pipe(__id);
	ff_i8_t *to_shut = &pi->to_shut;
	set_bit(pi->bits, OK); 
	while(is_bit(pi->bits, OK)) {
		if (!*to_shut)
			reterr;
	}
	retok;
}

ff_err_t
ffly_pipe_write(void *__buf, ff_uint_t __size, ff_uint_t __id) {
	struct pipe *pi = get_pipe(__id);
	ff_i8_t *to_shut = &pi->to_shut;
	__ffmod_debug
		ffly_printf("write. %u\n", *pi->bits);

	set_bit(pi->bits, STRT);
	while(is_bit(pi->bits, STRT)) {
		if (!*to_shut)
			reterr;
	}

	ff_u8_t *p = (ff_u8_t*)__buf;
	ff_u8_t *end = p+__size;
	while(p < end) {
		ff_uint_t left = __size-(p-(ff_u8_t*)__buf);
		ff_uint_t size = left>pi->width?pi->width:left;
		ffly_bcopy(pi->buf, p, size);
		p+=size;
		pi->h->size = size;
		__ffmod_debug
			ffly_printf("size: %u\n", pi->h->size);

		set_bit(pi->bits, DUMP);
		__ffmod_debug
			ffly_printf("waiting for peer.\n");

		while(is_bit(pi->bits, DUMP)) {
			if (!*to_shut)
				reterr;
		}
		__ffmod_debug
			ffly_printf("okay.\n");
	}

	set_bit(pi->bits, STOP);
	while(!is_bit(pi->bits, OK)) {
		if (!*to_shut)
			reterr;
	}
	clr_bit(pi->bits, OK);
	retok;
}

ff_err_t
ffly_pipe_read(void *__buf, ff_uint_t __size, ff_uint_t __id) {
	struct pipe *pi = get_pipe(__id);
	ff_i8_t *to_shut = &pi->to_shut;
	while(is_bit(pi->bits, OK)) {
		if (!*to_shut)
			reterr;
	}
	__ffmod_debug
		ffly_printf("read. %u\n", *pi->bits);

	while(!is_bit(pi->bits, STRT));
	__ffmod_debug
		ffly_printf("got start bit.\n");

	clr_bit(pi->bits, STRT);

	ff_u8_t *p = (ff_u8_t*)__buf;
	ff_u8_t *end = p+__size;
	while(!is_bit(pi->bits, STOP)) {
		if (!*to_shut)
			reterr;
		if (is_bit(pi->bits, DUMP) && p < end) {
			ff_uint_t size = pi->h->size;
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
	while(is_bit(pi->bits, OK)) {
		if (!*to_shut)
			reterr;
	}
	retok;
}

void ffly_pipe_close(ff_uint_t __id) {
	struct pipe *pi = get_pipe(__id);
	ffly_shmdt(pi->shm_id);
	if (is_flag(pi->flags, FF_PIPE_CREAT)) 
		ffly_shm_free(pi->shm_id);
	ffly_shm_cleanup(pi->shm_id);
	__ffmod_debug
		ffly_printf("id: %u\n", pi->shm_id);
	discard(pi);
}
