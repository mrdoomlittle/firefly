# include "pulse_audio.hpp"
static const pa_sample_spec def_sample_spec = {
	.format = PA_SAMPLE_S16LE,
	.rate = 44100,
	.channels = 2
};

mdl::firefly::types::err_t mdl::firefly::pulse_audio::play_sample(types::afformat_t __afformat, types::aformat_t __aformat) {
	switch(__afformat) {
		case system::WAV: {
			types::wav_disc_t *wav_disc = (types::wav_disc_t *)this-> aud_disc;
			types::adesc_t adesc = {
				.format = __aformat,
				.rate = wav_disc-> sample_rate,
				.chn_c = wav_disc-> chn_c
			};

			this-> play_sample(adesc);
			break;
		}
		default:
			fprintf(stderr, "pulse_audio: failed to lookup file format.\n");
			return FFLY_NOP;
	}
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::pulse_audio::configure(types::afformat_t __afformat, types::aformat_t __aformat) {
	if (this-> aud_disc != nullptr) {memory::mem_free(this-> aud_disc); this-> aud_disc = nullptr;}
	this-> afformat = __afformat;
	switch(__afformat) {
		case system::WAV: {
			types::wav_disc_t *wav_disc = (types::wav_disc_t *)memory::mem_alloc(sizeof(types::wav_disc_t));
			if (wav_disc == NULL) {
				fprintf(stderr, "puse_audio: failed to alloc memory for wav disc, errno: %d\n", errno);
				return FFLY_FAILURE;
			}

			types::err_t err;
			*wav_disc = ffly_audio::get_wav_fdisc(this-> fbuff, err);
			if (err == FFLY_FAILURE) return FFLY_FAILURE;

			this-> aud_disc = wav_disc;
			this-> abuff = this-> fbuff + sizeof(types::wav_disc_t);
			this-> abuff_size = this-> fbuff_size - sizeof(types::wav_disc_t);
			break;
		}
		default:
			fprintf(stderr, "pulse_audio: failed to lookup file format.\n");
			return FFLY_NOP;
	}
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::pulse_audio::play_sample(types::adesc_t __adesc, u8_t *__abuff, std::size_t __abuff_size) {
	pa_sample_spec *sample_spec = (pa_sample_spec *)memory::mem_alloc(sizeof(pa_sample_spec));
	if (sample_spec == NULL) return FFLY_FAILURE;

	pa_sample_format_t format;

	switch (__adesc.format) {
		case system::S16_LE:
			format = PA_SAMPLE_S16LE;
		break;
		case system::FLOAT32_LE:
			format = PA_SAMPLE_FLOAT32LE;
		break;
		default:
			fprintf(stderr, "pulse_audio: failed to lookup audio format.\n");
			memory::mem_free(sample_spec);
			return FFLY_NOP;
	}

	sample_spec-> format = format;
	sample_spec-> rate = __adesc.rate;
	sample_spec-> channels = __adesc.chn_c;

	if (this-> play_sample("firefly-audio", "firefly-stream", sample_spec, __abuff, __abuff_size) == FFLY_FAILURE) {
		fprintf(stderr, "pulse_audio: failed to play sample.\n");
		memory::mem_free(sample_spec);
		return FFLY_FAILURE;
	}

	memory::mem_free(sample_spec);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::pulse_audio::play_sample(char const *__name, char const *__stream_name, struct pa_sample_spec *__sample_spec, u8_t *__abuff, std::size_t __abuff_size) {
//	if (this-> fbuff == nullptr) {
//		fprintf(stderr, "audio file buff empty.\n");
//		return FFLY_FAILURE;
//	}

	printf("%d - %d\n", this-> fbuff_size, this-> abuff_size);

	pa_simple *simple = NULL;

	int any_err;

	std::size_t abuff_size = 0;
	if (!(simple = pa_simple_new(NULL, __name, PA_STREAM_PLAYBACK, NULL, __stream_name, __sample_spec == nullptr? &def_sample_spec : __sample_spec, NULL, NULL, &any_err))) {
		fprintf(stderr, "pulse_audio: failed to create new pulse audio sample, errno: %d\n", any_err);
		goto failure;
	}

	abuff_size = __abuff == nullptr? this-> abuff_size : __abuff_size;
	if (__sample_spec-> format == PA_SAMPLE_S16LE)
		abuff_size = floor(abuff_size/(__sample_spec-> channels * 2)) * (__sample_spec-> channels * 2);

	if (__sample_spec-> format == PA_SAMPLE_FLOAT32LE)
		abuff_size = floor(abuff_size/(__sample_spec-> channels * 4)) * (__sample_spec-> channels * 4);

	if (pa_simple_write(simple, __abuff == nullptr? this-> abuff : __abuff, abuff_size * sizeof(u8_t), &any_err) < 0) {
		fprintf(stderr, "pulse_audio: failed to write pa sample, errno: %s\n", pa_strerror(any_err));
		goto failure;
	}

	if (pa_simple_drain(simple, &any_err) < 0) {
		fprintf(stderr, "pulse_audio: failed to drain pa sample, errno: %s\n", pa_strerror(any_err));
		goto failure;
	}

	if (simple) pa_simple_free(simple);
	return FFLY_SUCCESS;
	failure:

	if (simple) pa_simple_free(simple);
	return  FFLY_FAILURE;
}
