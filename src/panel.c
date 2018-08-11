# include "panel.h"
# include "system/pipe.h"
# include "system/io.h"
ff_err_t static
sndop(ff_u8_t __op, ff_uint_t __pipe) {
	ff_err_t ret;
	ret = ffly_pipe_wr8l(__op, __pipe);
	return ret;
}

ff_uint_t static pipe;
void ff_p_connect(ff_uint_t __shm_id) {
	ff_err_t err;
	pipe = ffly_pipe(8, FF_PIPE_SHMM, __shm_id, &err);
	ffly_pipe_connect(pipe);
}

void ff_p_disconnect(void) {
	sndop(_ffly_po_disconnect, pipe);
	ffly_pipe_close(pipe);
}

void ff_p_meminfo(struct ffly_meminfo *__info) {
	sndop(_ffly_po_meminfo, pipe);
	ffly_pipe_read(__info, sizeof(struct ffly_meminfo), pipe);
}

ff_u8_t static
rcvop(ff_uint_t __pipe, ff_err_t *__err){
	ff_u8_t op;
	op = ffly_pipe_rd8l(__pipe, __err);
	return op;
}

void _disconnect();
void _halt();
void _resume();
void _meminfo();
void *op_tbl[] = {
	_disconnect,
	_halt,
	_resume,
	_meminfo
};

# define jmpagain \
	__asm__("jmp _again")
# define jmpend \
	__asm__("jmp _end")
void ffly_panel_start(void) {
	ff_err_t err;
	pipe = ffly_pipe(8, FF_PIPE_CREAT, 0, &err);
	ffly_printf("shmid: %u\n", ffly_pipe_get_shmid(pipe));
	ffly_fdrain(ffly_out);
	ffly_pipe_listen(pipe);

	ff_u8_t op;
	__asm__("_again:\n\t");
	op = rcvop(pipe, &err);
	__asm__("jmp *%0": : "r"(op_tbl[op]));
	
	__asm__("_halt:\n\t");
		
	jmpagain;
	__asm__("_resume:\n\t");

	jmpagain;
	__asm__("_meminfo:\n\t"); {
		struct ffly_meminfo info;
		ffly_meminfo(&info);
		ffly_pipe_write(&info, sizeof(struct ffly_meminfo), pipe);
		ffly_printf("operator requested memory infomation.\n");
	}
	jmpagain;
	__asm__("_disconnect:\n\t"
			"_end:\n\t");
	ffly_pipe_close(pipe);
	ffly_printf("goodbye.\n");
}

//ff_err_t ffmain(int __argc, char **__argv) {
//	ffly_panel_start();
//}
