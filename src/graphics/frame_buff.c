# include "frame_buff.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
ffly_frame_buffp
ffly_frame_buff_creat(ff_uint_t __width, ff_uint_t __height, ff_u8_t __chn_c, ff_err_t *__err) {
	ffly_frame_buffp fb = (ffly_frame_buffp)__ffly_mem_alloc(sizeof(struct ffly_frame_buff));
	ffly_frame_buff_init(fb, __width, __height, __chn_c);
	return fb;
}

ff_err_t
ffly_frame_buff_del(ffly_frame_buffp __fb) {
	ffly_frame_buff_de_init(__fb);
	__ffly_mem_free(__fb);
}

ff_err_t // should use mmap to allocate the frame?
ffly_frame_buff_init(ffly_frame_buffp __fb, ff_uint_t __width, ff_uint_t __height, ff_u8_t __chn_c) {
	__fb->frame = (ff_byte_t*)__ffly_mem_alloc((__width*__height)*__chn_c);
	__fb->width = __width;
	__fb->height = __height;
	__fb->chn_c = __chn_c;
}

ff_err_t
ffly_frame_buff_de_init(ffly_frame_buffp __fb) {
	__ffly_mem_free(__fb->frame);
}

ffly_frame_buffp __frame_buff__;
