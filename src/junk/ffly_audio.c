# include "ffly_audio.h"
# include "system/io.h"
# include "system/errno.h"
# include <mdl/str_cmb.h>
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "types/off_t.h"
# include "system/err.h"
# include "system/file.h"
# ifdef __ffly_use_pulse_audio
struct ffly_pulse __pulse__;
# endif
# ifdef __ffly_use_alsa_audio
struct ffly_alsa __alsa__;
# endif
ffly_err_t ffly_ld_aud(char *__dir, char *__name, ffly_aud_fformat_t __format, ffly_audiop __audio) {
	ffly_size_t header_size;
	char *ext;
	switch(__format) {
		case _ffly_audf_wav:
			ext = ".wav";
			header_size = 44;
		break;
		default:
			return FFLY_FAILURE;
	}

	char *path = mdl_str_cmb(__dir, mdl_str_cmb(__name, ext, 0), _mdl_stc_free_rhs);
	ffly_err_t err;
	FF_FILE *f = ffly_fopen(path, FF_O_RDONLY, 0, &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "ffly_audio: failed to open audio file at '%s'.\n", path);
		return err;
	}

	struct ffly_stat st;
	if (_err(err = ffly_fstat(path, &st))) {
		ffly_fprintf(ffly_err, "ffly_audio: failed to stat audio file at '%s'.\n", path);
		ffly_fclose(f);
		return err;
	}

	__ffly_mem_free(path);
	ffly_byte_t *p = (ffly_byte_t*)__ffly_mem_alloc(header_size);
	ffly_fread(f, p, header_size);

	switch(__format) {
		case _ffly_audf_wav: {
			ffly_wav_spec_t spec;
			ffly_extract_wav_spec(&spec, p);
			__audio->spec.rate = spec.sample_rate;
			__audio->spec.chn_c = spec.chn_c;
			switch(spec.bit_depth) {
				case 16:
					__audio->spec.format = _ffly_af_s16_le;
				break;
			}
		}
	}
	__ffly_mem_free(p);

	__audio->p = (ffly_byte_t*)__ffly_mem_alloc(__audio->size = (st.size-header_size));
	ffly_fseek(f, header_size, FF_SEEK_SET);
	ffly_fread(f, __audio->p, __audio->size);
	ffly_fclose(f);
	return FFLY_SUCCESS;
}

# include "data/mem_set.h"
void ffly_extract_wav_spec(ffly_wav_spec_t *__spec, ffly_byte_t *__p) {
	ffly_mem_set(__spec, 0, sizeof(ffly_wav_spec_t));

	__spec->cnk_id[0] = *(__p++);
	__spec->cnk_id[1] = *(__p++);
	__spec->cnk_id[2] = *(__p++);
	__spec->cnk_id[3] = *(__p++);

	__spec->cnk_size |= (__spec->cnk_size&0xFF) | *(__p++);
	__spec->cnk_size |= (__spec->cnk_size&0xFF) | *(__p++)<<8;
	__spec->cnk_size |= (__spec->cnk_size&0xFF) | *(__p++)<<16;
	__spec->cnk_size |= (__spec->cnk_size&0xFF) | *(__p++)<<24;

	__spec->format[0] = *(__p++);
	__spec->format[1] = *(__p++);
	__spec->format[2] = *(__p++);
	__spec->format[3] = *(__p++);

	__spec->sub_cnk1_id[0] = *(__p++);
	__spec->sub_cnk1_id[1] = *(__p++);
	__spec->sub_cnk1_id[2] = *(__p++);
	__spec->sub_cnk1_id[3] = *(__p++);

	__spec->sub_cnk1_size |= (__spec->sub_cnk1_size&0xFF) | *(__p++);
	__spec->sub_cnk1_size |= (__spec->sub_cnk1_size&0xFF) | *(__p++)<<8;
	__spec->sub_cnk1_size |= (__spec->sub_cnk1_size&0xFF) | *(__p++)<<16;
	__spec->sub_cnk1_size |= (__spec->sub_cnk1_size&0xFF) | *(__p++)<<24;

	__spec->aud_format |= (__spec->aud_format&0xFF) | *(__p++);
	__spec->aud_format |= (__spec->aud_format&0xFF) | *(__p++)<<8;

	__spec->chn_c |= (__spec->chn_c&0xFF) | *(__p++);
	__spec->chn_c |= (__spec->chn_c&0xFF) | *(__p++)<<8;

	__spec->sample_rate |= (__spec->sample_rate&0xFF) | *(__p++);
	__spec->sample_rate |= (__spec->sample_rate&0xFF) | *(__p++)<<8;
	__spec->sample_rate |= (__spec->sample_rate&0xFF) | *(__p++)<<16;
	__spec->sample_rate |= (__spec->sample_rate&0xFF) | *(__p++)<<24;

	__spec->byte_rate |= (__spec->byte_rate&0xFF) | *(__p++);
	__spec->byte_rate |= (__spec->byte_rate&0xFF) | *(__p++)<<8;
	__spec->byte_rate |= (__spec->byte_rate&0xFF) | *(__p++)<<16;
	__spec->byte_rate |= (__spec->byte_rate&0xFF) | *(__p++)<<24;

	__spec->blk_align |= (__spec->blk_align&0xFF) | *(__p++);
	__spec->blk_align |= (__spec->blk_align&0xFF) | *(__p++)<<8;

	__spec->bit_depth |= (__spec->bit_depth&0xFF) | *(__p++);
	__spec->bit_depth |= (__spec->bit_depth&0xFF) | *(__p++)<<8;

	__spec->sub_cnk2_id[0] = *(__p++);
	__spec->sub_cnk2_id[1] = *(__p++);
	__spec->sub_cnk2_id[2] = *(__p++);
	__spec->sub_cnk2_id[3] = *(__p++);

	__spec->sub_cnk2_size |= (__spec->sub_cnk2_size&0xFF) | *(__p++);
	__spec->sub_cnk2_size |= (__spec->sub_cnk2_size&0xFF) | *(__p++)<<8;
	__spec->sub_cnk2_size |= (__spec->sub_cnk2_size&0xFF) | *(__p++)<<16;
	__spec->sub_cnk2_size |= (__spec->sub_cnk2_size&0xFF) | *(__p++)<<24;
}

/*
ffly_err_t ffly_aud_play(ffly_byte_t *__fp, ffly_size_t __fsize, ffly_aud_fformat_t __fformat) {
	ffly_aud_spec_t aud_spec;
	ffly_off_t off;
	switch(__fformat) {
		case _ffly_audf_wav: {
			ffly_wav_spec_t wav_spec = ffly_extract_wav_spec(__fp);
			if (wav_spec.aud_format) {
				switch(wav_spec.bit_depth) {
					case 16:
						aud_spec.format = _ffly_af_s16_le;
					break;
					default: return FFLY_FAILURE;
				}

				aud_spec.chn_c = wav_spec.chn_c;
				aud_spec.rate = wav_spec.sample_rate;
            	off = sizeof(ffly_wav_spec_t);
			} else
				return FFLY_FAILURE;
			break;
		}
		default:
			ffly_fprintf(ffly_err, "ffly_audio, file format not rq.\n");
		return FFLY_FAILURE;
	}

	ffly_aud_raw_play(__fp+off, __fsize-off, &aud_spec);
	return FFLY_SUCCESS;
}

ffly_err_t ffly_aud_raw_play(ffly_byte_t *__p, ffly_size_t __size, ffly_aud_spec_t *__aud_spec) {
# ifdef __USING_PULSE_AUDIO
	if (ffly_pulse_write(&__pulse__, __p, __size) != FFLY_SUCCESS) {
		ffly_fprintf(ffly_err, "ffly_audio: failed to write.\n");
		return FFLY_FAILURE;
	}
# endif
	return FFLY_SUCCESS;
}
*/

ffly_byte_t static *w_buff = NULL, *r_buff = NULL;
ffly_byte_t static *wbuf_itr, *rbuf_itr;
ffly_size_t static wbuf_size, rbuf_size;
ffly_err_t ffly_audio_init(ffly_aud_spec_t *__aud_spec) {
	w_buff = (ffly_byte_t*)__ffly_mem_alloc((wbuf_size = (1<<20)));
	wbuf_itr = w_buff;

	r_buff = (ffly_byte_t*)__ffly_mem_alloc((rbuf_size = (1<<20)));
	rbuf_itr = r_buff;
# ifdef __ffly_use_pulse_audio
	struct pa_sample_spec sample_spec;
	switch(__aud_spec->format) {
		case _ffly_af_s16_le:
			sample_spec.format = PA_SAMPLE_S16LE;
		break;
		case _ffly_af_float32_le:
			sample_spec.format = PA_SAMPLE_FLOAT32LE;
		break;
		default:
            ffly_fprintf(ffly_err, "audio format not supported.\n");
        return FFLY_FAILURE;
	}

	sample_spec.channels = __aud_spec->chn_c;
	sample_spec.rate = __aud_spec->rate;
    ffly_err_t err;
	if (_err(err = ffly_pulse_init(&__pulse__, "pulse", "ffly-audio", &sample_spec))) {
        ffly_fprintf(ffly_err, "failed to init pulse audio.\n");
        return err;
    }
# endif
# ifdef __ffly_use_alsa_audio
    snd_pcm_format_t format;
    switch(__aud_spec->format) {
        case _ffly_af_s16_le:
            format = SND_PCM_FORMAT_S16_LE;
        break;
        case _ffly_af_float32_le:
            format = SND_PCM_FORMAT_FLOAT_LE;
        break;
        default:
            ffly_fprintf(ffly_err, "audio format not supported.\n");
        return FFLY_FAILURE;
    }
    ffly_err_t err;
    if (_err(err = ffly_alsa_init(&__alsa__, __aud_spec->rate, __aud_spec->chn_c, format, 16, 100))) {
        ffly_fprintf(ffly_err, "failed to init alsa audio.\n");
        return err;
    }
# endif
	return FFLY_SUCCESS;
}

ffly_err_t ffly_audio_de_init() {
	__ffly_mem_free(w_buff);
	__ffly_mem_free(r_buff);
	w_buff = NULL;
	r_buff = NULL;
# ifdef __ffly_use_pulse_audio
	ffly_pulse_free(&__pulse__);
# endif
# ifdef __ffly_use_alsa_audio
    ffly_alsa_free(&__alsa__);
# endif
    return FFLY_SUCCESS;
}

/*
ffly_err_t ffly_aud_raw_play(char *__fdir, char*__fname, ffly_aud_fformat_t __fformat) {
	ffly_byte_t *f;
	ffly_size_t fsize;
	ffly_ld_aud_file(__fdir, __fname, __fformat, &f, &fsize);
	ffly_aud_spec_t aud_spec;

	ffly_byte_t off;
	switch(__fformat) {
		case _ffly_audf_wav: {
			ffly_wav_spec_t wav_spec = ffly_extract_wav_spec(f);
			if (wav_spec.aud_format) {
				if (wav_spec.bit_depth == 16) {
					aud_spec.format = _ffly_af_s16_le;
				}
			}

			aud_spec.chn_c = wav_spec.chn_c;
			aud_spec.rate = wav_spec.sample_rate;
			off = sizeof(ffly_wav_spec_t);
			break;
		}
		default:
			return FFLY_FAILURE;
	}

# ifdef __USING_PULSE_AUDIO
	ffly_fprintf(stdout, "ffly_audio: using pulse audio.\n");
	struct pa_sample_spec sample_spec;
	switch(aud_spec.format) {
		case _ffly_af_s16_le:
			sample_spec.format = PA_SAMPLE_S16LE;
		break;
		case _ffly_af_float32_le:
			sample_spec.format = PA_SAMPLE_FLOAT32LE;
		break;
		default:
			return FFLY_FAILURE;
	}
	
	sample_spec.channels = aud_spec.chn_c;
	sample_spec.rate = aud_spec.rate;

	if (ffly_pulse_write("firefly-audio", "firefly-stream", &sample_spec, f+off, fsize-off) != FFLY_SUCCESS) {
		ffly_fprintf(ffly_err, "ffly_audio: failed to write.\n");
		return FFLY_FAILURE;
	}
# else
# ifdef __USING_ALSA_AUDIO
# else
	ffly_fprintf(stdout, "ffly_audio: alsa or pluse need to be defined at compile time.\n");
# endif
# endif
}*/

# include "data/mem_cpy.h"
ffly_err_t ffly_aud_drain() {
# ifdef __ffly_use_pulse_audio
	ffly_pulse_write(&__pulse__, w_buff, wbuf_itr-w_buff);
	ffly_pulse_drain(&__pulse__);
# endif
# ifdef __ffly_use_alsa_audio
    ffly_alsa_write(&__alsa__, w_buff, wbuf_itr-w_buff);
    ffly_alsa_drain(&__alsa__);
# endif
	wbuf_itr = w_buff;
}

ffly_err_t ffly_aud_write(ffly_byte_t *__buff, mdl_uint_t __bc) {
	if ((wbuf_itr-w_buff)+__bc >= wbuf_size) return FFLY_FAILURE;

	ffly_err_t err  = FFLY_SUCCESS;
	if (_ok(err = ffly_mem_cpy(wbuf_itr, __buff, __bc)))
		wbuf_itr+=__bc;
	return err;
}

ffly_err_t ffly_aud_read(ffly_byte_t *__buff, mdl_uint_t __bc) {
	ffly_byte_t static *itr = NULL;
	if (!itr) itr = r_buff;

	if ((rbuf_itr-itr)<__bc) return FFLY_FAILURE;

	ffly_err_t err = FFLY_SUCCESS;
	if (_ok(err = ffly_mem_cpy(__buff, itr, __bc)))
		itr+=__bc;
	return err;
}
