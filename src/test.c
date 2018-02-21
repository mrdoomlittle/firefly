# include "types/err_t.h"
# include "data/mem_cpy.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	mdl_u8_t dst[52488], src[52488];
	_ffly_mem_cpy(dst, src, 52488);
}
