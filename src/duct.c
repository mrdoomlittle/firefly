# include "duct.h"
#ifndef __ffly_bridge
# include "context.h"
# include "memory/plate.h"
# include "dep/mem_dup.h"
# include "dep/mem_cpy.h"
#endif
# include "system/io.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/error.h"
#ifndef __ffly_bridge
#define STACKSZ 512
ff_u8_t static stack[STACKSZ];
#else
ff_uint_t static stack = 0;
#endif
void duct_shm_pipe(ff_dcp, ff_u8_t);
ff_dcp ff_duct(ff_u8_t __comm, ff_u8_t __flags) {
	ff_dcp c;
	c = (ff_dcp)__ffly_mem_alloc(sizeof(struct ff_dc));
	
	// ignore for now this is default
	duct_shm_pipe(c, __flags);

	return c;
}

void ff_duct_destroy(ff_dcp __c) {
	__ffly_mem_free(__c);
}

enum {
	_op_duct_load,
	_op_duct_store,
	_op_duct_end,
	_op_duct_exit,
	_op_duct_write,
	_op_duct_read,
	_op_duct_event,
	_op_duct_alloc,
	_op_duct_free,
	_op_duct_get_frame,
	_op_duct_nt
};

# define tape(__c, __code, __len) \
{   \
    ff_uint_t len; \
    len = __len; \
	_write(__c, &len, sizeof(ff_uint_t)); \
	_write(__c, __code, __len);\
}

# define _write(__c, __buf, __size)\
	duct(__c, write, __buf, __size)
# define _read(__c, __buf, __size)\
	duct(__c, read, __buf, __size)
# ifdef __ffly_bridge
void ff_duct_exit(ff_dcp __c) {
	ff_u8_t code;
	code = _op_duct_exit;
	tape(__c, &code, 1);
}

void ff_duct_done(ff_dcp __c) {
	ff_u8_t code;
	code = _op_duct_end;
	tape(__c, &code, 1);
	stack = 0;
}

void ff_duct_get_frame(ff_dcp __c, void *__dst, ff_uint_t __width, ff_uint_t __height, ff_u8_t __chn_c) {
	ff_u8_t code;
	code = _op_duct_get_frame;
	tape(__c, &code, 1);

	_read(__c, __dst, __width*__height*__chn_c);
}

void ff_duct_read(ff_dcp __c, void *__dst, ff_u16_t __src, ff_u32_t __off, ff_u32_t __size) {
	ff_u8_t code[11];
	*code = _op_duct_read;
	*(ff_u32_t*)(code+1) = __off;
	*(ff_u16_t*)(code+5) = __src;
	*(ff_u32_t*)(code+7) = __size;
	tape(__c, code, 11);
	_read(__c, __dst, __size);
}

void ff_duct_write(ff_dcp __c, void *__src, ff_u16_t __dst, ff_u32_t __off, ff_u32_t __size) {
	ff_u8_t code[11];
	*code = _op_duct_write;
	*(ff_u32_t*)(code+1) = __off;
	*(ff_u16_t*)(code+5) = __dst;
	*(ff_u32_t*)(code+7) = __size;
	tape(__c, code, 11);
	_write(__c, __src, __size);
}

ff_u16_t ff_duct_alloc(ff_dcp __c, ff_u32_t __size) {
	ff_u8_t code[7];
	ff_u16_t r;
	*code = _op_duct_alloc;
	*(ff_u32_t*)(code+1) = __size;
	r = (*(ff_u16_t*)(code+5) = stack);
	stack+=sizeof(ff_uint_t);
	tape(__c, code, 7);
	return r;
}

void ff_duct_free(ff_dcp __c, ff_u16_t __p) {
	ff_u8_t code[3];
	*code = _op_duct_free;
	*(ff_u16_t*)(code+1) = __p;
	tape(__c, code, 3);
}

void ff_duct_event(ff_dcp __c, ff_eventp __event) {
	ff_u16_t data;

	data = ff_duct_alloc(__c, __event->size);
	ff_duct_write(__c, __event->data, data, 0, __event->size);
	ffly_printf("--> plate dats: %u\n", data);
	ff_u8_t code[1+sizeof(struct ff_des)];
	*code = _op_duct_event;
	ff_desp s;
	s = (ff_desp)(code+1);
	s->kind = __event->kind;
	s->field = __event->field;
	s->data = data;
	s->size = __event->size;
	tape(__c, code, 1+sizeof(struct ff_des));
	ff_duct_free(__c, data);
}
# endif
# ifndef __ffly_bridge
void _duct_load();
void _duct_store();
void _duct_end();
void _duct_exit();
void _duct_write();
void _duct_read();
void _duct_event();
void _duct_alloc();
void _duct_free();
void _duct_get_frame();
void _duct_nt();
void static *op[] = {
	_duct_load,
	_duct_store,
	_duct_end,
	_duct_exit,
	_duct_write,
	_duct_read,
	_duct_event,
	_duct_alloc,
	_duct_free,
	_duct_get_frame,
	_duct_nt
};

# include "intsize.h"
ff_uint_t static osz[] = {
	__16*2,					//duct_load
	__16*2,					//duct_store
	0,						//duct_end
	0,						//duct_exit
	(__32*2)+__16,			//duct_write
	(__32*2)+__16,			//duct_read
	sizeof(struct ff_des),	//duct_event
	__32+__16,				//duct_alloc
	__16,					//duct_free
	0,						//duct_get_frame
	0
};

# define stackat(__adr) \
	(stack+(__adr))
ff_u8_t static *t;
ff_dcp static c;
void static
duct_load(void) {
	ff_u16_t dst, n;
	dst = *(ff_u16_t*)t;
	n = *(ff_u16_t*)(t+2);
	_write(c, stackat(dst), n);
}

void static
duct_store(void) {
	ff_u16_t src, n;
	src = *(ff_u16_t*)t;
	n = *(ff_u16_t*)(t+2);
	_read(c, stackat(src), n);
}

void static
duct_event(void) {
	ff_desp s;
	s = (ff_desp)t;
	void *plate;

	plate = ffly_plate_at(*(ff_uint_t*)stackat(s->data));
	void *p;
	ffly_plate_store(plate, &p);
	ffly_mem_dup(&p, p, s->size);
	ffly_fprintf(ffly_log, "plate: %u, %p\n", s->data, p);
	ffly_fprintf(ffly_log, "duct event: %u, %u, %u\n", s->kind, s->field, s->size);
	ff_err_t err;
	ff_eventp event;
	event = ff_event_creat(s->kind, s->field, p, s->size, &err);
	if (_err(ffly_event_push(event))) {
		ff_event_del(event);
		ffly_printf("failed to push event into queue.\n");
	}
}

void static
duct_write(void) {
	ff_u32_t off, size;
	ff_u16_t plate;
	off = *(ff_u32_t*)t;
	plate = *(ff_u16_t*)(t+4);
	size = *(ff_u32_t*)(t+6);

	void *p = ffly_plate_at(*(ff_uint_t*)stackat(plate));
	void *src;
	ffly_plate_store(p, &src);
	
	_read(c, ((ff_u8_t*)src)+off, size);
}

void static
duct_read(void) {
	ff_u32_t off, size;
	ff_u16_t plate;
	off = *(ff_u32_t*)t;
	plate = *(ff_u16_t*)(t+4);
	size = *(ff_u32_t*)(t+6);

	void *p = ffly_plate_at(*(ff_uint_t*)stackat(plate));
	void *dst;
	ffly_plate_store(p, &dst);

	_write(c, ((ff_u8_t*)dst)+off, size);
}

#define jmpagain \
	__asm__("jmp _duct_again")
#define jmpnext \
	__asm__("jmp _duct_next")
#define jmpend \
	__asm__("jmp _duct_end")
#define jmpfi \
	__asm__("jmp _duct_fi")
#define OP(__name) \
	__asm__(__name ":\n\t")
# define MAX 11
ff_i8_t ff_duct_serve(ff_dcp __c) {
	c = __c;
	ff_i8_t rt;
	rt = -1;

	ff_u8_t *text, on;
	ff_uint_t len;
	OP("_duct_again");
	_read(__c, &len, sizeof(ff_uint_t));
	ffly_printf("tape size: %u\n", len);
	text = (ff_u8_t*)__ffly_mem_alloc(len+1);
	// next tape
	*(text+len) = _op_duct_nt;
	_read(__c, text, len);
	t = text;

	OP("_duct_next");
	on = *(t++);
	if (on>=MAX) {
		ffly_printf("error ------------------\n");
		while(1);
	}
	__asm__("jmp *%0" : : "r"(op[on]));

	OP("_duct_load");
		duct_load();
	jmpfi;

	OP("_duct_store");
		duct_store();	
	jmpfi;

	OP("_duct_get_frame"); {
		ffly_frame_buffp fb;
		fb = __ctx(hl_fb);
		fb_gen(fb);
		_write(__c, ffly_frame(fb), fb_w(fb)*fb_h(fb)*fb_cc(fb));
	}
	jmpfi;

	OP("_duct_write");
		duct_write();
	jmpfi;

	OP("_duct_read");
		duct_read();
	jmpfi;

	// remove???
	OP("_duct_window_new");

	OP("_duct_alloc"); {
		ff_u32_t size;
		void *plate;
		ff_u16_t dst;
		size = *(ff_u32_t*)t;
		dst = *(ff_u16_t*)(t+4);
		plate = ffly_plate_alloc();
		ffly_plate_load(plate, __ffly_mem_alloc(size));
		ff_uint_t pn;
		pn = ffly_plateno(plate);
		*(ff_uint_t*)stackat(dst) = pn;
		ffly_fprintf(ffly_log, "plate alloc: %u, %u, %u\n", dst, pn, size);
	}
	jmpfi;

	OP("_duct_free"); {
		ff_uint_t pn;
		pn = *(ff_uint_t*)stackat(*(ff_u16_t*)t);
		void *plate;
		plate = ffly_plate_at(pn);
		void *p;
		ffly_plate_store(plate, &p);
		__ffly_mem_free(p);
		ffly_plate_free(plate);
		ffly_fprintf(ffly_log, "plate free: %u, %u\n", *(ff_u16_t*)t, pn);
	}
	jmpfi;

	OP("_duct_event");
		duct_event();
	jmpfi;

	OP("_duct_fi");
		t+=osz[on];
	jmpnext;

	// next tape
	OP("_duct_nt");
	__ffly_mem_free(text);
	jmpagain;

	OP("_duct_exit");
		rt = 0;
	OP("_duct_end");
	return rt;
}
# endif
