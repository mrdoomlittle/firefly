# ifndef __ffly__nt__tex__h
# define __ffly__nt__tex__h
# include "../ffint.h"
#define NT_TB_MAPPED 0x01

typedef struct nt_texbuf {
    ff_u32_t size;
    ff_u8_t flags;
    void *data;
} *nt_texbufp;

typedef struct nt_tex {
	nt_texbufp b;
} *nt_texp;

void nt_tex_new(void);
void nt_tex_destroy(void);

void nt_texbuf_new(void);
void nt_texbuf_destroy(void);
void nt_texbuf_map(void);
void nt_texbuf_unmap(void);
void nt_texbuf_write(void);
void nt_texbuf_read(void);

# endif /*__ffly__nt__tex__h*/
