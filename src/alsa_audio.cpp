# include "alsa_audio.hpp"
mdl::firefly::types::err_t mdl::firefly::alsa_audio::configure(types::afformat_t __afformat, types::aformat_t __aformat) {
	if (this-> aud_fdisc != nullptr) {memory::mem_free(this-> aud_fdisc); this-> aud_fdisc = nullptr;}
	this-> afformat = __afformat;

	switch(__afformat) {
		case system::WAV: {
			types::wav_disc_t *wav_disc = (types::wav_disc_t *)memory::mem_alloc(sizeof(types::wav_disc_t));
			if (wav_disc == NULL) {
				fprintf(stderr, "alsa_audio: failed to alloc memory for wav disc, errno: %d\n", errno);
				return FFLY_FAILURE;
			}

			types::err_t err;
			*wav_disc = ffly_audio::get_wav_fdisc(this-> fbuff, err);
			if (err == FFLY_FAILURE) return FFLY_FAILURE;

			this-> aud_fdisc = wav_disc;
			this-> abuff = this-> fbuff + sizeof(types::wav_disc_t);
			this-> abuff_size = this-> fbuff_size - sizeof(types::wav_disc_t);
			break;
		}
		default:
			fprintf(stderr, "alsa_audio: failed to lookup file format.\n");
			return FFLY_NOP;
	}
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::alsa_audio::play_sample(types::adesc_t __adesc, types::aformat_t __aformat, uint_t __period_c, snd_pcm_uframes_t __period_size) {
	snd_pcm_t *pcm_handle = NULL;
	snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
	snd_pcm_hw_params_t *hw_params;

	int any_err;
	char *pcm_nme = strdup("default");
	snd_pcm_hw_params_alloca(&hw_params);

	if ((any_err = snd_pcm_open(&pcm_handle, pcm_nme, stream, 0)) < 0) {
		fprintf(stderr, "alsa_audio: failed to open device, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_any(pcm_handle, hw_params)) < 0) {
		fprintf(stderr, "alsa_audio: failed to configure device, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_set_access(pcm_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf(stderr, "alsa_audio: failed to set device access settings, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	uint_t blk_len;
	snd_pcm_format_t format;
	switch(__aformat) {
		case system::S16_LE:
			format = SND_PCM_FORMAT_S16_LE;
			blk_len = 2;
		break;
		case system::FLOAT32_LE:
			format = SND_PCM_FORMAT_FLOAT_LE;
			blk_len = 4;
		break;
		default:
			fprintf(stderr, "alsa_audio: current format is not surported.\n");
			return FFLY_NOP;
	}

	if ((any_err = snd_pcm_hw_params_set_format(pcm_handle, hw_params, format)) < 0) {
		fprintf(stderr, "alsa_audio: failed to apply audio format, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_set_rate(pcm_handle, hw_params, __adesc.rate, 0)) < 0) {
		fprintf(stderr, "alsa_audio: bit rate is not surported or ?, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_set_channels(pcm_handle, hw_params, __adesc.chn_c)) < 0) {
		fprintf(stderr, "alsa_audio: failed to set channels, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_set_periods(pcm_handle, hw_params, __period_c, 0)) < 0) {
		fprintf(stderr, "alsa_audio: failed to set periods, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_set_buffer_size(pcm_handle, hw_params, (__period_c * __period_size) >> 2)) < 0) {
		fprintf(stderr, "alsa_audio: failed to set buff size, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params(pcm_handle, hw_params)) < 0) {
		fprintf(stderr, "alsa_audio: failed to apply params to device, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_prepare(pcm_handle)) < 0) {
		fprintf(stderr, "alsa_audio: failed to prepare device for use, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	blk_len = __adesc.chn_c * blk_len;

	uint_t frame_c = __period_size >> 2;
	uint_t fs = blk_len * frame_c;

	uint_t ab_fc = floor(this-> abuff_size/(frame_c * blk_len));
	for (uint_t ab_frame = 0; ab_frame != ab_fc; ab_frame ++) {
		snd_pcm_wait(pcm_handle, 1000);
		while(snd_pcm_writei(pcm_handle, this-> abuff + ab_frame * fs, frame_c) < 0)
			snd_pcm_prepare(pcm_handle);
	}

	snd_pcm_drain(pcm_handle);
	snd_pcm_close(pcm_handle);
	return FFLY_SUCCESS;
}

