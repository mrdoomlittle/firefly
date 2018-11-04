# ifndef __ffly__nt__objbuf__h
# define __ffly__nt__objbuf__h
# include "../ffint.h"
#define NT_OB_MAPPED 0x01
typedef struct nt_objbuf {
	ff_u32_t size;
	ff_u8_t flags;
	void *data;
} *nt_objbufp;

void nt_objbuf_new(void);
void nt_objbuf_destroy(void);
void nt_objbuf_map(void);
void nt_objbuf_unmap(void);
void nt_objbuf_write(void);
void nt_objbuf_read(void);
# endif /*__ffly__nt__objbuf__h*/
