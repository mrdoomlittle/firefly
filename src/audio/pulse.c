# include "pulse.h"
# include "../system/io.h"
# include "../system/errno.h"
# include "../ffly_maths.h"
# include "../system/errno.h"
ffly_err_t ffly_pulse_init(struct ffly_pulse *__pulse, char *__name, char *__stream_name, struct pa_sample_spec *__sample_spec) {
	int any_err;
	__pulse->sample_spec = *__sample_spec;
	if (!(__pulse->simple = pa_simple_new(NULL, __name, PA_STREAM_PLAYBACK, NULL, __stream_name, &__pulse->sample_spec, NULL, NULL, &any_err))) {
		ffly_printf(stderr, "pluse: failed to create new sample, err: %d\n", any_err);
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_pulse_write(struct ffly_pulse *__pulse, ffly_byte_t *__p, ffly_size_t __size) {
	int any_err;
	if (pa_simple_write(__pulse->simple, __p, __size, &any_err) < 0) {
		ffly_printf(stderr, "pulse: failed to write, err: %d\n", any_err);
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_pulse_drain(struct ffly_pulse *__pulse) {
	int any_err;
	if (pa_simple_drain(__pulse->simple, &any_err) < 0) {
		ffly_printf(stderr, "pulse: failed to drain, err: %d\n", any_err);
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_pulse_free(struct ffly_pulse *__pulse) {

}
