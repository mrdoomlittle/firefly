# include "alsa.h"
# include "../system/io.h"
# include "../ffly_maths.h"
# include "../system/errno.h"
ffly_err_t ffly_alsa_write(mdl_u32_t __rate, mdl_u16_t __chn_c, snd_pcm_format_t __format, mdl_uint_t __period_c, snd_pcm_uframes_t __period_size, ffly_byte_t *__p, ffly_size_t __size) {
	snd_pcm_t *pcm_handle = NULL;
	snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
	snd_pcm_hw_params_t *hw_params;

	int any_err;
	char *pcm_name = strdup("default");
	snd_pcm_hw_params_alloca(&hw_params);

	if ((any_err = snd_pcm_open(&pcm_handle, pcm_name, stream, 0)) < 0) {
		ffly_printf(stderr, "alsa: failed to open device, err: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_any(pcm_handle, hw_params)) < 0) {
		ffly_printf(stderr, "alsa: failed to configure device, err: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_set_access(pcm_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		ffly_printf(stderr, "alsa: failed to set device access settings, err: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_set_format(pcm_handle, hw_params, __format)) < 0) {
		ffly_printf(stderr, "alsa: failed to set audio format, err: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_set_rate(pcm_handle, hw_params, __rate, 0)) < 0) {
		ffly_printf(stderr, "alsa: failed to set audio rate, err: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_set_channels(pcm_handle, hw_params, __chn_c)) < 0) {
		ffly_printf(stderr, "alsa: failed to set audio channels, err: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_set_periods(pcm_handle, hw_params, __period_c, 0)) < 0) {
		ffly_printf(stderr, "alsa: failed to set periods, err: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params_set_buffer_size(pcm_handle, hw_params, (__period_c*__period_size) >> 2)) < 0) {
		ffly_printf(stderr, "alsa: failed to set buff size, err: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_hw_params(pcm_handle, hw_params)) < 0) {
		ffly_printf(stderr, "alsa: failed to apply params to device, err: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = snd_pcm_prepare(pcm_handle)) < 0) {
		ffly_printf(stderr, "alsa: failed to prepare device for use, err: %d\n");
		return FFLY_FAILURE;
	}

	mdl_u8_t blk_len;
	switch(__format) {
		case SND_PCM_FORMAT_S16_LE:
			blk_len = 2;
		break;

		case SND_PCM_FORMAT_FLOAT_LE:
			blk_len = 4;
		break;
		default: break;
	}

	mdl_uint_t frame_c = __period_size >> 2;
	mdl_uint_t off = blk_len*frame_c;

	mdl_uint_t ab_fc = ffly_floor(__size/(frame_c*blk_len));
	mdl_uint_t f = 0;
	while(f != ab_fc) {
		snd_pcm_wait(pcm_handle, 1000);
		while(snd_pcm_writei(pcm_handle, __p+f*off, frame_c) < 0)
			snd_pcm_prepare(pcm_handle);
		f++;
	}

	snd_pcm_drain(pcm_handle);
	snd_pcm_close(pcm_handle);
	return FFLY_SUCCESS;
}
