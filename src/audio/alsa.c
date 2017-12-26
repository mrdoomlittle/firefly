# include "alsa.h"
# include "../system/io.h"
# include "../ffly_maths.h"
# include "../system/errno.h"
# include "../data/str_dupe.h"
ffly_err_t ffly_alsa_init(struct ffly_alsa *__alsa, mdl_u32_t __rate, mdl_u16_t __chn_c, snd_pcm_format_t __format, mdl_uint_t __period_c, mdl_uint_t __period_size) {
    __alsa->stream = SND_PCM_STREAM_PLAYBACK;
    __alsa->format = __format;
    char *pcm_name = ffly_str_dupe("default");
    __alsa->period_size = __period_size;
    __alsa->period_c = __period_c;

    switch(__format) {
        case SND_PCM_FORMAT_S16_LE:
            __alsa->period_size = __period_size<<1;
            __alsa->frame_size = __chn_c*2;
            __alsa->frame_shift = __chn_c*1; 
        break;
        case SND_PCM_FORMAT_FLOAT_LE:
            __alsa->period_size = __period_size<<2;
            __alsa->frame_size = __chn_c*4;
            __alsa->frame_shift = __chn_c*2;
        break;
        default: break;
    }

    int err;
    snd_pcm_hw_params_alloca(&__alsa->hw_params);
    if ((err = snd_pcm_open(&__alsa->handle, pcm_name, __alsa->stream, 0)) < 0) {
        ffly_fprintf(ffly_err, "alsa: failed to open device, err: %d\n", err);
        return FFLY_FAILURE;
    }

    if ((err = snd_pcm_hw_params_any(__alsa->handle, __alsa->hw_params)) < 0) {
        ffly_fprintf(ffly_err, "alsa: failed to configure device, err: %d\n", err);
        return FFLY_FAILURE;
    }

    if ((err = snd_pcm_hw_params_set_access(__alsa->handle, __alsa->hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        ffly_fprintf(ffly_err, "alsa: failed to set device access settings, err: %d\n", err);
        return FFLY_FAILURE;
    }

    if ((err = snd_pcm_hw_params_set_format(__alsa->handle, __alsa->hw_params, __format)) < 0) {
        ffly_fprintf(ffly_err, "alsa: failed to set audio format, err: %d\n", err);
        return FFLY_FAILURE;
    }

    if ((err = snd_pcm_hw_params_set_rate(__alsa->handle, __alsa->hw_params, __rate, 0)) < 0) {
        ffly_fprintf(ffly_err, "alsa: failed to set audio rate, err: %d\n", err);
        return FFLY_FAILURE;
    }

    if ((err = snd_pcm_hw_params_set_channels(__alsa->handle, __alsa->hw_params, __chn_c)) < 0) {
        ffly_fprintf(ffly_err, "alsa: failed to set audio channels, err: %d\n", err);
        return FFLY_FAILURE;
    }

    if ((err = snd_pcm_hw_params_set_periods(__alsa->handle, __alsa->hw_params, __period_c, 0)) < 0) {
        ffly_fprintf(ffly_err, "alsa: failed to set periods, err: %d\n", err);
        return FFLY_FAILURE;
    }

    if ((err = snd_pcm_hw_params_set_buffer_size(__alsa->handle, __alsa->hw_params, __period_c*__alsa->period_size)) < 0) {
        ffly_fprintf(ffly_err, "alsa: failed to set buff size, err: %d\n", err);
        return FFLY_FAILURE;
    }

    if ((err = snd_pcm_hw_params(__alsa->handle, __alsa->hw_params)) < 0) {
        ffly_fprintf(ffly_err, "alsa: failed to apply params to device, err: %d\n", err);
        return FFLY_FAILURE;
    }

    if ((err = snd_pcm_prepare(__alsa->handle)) < 0) {
        ffly_fprintf(ffly_err, "alsa: failed to prepare device for use, err: %d\n");
        return FFLY_FAILURE;
    }
}

ffly_err_t ffly_alsa_write(struct ffly_alsa *__alsa, ffly_byte_t *__p, ffly_size_t __size) {
    ffly_byte_t *itr = __p;

    mdl_uint_t frame_c;
    while(itr < __p+__size) {
        snd_pcm_wait(__alsa->handle, 1000);
        mdl_uint_t left = __size-(itr-__p);
        frame_c = (left>>__alsa->frame_shift) >= __alsa->period_size?__alsa->period_size:left>>__alsa->frame_shift; 
        while(snd_pcm_writei(__alsa->handle, itr, frame_c) < 0)
            snd_pcm_prepare(__alsa->handle);
        itr+= frame_c<<__alsa->frame_shift;
    }
}

ffly_err_t ffly_alsa_drain(struct ffly_alsa *__alsa) {
    snd_pcm_drain(__alsa->handle);
}

ffly_err_t ffly_alsa_free(struct ffly_alsa *__alsa) {
    snd_pcm_close(__alsa->handle);
}
/*
ffly_err_t ffly_alsa_init(mdl_u32_t __rate, mdl_u16_t __chn_c, snd_pcm_format_t __format, mdl_uint_t __period_c, snd_pcm_uframes_t __period_size, ffly_byte_t *__p, ffly_size_t __size) {

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
}*/
