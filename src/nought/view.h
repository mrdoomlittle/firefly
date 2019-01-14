# ifndef __ffly__nt__view__h
# define __ffly__nt__view__h
# include "../ffint.h"
#define nwivpgt(__x, __y, __where)\
	if ((__x)<nt_ctx->viewport.x || (__y)<nt_ctx->viewport.y || (__x)>nt_ctx->viewport.x+nt_ctx->viewport.width || (__y)>nt_ctx->viewport.y+nt_ctx->viewport.height) {goto __where;}

struct nt_viewport {
	ff_u32_t width, height;
	ff_u32_t x, y;
	
	ff_u32_t zfar, znear;
};
void nt_viewport(void);
# endif /*__ffly__nt__view__h*/
