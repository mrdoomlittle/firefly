# include "../linux/socket.h"
# include "../ffly_def.h"
# include <mdlint.h>
mdl_s32_t recv(mdl_u32_t __fd, void *__buf, mdl_u32_t __size, mdl_u32_t __flags) {
	return recvfrom(__fd, __buf, __size, __flags, NULL, NULL);
}
