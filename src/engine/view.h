# ifndef __ffly__engine__view__h
# define __ffly__engine__view__h
# include "../ffint.h"
#define FF_DF_VPW 640
#define FF_DF_VPH 640
#define EF_WIDTH ef_width
#define EF_HEIGHT ef_height
extern ff_u16_t ef_width;
extern ff_u16_t ef_height;

#define fw ef_width
#define fh ef_height
#define setframesize(__width, __height)\
	ef_width = __width;\
	ef_height = __height
# endif /*__ffly__engine__view__h*/
