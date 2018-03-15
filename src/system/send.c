# include "../linux/socket.h"
# include "../ffly_def.h"
# include <mdlint.h>
mdl_s32_t send(mdl_u32_t __fd, void *__buf, mdl_u32_t __size, mdl_u32_t __flags) {
	return sendto(__fd, __buf, __size, __flags, NULL, 0);
}
