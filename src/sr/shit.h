# ifndef __ffly__sr__shit__h
# define __ffly__sr__shit__h
# include "../ffint.h"
# define SR_BLEND 0x01

# define is_bit(__bits, __bit) \
	(((__bits)&(__bit))>0)
ff_u8_t extern sr_bits;

void sr_sb(void);
void sr_cb(void);
/*
	rename
*/
void sr_tdraw(void);
void sr_pixcopy(void);
void sr_pixdraw(void);
void sr_pixfill(void);
# endif /*__ffly__sr__shit__h*/
