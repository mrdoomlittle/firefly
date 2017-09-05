# ifndef __ffly__audio__h
# define __ffly__audio__h
# include "system/errno.h"
# include <mdlint.h>
# include <fcntl.h>
# include <sys/stat.h>
# include "types/err_t.h"
# include "types/aud_fformat_t.h"
# include "types/aud_format_t.h"
# include "types/audio/wav_spec_t.h"
# include "types/aud_spec_t.h"
# include "types/byte_t.h"
# include "types/size_t.h"
# include "audio/pulse.h"
# include "audio/alsa.h"
# include "data/pair.h"
struct ffly_audio {
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_ld_aud_file(char*, char*, ffly_aud_fformat_t, ffly_byte_t**, ffly_size_t*);
ffly_wav_spec_t ffly_extract_wav_spec(ffly_byte_t*);
ffly_err_t ffly_aud_play(char*, char*, ffly_aud_fformat_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__audio__h*/
