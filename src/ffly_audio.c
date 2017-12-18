# include "ffly_audio.h"
# include "system/io.h"
# include "system/errno.h"
# include <mdl/str_cmb.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/stat.h>
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "types/off_t.h"
# ifdef __USING_PULSE_AUDIO
struct ffly_pulse __pulse__;
# endif
ffly_err_t ffly_ld_aud_file(char *__dir, char *__name, ffly_aud_fformat_t __format, ffly_byte_t **__p, ffly_size_t *__size) {
	char *ext;
	switch(__format) {
		case _ffly_audf_wav:
				ext = ".wav";
		break;
		case _ffly_audf_mp3:
				ext = ".mp3";
		break;
		default:
			return FFLY_FAILURE;
	}

	char *fpth = mdl_str_cmb(__dir, mdl_str_cmb(__name, ext, 0), _mdl_stc_free_rhs);
	int fd;
	if ((fd = open(fpth, O_RDONLY)) < 0) {
		ffly_fprintf(ffly_err, "ffly_audio: failed to open audio file at '%s'.\n", fpth);
		return FFLY_FAILURE;
	}

	struct stat st;
	if (stat(fpth, &st) < 0) {
		ffly_fprintf(ffly_err, "ffly_audio: failed to stat audio file at '%s'.\n", fpth);
		close(fd);
		return FFLY_FAILURE;
	}

	__ffly_mem_free(fpth);

	*__size = st.st_size;
	*__p = (ffly_byte_t*)__ffly_mem_alloc(st.st_size);
	read(fd, *__p, st.st_size);
	close(fd);
	return FFLY_SUCCESS;
}

# include "data/mem_set.h"
ffly_wav_spec_t ffly_extract_wav_spec(ffly_byte_t *__p) {
	ffly_wav_spec_t wav_spec;
	ffly_mem_set(&wav_spec, 0, sizeof(ffly_wav_spec_t));

	wav_spec.cnk_id[0] = *(__p++);
	wav_spec.cnk_id[1] = *(__p++);
	wav_spec.cnk_id[2] = *(__p++);
	wav_spec.cnk_id[3] = *(__p++);

	wav_spec.cnk_size |= (wav_spec.cnk_size & 0xFF) | *(__p++);
	wav_spec.cnk_size |= (wav_spec.cnk_size & 0xFF) | *(__p++) << 8;
	wav_spec.cnk_size |= (wav_spec.cnk_size & 0xFF) | *(__p++) << 16;
	wav_spec.cnk_size |= (wav_spec.cnk_size & 0xFF) | *(__p++) << 24;

	wav_spec.format[0] = *(__p++);
	wav_spec.format[1] = *(__p++);
	wav_spec.format[2] = *(__p++);
	wav_spec.format[3] = *(__p++);

	wav_spec.sub_cnk1_id[0] = *(__p++);
	wav_spec.sub_cnk1_id[1] = *(__p++);
	wav_spec.sub_cnk1_id[2] = *(__p++);
	wav_spec.sub_cnk1_id[3] = *(__p++);

	wav_spec.sub_cnk1_size |= (wav_spec.sub_cnk1_size & 0xFF) | *(__p++);
	wav_spec.sub_cnk1_size |= (wav_spec.sub_cnk1_size & 0xFF) | *(__p++) << 8;
	wav_spec.sub_cnk1_size |= (wav_spec.sub_cnk1_size & 0xFF) | *(__p++) << 16;
	wav_spec.sub_cnk1_size |= (wav_spec.sub_cnk1_size & 0xFF) | *(__p++) << 24;

	wav_spec.aud_format |= (wav_spec.aud_format & 0xFF) | *(__p++);
	wav_spec.aud_format |= (wav_spec.aud_format & 0xFF) | *(__p++) << 8;

	wav_spec.chn_c |= (wav_spec.chn_c & 0xFF) | *(__p++);
	wav_spec.chn_c |= (wav_spec.chn_c & 0xFF) | *(__p++) << 8;

	wav_spec.sample_rate |= (wav_spec.sample_rate & 0xFF) | *(__p++);
	wav_spec.sample_rate |= (wav_spec.sample_rate & 0xFF) | *(__p++) << 8;
	wav_spec.sample_rate |= (wav_spec.sample_rate & 0xFF) | *(__p++) << 16;
	wav_spec.sample_rate |= (wav_spec.sample_rate & 0xFF) | *(__p++) << 24;

	wav_spec.byte_rate |= (wav_spec.byte_rate & 0xFF) | *(__p++);
	wav_spec.byte_rate |= (wav_spec.byte_rate & 0xFF) | *(__p++) << 8;
	wav_spec.byte_rate |= (wav_spec.byte_rate & 0xFF) | *(__p++) << 16;
	wav_spec.byte_rate |= (wav_spec.byte_rate & 0xFF) | *(__p++) << 24;

	wav_spec.blk_align |= (wav_spec.blk_align & 0xFF) | *(__p++);
	wav_spec.blk_align |= (wav_spec.blk_align & 0xFF) | *(__p++) << 8;

	wav_spec.bit_depth |= (wav_spec.bit_depth & 0xFF) | *(__p++);
	wav_spec.bit_depth |= (wav_spec.bit_depth & 0xFF) | *(__p++) << 8;

	wav_spec.sub_cnk2_id[0] = *(__p++);
	wav_spec.sub_cnk2_id[1] = *(__p++);
	wav_spec.sub_cnk2_id[2] = *(__p++);
	wav_spec.sub_cnk2_id[3] = *(__p++);

	wav_spec.sub_cnk2_size |= (wav_spec.sub_cnk2_size & 0xFF) | *(__p++);
	wav_spec.sub_cnk2_size |= (wav_spec.sub_cnk2_size & 0xFF) | *(__p++) << 8;
	wav_spec.sub_cnk2_size |= (wav_spec.sub_cnk2_size & 0xFF) | *(__p++) << 16;
	wav_spec.sub_cnk2_size |= (wav_spec.sub_cnk2_size & 0xFF) | *(__p++) << 24;
	return wav_spec;
}

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

ffly_byte_t static *w_buff = NULL, *r_buff = NULL;
ffly_byte_t static *wbuf_itr, *rbuf_itr;
ffly_size_t static wbuf_size, rbuf_size;
ffly_err_t ffly_audio_init(ffly_aud_spec_t *__aud_spec) {
	w_buff = (ffly_byte_t*)__ffly_mem_alloc((wbuf_size = (1<<20)));
	wbuf_itr = w_buff;

	r_buff = (ffly_byte_t*)__ffly_mem_alloc((rbuf_size = (1<<20)));
	rbuf_itr = r_buff;
# ifdef __USING_PULSE_AUDIO
	struct pa_sample_spec sample_spec;
	switch(__aud_spec->format) {
		case _ffly_af_s16_le:
			sample_spec.format = PA_SAMPLE_S16LE;
		break;
		case _ffly_af_float32_le:
			sample_spec.format = PA_SAMPLE_FLOAT32LE;
		break;
		default: return FFLY_FAILURE;
	}

	sample_spec.channels = __aud_spec->chn_c;
	sample_spec.rate = __aud_spec->rate;

	ffly_pulse_init(&__pulse__, "pulse", "ffly-audio", &sample_spec);
# endif
	return FFLY_SUCCESS;
}

ffly_err_t ffly_audio_de_init() {
	__ffly_mem_free(w_buff);
	__ffly_mem_free(r_buff);
	w_buff = NULL;
	r_buff = NULL;
# ifdef __USING_PULSE_AUDIO
	ffly_pulse_free(&__pulse__);
# endif
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
	ffly_pulse_write(&__pulse__, w_buff, wbuf_itr-w_buff);
	ffly_pulse_drain(&__pulse__);
	wbuf_itr = w_buff;
}

ffly_err_t ffly_aud_write(ffly_byte_t *__buff, ffly_size_t __size) {
	if ((wbuf_itr-w_buff)+__size >= wbuf_size) return FFLY_FAILURE;

	ffly_err_t any_err = FFLY_SUCCESS;
	if ((any_err = ffly_mem_cpy(wbuf_itr, __buff, __size)) == FFLY_SUCCESS)
		wbuf_itr+=__size;
	return any_err;
}

ffly_err_t ffly_aud_read(ffly_byte_t *__buff, ffly_size_t __size) {
	ffly_byte_t static *itr = NULL;
	if (!itr) itr = r_buff;

	if ((rbuf_itr-itr)<__size) return FFLY_FAILURE;

	ffly_err_t any_err = FFLY_SUCCESS;
	if ((any_err = ffly_mem_cpy(__buff, itr, __size)) == FFLY_SUCCESS)
		itr+=__size;
	return any_err;
}
