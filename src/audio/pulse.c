# include "pulse.h"
# include "../system/io.h"
# include "../system/errno.h"
# include "../ffly_maths.h"
# include "../system/errno.h"
ffly_err_t ffly_pulse_write(char *__name, char *__stream_name, struct pa_sample_spec *__sample_spec, ffly_byte_t *__p, ffly_size_t __size) {
	pa_simple *simple = NULL;
	int any_err;

	if (!(simple = pa_simple_new(NULL, __name, PA_STREAM_PLAYBACK, NULL, __stream_name, __sample_spec, NULL, NULL, &any_err))) {
		ffly_printf(stderr, "pluse: failed to create new sample, err: %d\n", any_err);
		return FFLY_FAILURE;
	}

	ffly_size_t size;
	switch(__sample_spec->format) {
		case PA_SAMPLE_S16LE:
			size = ffly_floor(size/(__sample_spec->channels*2))*(__sample_spec->channels*2);
		break;
		case PA_SAMPLE_FLOAT32LE:
			size = ffly_floor(size/(__sample_spec->channels*4))*(__sample_spec-> channels*4);
		break;
		default: break;
	}

	if (pa_simple_write(simple, __p, size, &any_err) < 0) {
		ffly_printf(stderr, "pulse: failed to write, err: %d\n", any_err);
		pa_simple_free(simple);
		return FFLY_FAILURE;
	}

	if (pa_simple_drain(simple, &any_err) < 0) {
		ffly_printf(stderr, "pulse: failed to drain, err: %d\n", any_err);
		pa_simple_free(simple);
		return FFLY_FAILURE;
	}

	pa_simple_free(simple);
	return FFLY_SUCCESS;
}
