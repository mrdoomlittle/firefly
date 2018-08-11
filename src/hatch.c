# include "system/err.h"
# include "system/pipe.h"
# include "linux/types.h"
# include "linux/unistd.h"
# include "dep/str_cpy.h"
# include "system/io.h"
# include "hatch.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/vec.h"
# include "system/map.h"
# include "system/flags.h"
# include "lib.h"
# include "system/thread.h"
static char const tmpl[] = "/tmp/hatch.XXXXXX";
ff_u8_t static
rcvop(ff_uint_t __pipe, ff_err_t *__err){
	ff_u8_t op = 0;
	ffly_printf("recving op.\n");
	op = ffly_pipe_rd8l(__pipe, __err);
	ffly_printf("recved.\n");
	return op;
} 

# define flg_sep if (p != buf) *p++ = ',';
char static*
vec_flags(ffly_vecp __vec) {
	char static buf[200];
	char *p = buf;
	if (ffly_is_flag(__vec->flags, VEC_AUTO_RESIZE))
		p+=ffly_str_cpy(p, "auto-resize");
	if (ffly_is_flag(__vec->flags, VEC_BLK_CHAIN)) {
		flg_sep
		p+=ffly_str_cpy(p, "block-chain");
	}
	if (ffly_is_flag(__vec->flags, VEC_UUU_BLKS)) {
		flg_sep
		p+=ffly_str_cpy(p, "uuu-blocks");
	}
	if (ffly_is_flag(__vec->flags, VEC_NONCONTINUOUS)) {
		flg_sep
		p+=ffly_str_cpy(p, "noncontinuous");
	}
	*p = '\0';
	return buf;
}

ff_err_t static
lsvec(ff_uint_t __pipe) {
	ffly_vecp p;
	if (!(p = ffly_vec_list())) {
		ffly_printf("nothing to be sent.\n");
		ffly_pipe_wr8l(-1, __pipe);
		retok;
	}

	if (_err(ffly_pipe_wr8l(0, __pipe)))
		reterr;
	ff_uint_t l = 1024;
	char *buf = (char*)__ffly_mem_alloc(l);
	if (_err(ffly_pipe_write(&l, sizeof(ff_uint_t), __pipe)))
		reterr;
	ff_uint_t i = 0;
	while(p != NULL) {
		ffly_sprintf(buf, "vec %u; size: %u, page_c: %u, blk_size: %u - off: %u, flags: %s\n",
			i, p->size, p->page_c, p->blk_size, p->off, vec_flags(p));
		ffly_printf("sent: %s\n", buf);
		if (_err(ffly_pipe_write(buf, l, __pipe)))
			reterr;
		p = vec_next(p);
		if (_err(ffly_pipe_wr8l(!p?-1:0, __pipe)))
			reterr;
	} 
	__ffly_mem_free(buf);
	retok;
}

void static
lsmap(ff_uint_t __pipe) {

}

ff_uint_t static pipe;
ff_tid_t static thread;

ff_i8_t static live = -1;
void static*
hatch(void *__arg_p) {
	char file[sizeof(tmpl)];
	ffly_str_cpy(file, tmpl);
	int fd;
	if ((fd = ff_mktemp(file)) == -1) {
		return NULL;
	}

	ff_err_t err;
	ffly_printf("key: %s\n", file); 
	pipe = ffly_pipe(8, FF_PIPE_CREAT, 0, &err);

	ff_uint_t shm_id = ffly_pipe_get_shmid(pipe);
	write(fd, &shm_id, sizeof(ff_uint_t));
	ff_u8_t op;
_bk:
	live = 0;
	if (_err(ffly_pipe_listen(pipe))) {
		ffly_printf("failed to listen.\n");
		goto _end;
	}
_again:
	op = rcvop(pipe, &err);
	if (_err(err))
		goto _end;
	if (op == _ffly_ho_login) {
		/*
			recv hashed password then send a session key
		*/
	} else if (op == _ffly_ho_logout) {
			
	} else if (op == _ffly_ho_meminfo) {
		struct ffly_meminfo info;
		ffly_meminfo(&info);
		if (_err(ffly_pipe_write(&info, sizeof(struct ffly_meminfo), pipe)))
			goto _end;
	} else if (op == _ffly_ho_disconnect)
		goto _bk;  
	else if (op == _ffly_ho_shutdown) {
		ffly_printf("recved shutdown command.\n");
		goto _end;
	} else {
		switch(op) {
			case _ffly_ho_lsvec: lsvec(pipe); break;
			case _ffly_ho_lsmap: lsmap(pipe); break;
			default:
				ffly_printf("unknown.\n");
			goto _end;
		}
	}
	goto _again;
_end:
	ffly_pipe_close(pipe);
	unlink(file);
	return NULL;
}

ff_err_t
ffly_hatch_start() {
	ffly_thread_create(&thread, hatch, NULL);
	while(live<0);
}

void ffly_hatch_shutoff() {
	ffly_pipe_shutoff(pipe);
}

void ffly_hatch_wait() {
	ffly_thread_wait(thread);
}

/*
# include "system/vec.h"
ff_err_t ffmain(int __argc, char **__argv) {
//	ff_err_t err;
//	ffly_vecp p = ffly_vec(1, VEC_AUTO_RESIZE|VEC_NONCONTINUOUS, &err);
//	ffly_hatch_run();
//	ffly_vec_destroy(p);
	ffly_hatch_start();
	ffly_printf("shutoff pipe.\n");
	ffly_hatch_shutoff();

	ffly_hatch_wait();
	retok;
}*/
