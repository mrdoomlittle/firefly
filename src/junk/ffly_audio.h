# ifndef __ffly__audio__h
# define __ffly__audio__h
# include <mdlint.h>
# include "types/err_t.h"
# include "types/aud_fformat_t.h"
# include "types/aud_format_t.h"
# include "types/audio/wav_spec_t.h"
# include "types/aud_spec_t.h"
# include "types/byte_t.h"
# include "types/size_t.h"
# ifdef __ffly_use_pulse_audio
# include "audio/pulse.h"
# endif
# ifdef __ffly_use_alsa_audio
# include "audio/alsa.h"
# endif

# include "data/pair.h"

struct ffly_audio {
	ffly_byte_t *p;
	ffly_aud_spec_t spec;
	ffly_size_t size;
};
typedef struct ffly_audio* ffly_audiop;

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_audio_init(ffly_aud_spec_t*);
ffly_err_t ffly_audio_de_init();
ffly_err_t ffly_ld_aud(char*, char*, ffly_aud_fformat_t, ffly_audiop);
void ffly_extract_wav_spec(ffly_wav_spec_t*, ffly_byte_t*);
//ffly_err_t ffly_aud_play(ffly_byte_t*, ffly_size_t, ffly_aud_fformat_t);
//ffly_err_t ffly_aud_raw_play(ffly_byte_t*, ffly_size_t, ffly_aud_spec_t*);

ffly_err_t ffly_aud_write(ffly_byte_t*, mdl_uint_t);
ffly_err_t ffly_aud_drain();
ffly_err_t ffly_aud_read(ffly_byte_t*, mdl_uint_t);
# ifdef __cplusplus
}
namespace mdl {
namespace ffly_audio {
firefly::types::err_t __inline__ static init() {
	firefly::types::aud_spec_t aud_spec = (firefly::types::aud_spec_t) {
		.format = _ffly_af_s16_le,
		.rate = 48000,
		.chn_c = 2
	};
	return ffly_audio_init(&aud_spec);
}
firefly::types::err_t __inline__ static de_init() {
	return ffly_audio_de_init();
}
}
namespace firefly {
namespace audio {
static types::err_t(*write)(types::byte_t*, mdl_uint_t) = &ffly_aud_write;
static types::err_t(*read)(types::byte_t*, mdl_uint_t) = &ffly_aud_read;
static types::err_t(*drain)() = &ffly_aud_drain;
}
}
}
# endif
# endif /*__ffly__audio__h*/
