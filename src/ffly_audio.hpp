# ifndef __ffly__audio__hpp
# define __ffly__audio__hpp
# include "types/afformat_t.h"
# include "types/aformat_t.h"
# include "types/audio/wav_disc_t.h"
# include <errno.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <cstdio>
# include "types/err_t.h"
# include <eint_t.hpp>
# include "system/errno.h"
# include <strcmb.hpp>
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
namespace mdl {
namespace ffly_audio {
firefly::types::err_t load_aud_file(char const *__fdir, char const *__fname, firefly::types::afformat_t __afformat, u8_t*& __fbuff, off_t& __fsize);
firefly::types::wav_disc_t get_wav_fdisc(u8_t *__raw_data, firefly::types::err_t& __any_err);
firefly::types::wav_disc_t get_wav_fdisc(char const *__fdir, char const *__fname, firefly::types::err_t& __any_err);
}
}

# endif /*__ffly__audio__hpp*/
