# include "../ffint.h"
# include "../linux/ioctl.h"
# include "../linux/ioctls.h"
# include "../linux/termios.h"
# include "../dep/mem_cpy.h"
ff_s32_t tcgetattr(ff_u32_t __fd, struct termios *__tr) {
	struct termios tr;
	ff_s32_t retv;

	retv = ioctl(__fd, TCGETS, &tr);
	__tr->c_iflag = tr.c_iflag;
	__tr->c_oflag = tr.c_oflag;
	__tr->c_cflag = tr.c_cflag;
	__tr->c_lflag = tr.c_lflag;
	__tr->c_line = tr.c_line;

	ffly_mem_cpy(__tr->c_cc, tr.c_cc, NCC);	
	return retv;
}
