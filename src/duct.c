# include "duct.h"
# include "system/pipe.h"
# ifndef __ffly_bridge
# include "graphics/frame_buff.h"
# include "memory/plate.h"
# include "dep/mem_dup.h"
# include "dep/mem_cpy.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# endif
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
	_ff_duct_write,
	_ff_duct_alloc,
	_ff_duct_free,
	_ff_duct_event
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
	ffly_pipe_write(&__dst, sizeof(ff_uint_t), pipe);
	ffly_pipe_write(&__size, sizeof(ff_uint_t), pipe);
	ffly_pipe_write(__src, __size, pipe);
}

ff_uint_t ff_duct_alloc(ff_uint_t __size) {
	ffly_pipe_wr8l(_ff_duct_alloc, pipe);
	ffly_pipe_write(&__size, sizeof(ff_uint_t), pipe);
	ff_uint_t ret_val;
	ffly_pipe_read(&ret_val, sizeof(ff_uint_t), pipe);
	return ret_val;
}

void ff_duct_free(ff_uint_t __p) {
	ffly_pipe_wr8l(_ff_duct_free, pipe);
	ffly_pipe_write(&__p, sizeof(ff_uint_t), pipe);
}

void ff_duct_event(ff_eventp __event) {
	ff_uint_t data = ff_duct_alloc(__event->size);
	ff_duct_write(__event->data, data, 0, __event->size);

	ffly_pipe_wr8l(_ff_duct_event, pipe);
	ffly_pipe_write(&__event->kind, sizeof(ff_u8_t), pipe);
	ffly_pipe_write(&__event->field, sizeof(ff_u8_t), pipe);
	ffly_pipe_write(&data, sizeof(ff_uint_t), pipe);
	ffly_pipe_write(&__event->size, sizeof(ff_uint_t), pipe);
	ff_duct_free(data);
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

	void *p = ffly_plate_at(plate);
	void *src;
	ffly_plate_store(p, &src);

	ffly_pipe_write(((ff_u8_t*)src)+off, size, pipe);
}

void static
duct_write() {
	ff_uint_t off;
	ffly_pipe_read(&off, sizeof(ff_uint_t), pipe);

	ff_uint_t plate;
	ffly_pipe_read(&plate, sizeof(ff_uint_t), pipe);

	ff_uint_t size;
	ffly_pipe_read(&size, sizeof(ff_uint_t), pipe);

	void *p = ffly_plate_at(plate);
	void *dst;
	ffly_plate_store(p, &dst);

	ffly_pipe_read(((ff_u8_t*)dst)+off, size, pipe);
}

void static
duct_event() {
	ff_u8_t kind;
	ff_u8_t field;
	ff_uint_t size;
	ff_uint_t data;
	ffly_pipe_read(&kind, sizeof(ff_u8_t), pipe);
	ffly_pipe_read(&field, sizeof(ff_u8_t), pipe);
	ffly_pipe_read(&data, sizeof(ff_uint_t), pipe);
	ffly_pipe_read(&size, sizeof(ff_uint_t), pipe);

	void *plate = ffly_plate_at(data);
	void *p;
	ffly_plate_store(plate, &p);

	ffly_mem_dup(&p, p, size);	

	ff_err_t err;
	ff_eventp event;
	event = ff_event_creat(kind, field, p, size, &err);
	if (_err(ffly_event_push(event))) {
		ff_event_del(event);
		ffly_printf("failed to push event into queue.\n");
	}
}

void _duct_exit();
void _duct_get_frame();
void _duct_end();
void _duct_read();
void _duct_write();
void _duct_alloc();
void _duct_free();
void _duct_event();

void static *op[] = {
	_duct_exit,
	_duct_get_frame,
	_duct_end,
	_duct_read,
	_duct_write,
	_duct_alloc,
	_duct_free,
	_duct_event
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

	__asm__("_duct_alloc:\n\t"); {
		ff_uint_t size;
		void *plate;
		ffly_pipe_read(&size, sizeof(ff_uint_t), pipe);
		ffly_plate_load(plate = ffly_plate_alloc(), __ffly_mem_alloc(size));
		ff_uint_t no = ffly_plateno(plate);
		ffly_pipe_write(&no, sizeof(ff_uint_t), pipe);
	}
	__asm__("jmp _duct_again");

	__asm__("_duct_free:\n\t"); {
		ff_uint_t no;
		ffly_pipe_read(&no, sizeof(ff_uint_t), pipe);
		void *plate;
		plate = ffly_plate_at(no);
		void *p;
		ffly_plate_store(plate, &p);

		__ffly_mem_free(p);
		ffly_plate_free(plate);
	}
	__asm__("jmp _duct_again");

	__asm__("_duct_event:\n\t"); {
		duct_event();
	}
	__asm__("jmp _duct_again");

	__asm__("_duct_get_frame:\n\t"); {
		ffly_fb_gen(__frame_buff__);
		ffly_pipe_write(ffly_frame(__frame_buff__), __frame_buff__->width*__frame_buff__->height*__frame_buff__->chn_c, pipe);
	}
	__asm__("jmp _duct_again");
	__asm__("_duct_end:\n\t");
	return ret;
}
# endif
