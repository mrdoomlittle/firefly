# ifndef __ffly__sr__raise__h
# define __ffly__sr__raise__h
# include "../ffint.h"
# define STACK_SIZE 256

# define sr_op_raster_tri2 0x00
# define sr_op_ctx_new 0x01
# define sr_op_ctx_destroy 0x02
# define sr_op_putframe 0x03
# define sr_op_setctx 0x04
# define sr_op_start 0x05
# define sr_op_finish 0x06
# define sr_op_pixcopy 0x07
# define sr_op_pixdraw 0x08
# define sr_op_pixfill 0x09
# define sr_op_fb_set 0x0a
# define sr_op_fb_new 0x0b
# define sr_op_fb_destroy 0x0c
# define stack_at(__adr) \
	(sr_raise_stack+(__adr))
extern ff_u8_t *sr_raise_p;
extern ff_u8_t sr_raise_stack[STACK_SIZE];
extern ff_u16_t sr_raise_sp;
void sr_raise(ff_u8_t*, ff_uint_t);
# endif /*__ffly__sr__raise__h*/
