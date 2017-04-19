# include "pulse_audio.hpp"
static const pa_sample_spec def_sample_spec = {
	.format = PA_SAMPLE_S16LE,
	.rate = 44100,
	.channels = 2
};

mdl::firefly::types::err_t mdl::firefly::pulse_audio::load_sample(char const *__fdir, char const *__fname, types::aftype_t __aftype) {
	char *fpth = strcmb(const_cast<char *>(__fdir), const_cast<char *>(__fname), STRCMB_FREE_NONE);

	int fd;
	if ((fd = open(fpth, O_RDONLY)) < 0) {
		fprintf(stderr, "pulse_audio: failed to open file, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	if (dup2(fd, STDIN_FILENO) < 0) {
		fprintf(stderr, "pulse_audio: failed to duplicate file descriptor, errno: %d\n", errno);
		close(fd);
		return FFLY_FAILURE;
	}

	struct stat st;
	if (stat(fpth, &st) == 0)
		this-> fbuff_size = st.st_size;
	else {
		fprintf(stderr, "failed to get file size.\n");
		return FFLY_FAILURE;
	}

	printf("loading sound file with size: %d\n", this-> fbuff_size);
	if (this-> fbuff != nullptr) memory::mem_free(this-> fbuff);

	this-> fbuff = (u8_t *)memory::mem_alloc(this-> fbuff_size * sizeof(u8_t));
	bzero(this-> fbuff, this-> fbuff_size * sizeof(u8_t));

	read(STDIN_FILENO, this-> fbuff, this-> fbuff_size * sizeof(u8_t));

	close(fd);

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::pulse_audio::play_sample(char const *__fdir, char const *__fname, types::aftype_t __aftype, types::aformat_t __aformat) {
	this-> load_sample(__fdir, __fname, __aftype);
	this-> load_info(__aftype, __aformat);
	this-> play_sample(__aftype, __aformat);
}

mdl::firefly::types::err_t mdl::firefly::pulse_audio::play_sample(types::adisc_t __adisc, u8_t *__fbuff, off_t __fbuff_size) {
	static pa_sample_spec sample_spec;
	pa_sample_format_t format;

	switch (__adisc.format) {
		case system::S16_LE:
			format = PA_SAMPLE_S16LE;
		break;
		case system::FLOAT32_LE:
			format = PA_SAMPLE_FLOAT32LE;
		break;
	}

	sample_spec.format = format;
	sample_spec.rate = __adisc.rate;
	sample_spec.channels = __adisc.chn_c;
	this-> play_sample("firefly-audio", "firefly-stream", &sample_spec);
}

mdl::firefly::types::err_t mdl::firefly::pulse_audio::play_sample(char const *__name, char const *__stream_name, struct pa_sample_spec *__sample_spec, u8_t *__abuff, std::size_t __abuff_size) {
	if (this-> fbuff == nullptr) {
		fprintf(stderr, "audio file buff empty.\n");
		return FFLY_FAILURE;
	}

	pa_simple *simple = NULL;

	int any_error;

	std::size_t abuff_size = 0;
	if (!(simple = pa_simple_new(NULL, __name, PA_STREAM_PLAYBACK, NULL, __stream_name, __sample_spec == nullptr? &def_sample_spec : __sample_spec, NULL, NULL, &any_error))) {
		fprintf(stderr, "pulse_audio: failed to create new pulse audio sample.\n");
		goto failure;
	}

	if (!simple) {
		fprintf(stderr, "pulse_audio: looks like it failed to create sample.\n");
		goto failure;
	}

	abuff_size = __abuff == nullptr? this-> abuff_size : __abuff_size;
	if (pa_simple_write(simple, this-> abuff, abuff_size * sizeof(u8_t), &any_error) < 0) {
		fprintf(stderr, "failed to write pa sample, errno: %s\n", pa_strerror(any_error));
		goto failure;
	}

	if (pa_simple_drain(simple, &any_error) < 0) {
		fprintf(stderr, "failed to drain pa sample, errno: %s\n", pa_strerror(any_error));
		goto failure;
	}

	if (simple) pa_simple_free(simple);
	return FFLY_SUCCESS;
	failure:

	if (simple) pa_simple_free(simple);
	return  FFLY_FAILURE;
}
