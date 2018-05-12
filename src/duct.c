# include "duct.h"
# include "system/pipe.h"
# ifndef __ffly_bridge
# include "graphics/frame_buff.h"
# endif
# include "memory/plate.h"
/*
	access all parts of engine from one point,
	diffrent from hatch as that for debugging lower level parts
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
	_ff_duct_done,
	_ff_duct_read,
	_ff_duct_write
};

void ff_duct_done() {
	ffly_pipe_wr8l(_ff_duct_done, pipe);
}

void ff_duct_exit() {	// send exit signal
	ffly_pipe_wr8l(_ff_duct_exit, pipe);
}

void ff_duct_get_frame(void *__dst, ff_uint_t __width, ff_uint_t __height, ff_u8_t __chn_c) {
	ffly_pipe_wr8l(_ff_duct_get_frame, pipe);
	ffly_pipe_read(__dst, __width*__height*__chn_c, pipe);
}

void ff_duct_read(void *__dst, ff_uint_t __src, ff_uint_t __off, ff_uint_t __size) {
	ffly_pipe_wr8l(_ff_duct_read, pipe);
	ffly_pipe_write(&__off, sizeof(ff_uint_t), pipe);
	ffly_pipe_write(&__src, sizeof(ff_uint_t), pipe);
	ffly_pipe_write(&__size, sizeof(ff_uint_t), pipe);
	ffly_pipe_read(__dst, __size, pipe);
}

void ff_duct_write(void *__src, ff_uint_t __dst, ff_uint_t __off, ff_uint_t __size) {
	ffly_pipe_wr8l(_ff_duct_write, pipe);
	ffly_pipe_write(&__off, sizeof(ff_uint_t), pipe);
	ffly_pipe_write(&__src, sizeof(ff_uint_t), pipe);
	ffly_pipe_write(&__size, sizeof(ff_uint_t), pipe);
	ffly_pipe_write(__dst, __size, pipe);
}

# ifndef __ffly_bridge
void static
duct_read() {
	ff_uint_t off;
	ffly_pipe_read(&off, sizeof(ff_uint_t), pipe);

	ff_uint_t plate;
	ffly_pipe_read(&plate, sizeof(ff_uint_t), pipe);

	ff_uint_t size;
	ffly_pipe_read(&size, sizeof(ff_uint_t), pipe);

	ffly_pipe_write(((ff_u8_t*)ffly_plate_at(plate))+off, size, pipe);
}

void static
duct_write() {
	ff_uint_t off;
	ffly_pipe_read(&off, sizeof(ff_uint_t), pipe);

	ff_uint_t plate;
	ffly_pipe_read(&plate, sizeof(ff_uint_t), pipe);

	ff_uint_t size;
	ffly_pipe_read(&size, sizeof(ff_uint_t), pipe);

	ffly_pipe_read(((ff_u8_t*)ffly_plate_at(plate))+off, size, pipe);
}

void _duct_exit();
void _duct_get_frame();
void _duct_end();
void _duct_read();
void _duct_write();

void static *op[] = {
	_duct_exit,
	_duct_get_frame,
	_duct_end,
	_duct_read,
	_duct_write
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


	__asm__("_duct_read:\n\t"); {
		duct_read();
	}
	__asm__("jmp _duct_again");

	__asm__("_duct_write:\n\t"); {
		duct_write();
	}
	__asm__("jmp _duct_again");

	__asm__("_duct_get_frame:\n\t"); {
		ffly_pipe_write(ffly_frame(__frame_buff__), __frame_buff__->width*__frame_buff__->height*__frame_buff__->chn_c, pipe);
	}
	__asm__("jmp _duct_again");
	__asm__("_duct_end:\n\t");
	return ret;
}
# endif
