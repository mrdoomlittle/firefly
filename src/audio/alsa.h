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
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_alsa_write(mdl_u32_t, mdl_u16_t, snd_pcm_format_t, mdl_uint_t, snd_pcm_uframes_t, ffly_byte_t*, ffly_size_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__alsa__h*/
