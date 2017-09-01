# ifndef __ffly__alsa__audio__hpp
# define __ffly__alsa__audio__hpp
# include <alsa/asoundlib.h>
# include "types/err_t.h"
# include <mdlint.h>
# include <math.h>
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "types/adesc_t.h"
# include "types/afformat_t.h"
# include "ffly_audio.hpp"
# include "types/audio/wav_disc_t.h"
namespace mdl {
namespace firefly {
class alsa_audio {
	public:
	types::err_t de_init() {}
	types::err_t load_sample(char const *__fdir, char const *__fname, types::afformat_t __afformat) {
		if (this-> fbuff != nullptr) memory::mem_free(this-> fbuff);
		ffly_audio::load_aud_file(__fdir, __fname, __afformat, this-> fbuff, this-> fbuff_size);
		return FFLY_SUCCESS;
	}

	types::err_t configure(types::afformat_t __afformat, types::aformat_t __aformat);

	types::err_t play_sample(char const *__fdir, char const *__fname, types::afformat_t __afformat, types::aformat_t __aformat) {
		if (this-> load_sample(__fdir, __fname, __afformat) == FFLY_FAILURE)
			return FFLY_FAILURE;

		if (this-> configure(__afformat, __aformat) == FFLY_FAILURE)
			return FFLY_FAILURE;

		if (this-> play_sample(__afformat, __aformat) == FFLY_FAILURE)
			return FFLY_FAILURE;

		return FFLY_SUCCESS;
	}

	types::err_t play_sample(types::afformat_t __afformat, types::aformat_t __aformat) {
		switch(__afformat) {
			case system::WAV: {
				types::wav_disc_t *wav_disc = (types::wav_disc_t *)this-> aud_fdisc;
				types::adesc_t adesc = {
					.format = __aformat,
					.rate = wav_disc-> sample_rate,
					.chn_c = wav_disc-> chn_c
				};
				snd_pcm_uframes_t period_size = 32768;
				this-> play_sample(adesc, __aformat, 32, period_size);
				break;
			}
		}
	}

	types::err_t play_sample(types::adesc_t __adesc, types::aformat_t __aformat, uint_t __period_c, snd_pcm_uframes_t __period_size);

	private:
	types::afformat_t afformat;
	void *aud_fdisc = nullptr;
	off_t fbuff_size;
	u8_t *fbuff = nullptr;
	std::size_t abuff_size;
	u8_t *abuff = nullptr;
};
}
}

# endif /*__ffly__alsa__audio__hpp*/
