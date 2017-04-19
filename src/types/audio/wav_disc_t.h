# ifndef __wav__disc__t__h
# define __wav__disc__t__h
# include <eint_t.h>
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	u8_t cnk_id[4];
	u32_t cnk_size;
	u8_t format[4];

	u8_t sub_cnk1_id[4];
	u32_t sub_cnk1_size;
	u16_t aud_format;
	u16_t chn_c;
	u32_t sample_rate;
	u32_t byte_rate;
	u16_t blk_align;
	u16_t bit_depth;

	u8_t sub_cnk2_id[4];
	u32_t sub_cnk2_size;
} wav_disc_t;
}
}
}

# endif /*__wav__disc__t__h*/
