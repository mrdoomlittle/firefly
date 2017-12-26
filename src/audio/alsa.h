# ifndef __ffly__alsa__h
# define __ffly__alsa__h
# include "../types/err_t.h"
# include "../types/byte_t.h"
# include "../types/size_t.h"
# define ALSA_PCM_NEW_HW_PARAMS_API
# define _POSIX_SOURCE
# include <alloca.h>
# include <alsa/asoundlib.h>
# include <mdlint.h>

struct ffly_alsa {
    mdl_uint_t period_c, period_size, frame_size, frame_shift;
    snd_pcm_t *handle;
    snd_pcm_stream_t stream;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_format_t format;
};
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_alsa_init(struct ffly_alsa*, mdl_u32_t, mdl_u16_t, snd_pcm_format_t, mdl_uint_t, mdl_uint_t);
ffly_err_t ffly_alsa_write(struct ffly_alsa*, ffly_byte_t*, ffly_size_t);
ffly_err_t ffly_alsa_drain(struct ffly_alsa*);
ffly_err_t ffly_alsa_free(struct ffly_alsa*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__alsa__h*/
