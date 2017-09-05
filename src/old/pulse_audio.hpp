# ifndef __ffly__pulse__audio__hpp
# define __ffly__pulse__audio__hpp
# include <pulse/simple.h>
# include <pulse/error.h>
# include <errno.h>
# include <math.h>
# include <cstdio>
# include <strcmb.hpp>
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/errno.h"
# include "types/err_t.h"
# include "types/adesc_t.h"
# include "types/afformat_t.h"
# include "ffly_audio.hpp"
# include "types/audio/wav_disc_t.h"
namespace mdl {
namespace firefly {
class pulse_audio {
	private:
	public:

	types::err_t de_init() {
		if (this-> fbuff != nullptr)
			memory::mem_free(this-> fbuff);
		return FFLY_SUCCESS;
	}

	types::err_t load_sample(char const *__fdir, char const *__fname, types::afformat_t __afformat) {
		if (this-> fbuff != nullptr) memory::mem_free(this-> fbuff);
		ffly_audio::load_aud_file(__fdir, __fname, __afformat, this-> fbuff, this-> fbuff_size);
		return FFLY_SUCCESS;
	}

	types::err_t play_sample(char const *__fdir, char const *__fname, types::afformat_t __afformat, types::aformat_t __aformat) {
		if (this-> load_sample(__fdir, __fname, __afformat) == FFLY_FAILURE)
			return FFLY_FAILURE;
		if (this-> configure(__afformat, __aformat) == FFLY_FAILURE)
			return FFLY_FAILURE;
		if (this-> play_sample(__afformat, __aformat) == FFLY_FAILURE)
			return FFLY_FAILURE;
		return FFLY_SUCCESS;
	}

	types::err_t play_sample(types::afformat_t __afformat, types::aformat_t __aformat);

	types::err_t configure(types::afformat_t __afformat, types::aformat_t __aformat);

	types::err_t play_sample(types::adesc_t __adesc, u8_t *__abuff = nullptr, std::size_t __abuff_size = 0);

	types::err_t play_sample(char const *__name, char const *__stream_name, struct pa_sample_spec *__sample_spec = nullptr, u8_t *__abuff = nullptr, std::size_t __abuff_size = 0);
	private:
	types::afformat_t afformat;
	void *aud_disc = nullptr;
	off_t fbuff_size = 0;
	u8_t *fbuff = nullptr;
	u8_t *abuff = nullptr;
	uint_t abuff_size = 0;
};

}
}

# endif /*__ffly__pulse__audio__hpp*/
