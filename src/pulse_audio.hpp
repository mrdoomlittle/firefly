# ifndef __ffly__pulse__audio__hpp
# define __ffly__pulse__audio__hpp
# include <pulse/simple.h>
# include <pulse/error.h>
# include <errno.h>
# include <cstdio>
# include <strcmb.hpp>
# include <boost/cstdint.hpp>
# include <sys/stat.h>
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/errno.h"
# include <fcntl.h>
# include "types/err_t.h"
# include "types/adisc_t.h"
# include "types/aftype_t.h"
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

	types::err_t load_sample(char const *__fdir, char const *__fname, types::aftype_t __aftype);

	types::err_t play_sample(char const *__fdir, char const *__fname, types::aftype_t __aftype, types::aformat_t __aformat);

	types::err_t play_sample(types::aftype_t __aftype, types::aformat_t __aformat) {
		switch(__aftype) {
			case system::WAV: {
				types::wav_disc_t *wav_disc = (types::wav_disc_t *)this-> aud_disc;
				types::adisc_t adisc = {
					.format = __aformat,
					.rate = wav_disc-> sample_rate,
					.chn_c = wav_disc-> chn_c
				};

				this-> play_sample(adisc);
				break;
			}
		}
		return FFLY_SUCCESS;
	}

	types::err_t load_info(types::aftype_t __aftype, types::aformat_t __aformat) {
		if (this-> aud_disc != nullptr) {memory::mem_free(this-> aud_disc); this-> aud_disc = nullptr;}
		this-> aftype = __aftype;
		switch(__aftype) {
			case system::WAV: {
				types::wav_disc_t *wav_disc = (types::wav_disc_t *)memory::mem_alloc(sizeof(types::wav_disc_t));
				types::err_t err;
				*wav_disc = ffly_audio::get_wav_fdisc(this-> fbuff, err);
				this-> aud_disc = wav_disc;
				this-> abuff = this-> fbuff + sizeof(types::wav_disc_t);
				this-> abuff_size = fbuff_size - sizeof(types::wav_disc_t);
				break;
			}
		}

		return FFLY_SUCCESS;
	}

	types::err_t play_sample(types::adisc_t __adisc, u8_t *__fbuff = nullptr, off_t __fbuff_size = 0);

	types::err_t play_sample(char const *__name, char const *__stream_name, struct pa_sample_spec *__sample_spec = nullptr, u8_t *__abuff = nullptr, std::size_t __abuff_size = 0);
	private:
	types::aftype_t aftype;
	void *aud_disc = nullptr;
	off_t fbuff_size = 0;
	u8_t *fbuff = nullptr;
	u8_t *abuff = nullptr;
	uint_t abuff_size = 0;
};

}
}

# endif /*__ffly__pulse__audio__hpp*/
