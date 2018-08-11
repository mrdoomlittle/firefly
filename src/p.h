# ifndef __ffly__p__h
# define __ffly__p__h
# include "ffint.h"
# include "types.h"
# include "system/file.h"
typedef struct ff_pimage {
	ff_uint_t width, height;
} *ff_pimage;

typedef struct ff_pstruc {
	ff_uint_t width, height;
	ff_u32_t off;
	ff_uint_t row_num;
	void *arg;
	void(*read)(void*, ff_u32_t, ff_uint_t, void*);
	void(*write)(void*, ff_u32_t, ff_uint_t, void*);
} *ff_pstrucp;

typedef struct ff_pinfo {
	ff_uint_t width, height;
} *ff_pinfop;

void ff_p_info_read(ff_pstrucp, ff_pinfop);
ff_pstrucp ff_p_struc_creat(void);
void ff_p_io_init(ff_pstrucp, void(*)(void*, ff_u32_t, ff_uint_t, void*), void(*)(void*, ff_u32_t, ff_uint_t, void*), void*);
void ff_p_row_read(ff_pstrucp, ff_u8_t*);
void ff_p_row_write(ff_pstrucp, ff_u8_t*);
void ff_p_struc_destroy(ff_pstrucp);
void ff_p_reader_init(ff_pstrucp);
void ff_p_set_hdr(ff_pstrucp);
void ff_p_set_prop(ff_pstrucp, ff_uint_t, ff_uint_t);
# endif /*__ffly__p__h*/
