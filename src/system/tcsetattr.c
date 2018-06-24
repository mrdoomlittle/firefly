# include "../ffint.h"
# include "../linux/ioctl.h"
# include "../linux/ioctls.h"
# include "../linux/termios.h"
# include "../dep/mem_cpy.h"
ff_s32_t tcsetattr(ff_u32_t __fd, struct termios *__tr) {
	ff_u64_t cmd;

	cmd = TCSETS;

	struct termios tr;
	tr.c_iflag = __tr->c_iflag;
	tr.c_oflag = __tr->c_oflag;
	tr.c_cflag = __tr->c_cflag;
	tr.c_lflag = __tr->c_lflag;
	tr.c_line = __tr->c_line;

	ffly_mem_cpy(tr.c_cc, __tr->c_cc, NCC);
	return ioctl(__fd, cmd, &tr);
}
