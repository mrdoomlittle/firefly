# ifndef __ffly__nt__shit__h
# define __ffly__nt__shit__h
# include "../ffint.h"
# define NT_BLEND 0x01

# define is_bit(__bits, __bit) \
	(((__bits)&(__bit))>0)
ff_u8_t extern nt_bits;

void nt_sb(void);
void nt_cb(void);
/*
	TODO:
		rename file

		dont know what to name it and im not spending a large amount of time on this
*/
void nt_tdraw(void);
void nt_pixcopy(void);
void nt_pixdraw(void);
void nt_pixfill(void);
# endif /*__ffly__nt__shit__h*/
