# include "duct.h"
# include "system/pipe.h"
# ifndef __ffly_bridge
# include "graphics/frame_buff.h"
# endif
/*
	access all parts of engine from one point,
	diffrent from hatch as that for debugging
*/

ff_uint_t static pipe;
ff_err_t ff_duct_open(ff_u8_t __flags) {
	ff_err_t err;
	pipe = ffly_pipe(8, __flags, 0, &err);
	return err;
}

void ff_duct_close() {
	ffly_pipe_close(pipe);
}

void ff_duct_connect() {
	ffly_pipe_connect(pipe);
}

void ff_duct_listen() {
	ffly_pipe_listen(pipe);
}

enum {
	_ff_duct_exit,
	_ff_duct_get_frame,
	_ff_duct_done
};

void ff_duct_done() {
	ffly_pipe_wr8l(_ff_duct_done, pipe);
}

void ff_duct_exit() {
	// send exit signal
	ffly_pipe_wr8l(_ff_duct_exit, pipe);
}

void ff_duct_get_frame(void *__dst, ff_uint_t __width, ff_uint_t __height, ff_u8_t __chn_c) {
	ffly_pipe_wr8l(_ff_duct_get_frame, pipe);
	ffly_pipe_read(__dst, __width*__height*__chn_c, pipe);
}

# ifndef __ffly_bridge
void _duct_exit();
void _duct_get_frame();
void _duct_end();

void static *op[] = {
	_duct_exit,
	_duct_get_frame,
	_duct_end
};

ff_i8_t ff_duct_serve() {
	ff_u8_t no;
	ff_i8_t ret = -1;

	__asm__("_duct_again:\n\t");
	ffly_pipe_read(&no, 1, pipe);
	__asm__("jmp *%0" : : "r"(op[no]));

	__asm__("_duct_exit:\n\t"); {
		ret = 0;
	}
	__asm__("jmp _duct_end");

	__asm__("_duct_get_frame:\n\t"); {
		ffly_pipe_write(ffly_frame(__frame_buff__), __frame_buff__->width*__frame_buff__->height*__frame_buff__->chn_c, pipe);
	}
	__asm__("jmp _duct_again");
	__asm__("_duct_end:\n\t");
	return ret;
}
# endif
