# ifndef __ffly__wav__spec__t__h
# define __ffly__wav__spec__t__h
# include "../byte_t.h"
# include <mdlint.h>
typedef struct {
	ffly_byte_t cnk_id[4];
	mdl_u32_t cnk_size;
	ffly_byte_t format[4];

	ffly_byte_t sub_cnk1_id[4];
	mdl_u32_t sub_cnk1_size;
	mdl_u16_t aud_format;
	mdl_u16_t chn_c;
	mdl_u32_t sample_rate;
	mdl_u32_t byte_rate;
	mdl_u16_t blk_align;
	mdl_u16_t bit_depth;

	ffly_byte_t sub_cnk2_id[4];
	mdl_u32_t sub_cnk2_size;
} ffly_wav_spec_t;
# endif /*__ffly__wav__spec__t__h*/
